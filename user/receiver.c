// Path: /user/receiver.c
// Created by CS3103 Group 70

#include "library/syscalls.h"

int main()
{
    char *fname = "/data/test_pipe";
    int res = syscall_make_named_pipe(fname);
    if (res != 1)
    {
        printf("[ERROR] Failed to create named pipe at %s (Error code: %s)\n", fname, strerror(res));
        return 1;
    }
    int fd = syscall_open_named_pipe(fname);
    if (fd < 0)
    {
        printf("[ERROR] Error code: %s\n", strerror(fd));
        return 2;
    }
    printf("[INFO] Successfully created named pipe at %s.\n", fname);
    printf("[INFO] Waiting for message...\n");
    char *buffer = malloc(25);
    syscall_object_read(fd, buffer, 20, 0);
    printf("%s\n", buffer);
    syscall_destroy_named_pipe(fname);
    printf("[INFO] Destroyed named pipe at %s.\n", fname);
    return 0;
}