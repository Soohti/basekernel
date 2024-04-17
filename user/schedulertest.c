// Path: /user/schedulertest.c
// Created by CS3103 Group 70

#include "library/syscalls.h"
#include "library/stdio.h"
#include "library/errno.h"

void schedule_process(const char *exec, int priority) {
    int pfd = syscall_open_file(KNO_STDDIR, exec, 0, 0);

    if (pfd >= 0) {
        int pid = syscall_process_run_with_priority(pfd, 0, &exec, priority);
        if (pid > 0) {
            printf("[INFO] Process %s (PID: %d) is scheduled with priority %d\n", exec, pid, priority);
        } else {
            printf("[ERROR] Failed to create process %s (Error code: %s)\n", exec, strerror(pid));
        }
    } else {
        printf("[ERROR] Failed to open file %s (Error code: %s)\n", exec, strerror(pfd));
    }
}

int main(int argc, char** argv) {
    printf("[INFO] Scheduler test started\n");

    int n = 5, i;
    const char *execs[] = {"bin/process1.exe", "bin/process2.exe", "bin/process3.exe", "bin/process4.exe", "bin/process5.exe"};
    int priorities[] = {9, 7, 2, 1, 5};
    for (i = 0; i < n; i++) {
        schedule_process(execs[i], priorities[i]);
    }
    printf("[INFO] Processes will start running\n");
    syscall_process_run_blocked();
    return 0;
}
