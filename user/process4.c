// Path: /user/process4.c
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
    printf("Process 4 (PID: %d) is running for 4 seconds\n", syscall_process_self());
    runForSeconds(4);
    return 0;
}
