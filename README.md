CITS2002: Round-Robin Process Scheduler

Student: Han Nguyen 
Student ID: 

Overview

This C program simulates a round-robin CPU scheduling with I/O events. Each process has:

Name

Total CPU time required

Optional I/O events, defined as pairs of turn number and runtime before the block

The scheduler runs each process in turns, handling I/O blocks and finishing processes when their total CPU time is reached.

Features

Handles multiple processes with dynamic I/O events

Fixed time quantum of 10 units

Tracks the number of CPU turns each process has had

Prints the completion time for each process

Input Format

Input file: input (text file)

Each line represents a process:

<ProcessName> <TotalTime> [<IO_Turn1> <IO_Run1> <IO_Turn2> <IO_Run2> ...]


<ProcessName>: string

<TotalTime>: integer, total CPU time required

<IO_TurnX>: integer, CPU turn number when an I/O occurs

<IO_RunX>: integer, CPU runtime before being blocked

Example:

P1 30 2 5 4 3
P2 20
P3 25 1 10


Output

The program prints the completion time of each process:

P2 completed at time 20
P1 completed at time 30
P3 completed at time 40
