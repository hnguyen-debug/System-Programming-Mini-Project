/*  Student: Han Nguyen Thi
    Student ID: 24643978
    CITS2002 - Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TIME_QUANTUM  10
#define BUFERSIZE     512

typedef struct {
    char *name;        // dynamically allocated string
    int total_time;    // total CPU time needed
    int time_used;     // CPU time consumed so far
    int cpu_turn;      // how many CPU turns so far
    int finished;      // 0 = running, 1 = finished
    int io_count;      // number of I/O pairs
    int *io_turns;     // turn numbers
    int *io_runs;      // runtimes before block
} Process;

// return index of io event for this turn, -1 if none
int get_io_index(Process *p, int turn) {
    for (int i = 0; i < p->io_count; i++) {
        if (p->io_turns[i] == turn) return i;
    }
    return -1;
}

int main(void) {
    FILE *fp = fopen("input", "r");
    if (!fp) {
        perror("Open file error!!");
        return 1;
    }

    Process *processes = NULL;
    int process_count = 0;
    char line[BUFERSIZE];

    while (fgets(line, BUFERSIZE, fp) != NULL) {
        char namebufer[BUFERSIZE];
        int total_time;

        // dynamic array for IO numbers
        int *numbers = NULL;
        int n = 0;          // how many numbers stored
        int allocation_size = 0;   // current size

        int parsed_chars = 0;
        if (sscanf(line, "%s %d %n", namebufer, &total_time, &parsed_chars) < 2) {
            continue;
        }

        char *rest = line + parsed_chars;
        int io_info;
        while (sscanf(rest, "%d%n", &io_info, &parsed_chars) == 1) {
            // grow array if needed
            if (n >= allocation_size) {
                allocation_size = (allocation_size == 0) ? 8 : allocation_size * 2;
                numbers = realloc(numbers, allocation_size * sizeof(int));
                if (!numbers) {
                    perror("realloc failed");
                    fclose(fp);
                    return 1;
                }
            }
            numbers[n++] = io_info;
            rest += parsed_chars;
        }

        Process p;
        p.name = malloc(strlen(namebufer) + 1);
        strcpy(p.name, namebufer);
        p.total_time = total_time;
        p.time_used = 0;
        p.cpu_turn = 0;
        p.finished = 0;

        p.io_count = n / 2;
        p.io_turns = malloc(sizeof(int) * p.io_count);
        p.io_runs  = malloc(sizeof(int) * p.io_count);

        for (int i = 0, j = 0; i < n; i += 2, j++) {
            p.io_turns[j] = numbers[i];     // which turn
            p.io_runs[j]  = numbers[i + 1]; // runtime before block
        }

        free(numbers); // clean up

        processes = realloc(processes, sizeof(Process) * (process_count + 1));
        if (!processes) {
            perror("realloc failed");
            fclose(fp);
            return 1;
        }
        processes[process_count] = p;
        process_count++;
    }
    fclose(fp);

    if (process_count == 0) return 0;

    int remaining = process_count;
    int global_time = 0;
    int idx = 0;

    while (remaining > 0) {
        if (processes[idx].finished) {  
            idx = (idx + 1) % process_count;
            continue;
        }

        processes[idx].cpu_turn++;
        int io_index = get_io_index(&processes[idx], processes[idx].cpu_turn);
        int event_ms = (io_index >= 0) ? processes[idx].io_runs[io_index] : TIME_QUANTUM;
        int remaining_ms = processes[idx].total_time - processes[idx].time_used;

        // Case 1: finish this turn
        if (remaining_ms <= event_ms) {
            processes[idx].time_used += remaining_ms;
            global_time              += remaining_ms;
            processes[idx].finished   = 1;
            remaining--;
            printf("%s completed at time %d\n", processes[idx].name, global_time);
            idx = (idx + 1) % process_count;
            continue;
        }

        // Case 2: not finished yet; hit event
        processes[idx].time_used += event_ms;
        global_time              += event_ms;

        idx = (idx + 1) % process_count;
    }

    // free memory
    for (int i = 0; i < process_count; i++) {
        free(processes[i].name);
        free(processes[i].io_turns);
        free(processes[i].io_runs);
    }
    free(processes);

    return 0;
}