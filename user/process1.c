// Path: /user/process1.c
// Created by CS3103 Group 70

#include "library/syscalls.h"
#include "library/stdio.h"

void runForSeconds(int seconds) {
    unsigned int startTime;
    syscall_system_time(&startTime);
    unsigned int timeElapsed;
    do {
        syscall_system_time(&timeElapsed);
        timeElapsed -= startTime;
    } while (timeElapsed < seconds);
}

int main(int argc, char **argv) {
    printf("Process 1 (PID: %d, Priority: %d) is running for 1 second\n", syscall_process_self(), syscall_process_priority());
    runForSeconds(1);
    return 0;
}
