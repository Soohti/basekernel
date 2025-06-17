// Path: /user/sender.c
// Created by CS3103 Group 70

#include "library/errno.h"
#include "library/string.h"
#include "library/syscalls.h"

int main()
{
    char *fname = "/data/test_pipe";
    int fd = syscall_open_named_pipe(fname);
    if (fd < 0)
    {
        printf("[ERROR] Failed to open named pipe (Error code: %s)\n", strerror(fd));
        return 1;
    }
    printf("[INFO] Successfully opened named pipe at %s\n", fname);
    char *msg = "Hello, receiver!";
    syscall_object_write(fd, msg, 20, 0);
    printf("[INFO] Successfully sent message: %s\n", msg);
    return 0;
}
