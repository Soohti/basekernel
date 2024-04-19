// Path: /user/sender.c
// Created by CS3103 Group 70

#include "library/syscalls.h"

int main()
{
    char *fname = "/test_pipe";
    int fd = syscall_open_named_pipe(fname);
    char *msg = "Hello, receiver!";
    syscall_object_write(fd, msg, 20, 0);
}