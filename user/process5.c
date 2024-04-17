// Path: /user/process5.c
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
    printf("Process 5 (PID: %d, Priority: %d) is running for 5 seconds\n", syscall_process_self(), syscall_process_priority());
    runForSeconds(5);
    return 0;
}
