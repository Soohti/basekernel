# Part 1: Priority-Based Scheduling

## 1.1 Implementation Result and Testing

We have successfully implemented the implemented the priority scheduling into the basekernel. We also created test programs that executed properly.

![Screenshot of scheduler test program](./screenshots/scheduler_test.png)

To use the test programs, follow the steps below:

### Step 1: Compile and run basekernel

```bash
make
qemu-system-i386 -cdrom basekernel.iso
```

### Step 2: Mount the disk image

In QEMU:

```bash
kshell > automount
```

### Step 3: Run schedulertest.exe

```bash
kshell > run /bin/schedulertest.exe
```

## 1.2 Abstract Idea and Mechanism Design

Abstract idea: To implement priority-based scheduling, we need to maintain a priority queue of processes. The scheduler will select the process with the smallest priority value to run next.

Mechanism design: The implementation involves three key modifications:

1. Extending `list.h` to support priority queues. `list.h` is a generic linked list implementation that Basekernel uses for FCFS scheduling. We add several functions to support priority-based insertions and deletions, where the priority is stored as the `priority` field in the `list_node` structure.
2. Introducing a temporarily blocked queue in `process.h`. To simulate the scenario where multiple processes arrive at the same time, we need to store the processes in a temporarily blocked queue before they are ready to run. When they are added to the queue, they are sorted based on their priority. Then, the system can take the process with the highest priority from the queue and add it to the ready queue for execution.
3. Adding system calls to allow user programs to interact with the scheduler. This includes scheduling a process with a specific priority and getting the current process's priority.

## 1.3 Implementation Details

`kernel/list.h`, `kernel/list.c`:

- Modified function `void list_push_head(struct list *list, struct list_node *node)`
    - Purpose: Insert a node at the head of the list.
    - Parameters:
        - `list`: The list to insert the node into.
        - `node`: The node to insert.
    - Changes:
        - Added a default priority value of 1024 to the node, so as to make it the lowest priority.
- New function `void list_push_head_priority(struct list *list, struct list_node *node, int pri)`
    - Purpose: Insert a node at the head of the list with a specific priority.
    - Parameters:
        - `list`: The list to insert the node into.
        - `node`: The node to insert.
        - `pri`: The priority of the node.
- Modified function `void list_push_tail(struct list *list, struct list_node *node)`
    - Purpose: Insert a node at the tail of the list.
    - Parameters and changes are the same as `list_push_head`.
- New function `void list_push_tail_priority(struct list *list, struct list_node *node, int pri)`
    - Purpose: Insert a node at the tail of the list with a specific priority.
    - Parameters are the same as `list_push_head_priority`.
- Modified function `void list_push_priority(struct list *list, struct list_node *node, int pri)`
    - Purpose: Insert a node into the list based on its priority.
    - Parameters:
        - `list`: The list to insert the node into.
        - `node`: The node to insert.
        - `pri`: The priority of the node.
    - Changes:
        - Changed `list_push_head` and `list_push_tail` calls to `list_push_head_priority` and `list_push_tail_priority`, respectively.
        - Changed the priority ordering to be ascending. A smaller value indicates a higher priority. In case of a tie, the new node is inserted after the existing nodes with the same priority.

`kernel/process.h`, `kernel/process.c`:

- New global variable `struct list blocked_list`
    - Purpose: To store processes that are temporarily blocked.
- Modified function `struct process *process_create()`
    - Purpose: Create a new process.
    - Changes:
        - Added a default priority value of 1024 to the process.
- New function `struct process *process_create_with_priority(int pri)`
    - Purpose: Create a new process with a specific priority.
    - Parameters:
        - `pri`: The priority of the process.
    - Note: Will temporarily block the process so that it cannot run even if in the ready queue.
- New funtcion `void process_launch_with_priority(struct process *p, int pri)`
    - Purpose: Add a process to the blocked queue with a specific priority.
    - Parameters:
        - `p`: The process to launch.
        - `pri`: The priority of the process.
- New function `int process_unblock(struct process *p)`
    - Purpose: Sets the state of a process to ready and adds it to the ready queue.
    - Parameters:
        - `p`: The process to unblock.
    - Returns: 0 if successful, 1 if the process is not blocked at all.
- New function `int process_run_blocked()`
    - Purpose: Move the highest priority process from the blocked queue to the ready queue.
    - Returns: 0 if successful, 1 if the blocked queue is empty.
- New function `int process_run_blocked_all()`
    - Purpose: Move all processes from the blocked queue to the ready queue based on their priority.
    - Returns: 0 if successful, 1 if the blocked queue is empty.

`include/kernel/syscall.h`, `include/library/syscalls.h`, `kernel/syscall_handler.c`, `library/syscalls.c`:

Adds 4 new system calls:

- `int syscall_process_run_with_priority(int fd, int argc, const char **argv, int pri)`
    - Purpose: Schedule a process with a specific priority.
    - Parameters:
        - `fd`: File descriptor.
        - `argc`: Number of arguments.
        - `argv`: Arguments.
        - `pri`: The priority of the process.
- `int syscall_process_priority()`
    - Purpose: Get the priority of the current process.
- `int syscall_process_run_blocked()`
    - Purpose: Run the highest priority process from the blocked queue.
- `int syscall_process_run_blocked_all()`
    - Purpose: Run all processes from the blocked queue based on their priority.

`user/process1.c`, `user/process2.c`, `user/process3.c`, `user/process4.c`, `user/process5.c`:

- 5 test programs that run and print their PID and priority.
- `void runForSeconds(int seconds)` is used to simulate the process running for a specific number of seconds.

`user/schedulertest.c`:

- A test program that schedules the 5 processes with different priorities and runs them.
- `void schedule_process(const char* exec, int priority)` is used to schedule a process with a specific priority.
    - Note: It will prints error messages if the file does not exist or the execution fails.



# Part 2: Named Pipe

## 2.1 Implementation Result and Testing

We have successfully implemented the named pipe into the base kernel. We also created test programs that executed properly. Follow the steps, and you should get the same output as the screenshot.

![Screenshot of named pipe test programs](./screenshots/named_pipe_test.png)

### Step 1: Set up hard disk

In project directory:

```bash
$ make run
```

Inside QEMU:

```bash
kshell> format ata 0 diskfs
kshell> install atapi 2 ata 0
kshell> mount ata 0 diskfs
```

### Step 2: Run multishell.exe

```bash
kshell> run /bin/multishell.exe
```

### Step 3: Run receiver.exe to create named pipe

Use <kbd>Tab</kbd> to switch to the $\nwarrow$ upper left shell:

```bash
shell> run /bin/receiver.exe
```

This will create a named pipe at the specified path.

Next, switch to the $\swarrow$ lower left shell:

```bash
shell> list /data
```

In the output, you will see the named pipe created by the receiver.

### Step 4: Run sender.exe to send message

Use <kbd>Tab</kbd> to switch to the $\nearrow$​ upper right shell:

```bash
shell> run /bin/sender.exe
```

Sender will open the named pipe, and write message to the pipe.

Then, receiver will print the same message.

Finally, both sender and receiver will exit. Receiver will delete the named pipe.

We can switch to the $\searrow$ lower right shell to verify:

```bash
shell> list /data
```

## 2.2 Abstract Idea and Mechanism Design

Abstract idea: To make a named pipe, we need to create a file in the disk file system, create a named pipe object, and bind the file with the named pipe object. To open the named pipe, we need to find the named pipe object binded with the file. We can then read and write to the named pipe object. We stored the relationship between a file and a named pipe object in the inode of the file.

Mechanism design: The implementation involves several key modifications:

1. Define the structure of a named pipe:

   ```c
   struct named_pipe
   {
     	struct pipe *p;
     	int refcount;
   };
   ```
   
   We reused the original pipe structure to store the data of the named pipe. We also added a reference count to keep track of the number of processes using the named pipe.
   
   * When a process uses syscall to open a named pipe, the reference count is increased.
   * When a process exits, the reference count is decreased. If the reference count becomes zero, the named pipe structure is freed.

2. Extending the `diskfs.c` to support named pipes. We added a new pointer `struct named_pipe *` to the `struct diskfs_inode` to store the named pipe object binded with the file. We also added functions to bind and get the named pipe object from the file.

3. Extending the `fs.c` to support creating and removing files at a given path.

4. Implementing the named pipe operations. We added functions to create, destroy, open, read, and write to the named pipe. The named pipe operations are similar to the pipe operations, but they are more complex because they involve file system operations.

5. Adding a new kobject type `KOBJECT_NAMED_PIPE` to represent the named pipe. We also added functions to create, copy, write, close, and get the size of the named pipe object.

6. Adding system calls to allow user programs to interact with the named pipe. This includes making, destroying, opening the named pipe.

7. Creating a multishell program to test the named pipe operations. The multishell program creates four shells in parallel, allowing us to test the receiver and sender programs simultaneously.


## 2.3 Implementation Details

`user/receiver.c`:

  ```c
#include "library/errno.h"
#include "library/malloc.h"
#include "library/string.h"
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
  ```

`user/sender.c`:

  ```c
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
  ```

`include/kernel/syscall.h`, `include/library/syscalls.h`, `kernel/syscall_handler.c`, `library/syscalls.c`:

* `int syscall_make_named_pipe(const char *)`:  creates a named pipe at given path, and shows whether there is an error occuring.
* `int syscall_destroy_named_pipe(const char *)`: destroys a named pipe at given path, and deletes the file.
* `int syscall_open_named_pipe(const char *)`: opens a named pipe at given path, and returns the file descriptor.

`include/library/string.h`, `library/string.c`, `kernel/string.h`, `kernel/string.c`:

* `const char *strrchr(const char *, char)`:  returns the last occurrence of given char in the cstring.

`kernel/diskfs.h`, `kernel/diskfs.c`:

* `int diskfs_inode_bind_named_pipe(struct fs_dirent *, struct named_pipe *)`: binds the inode of given file with the named pipe, and returns 1 if succeed. 
* `struct named_pipe *diskfs_inode_get_named_pipe(struct fs_dirent *)`: gets the named pipe binded wih the given file.
* `DISKFS_DIRECT_POINTERS`: The value is changed from 6 to 20 to allow more files in a directory. 
* `struct diskfs_inode`: A new pointer `struct named_pipe *` is added.

`kernel/fs.h`, `kernel/fs.c`:

* `struct fs_dirent *fs_mkfile(const char *)`: first extracts the parent path and file name, and uses `fs_dirent_mkfile(parent, name)` to create the file.
* `int fs_remove(const char *)`: removes the file created at gven path.

`include/kernel/types.h`, `kernel/kobject.h`, `kernel/kobject.c`, `library/kernel/object/string.c`:

* `KOBJECT_NAMED_PIPE`: a new kobject type for named pipe.

* `struct kobject *kobject_create_named_pipe(struct named_pipe *)`: creates a kobject with the type of named pipe.

* `struct kobject * kobject_copy( struct kobject *)`, 

  `struct kobject * kobject_create_dir_from_dir( struct kobject *, const char *)`, 

  ` int kobject_write(struct kobject *, void *, int , kernel_io_flags_t)`,

  ` int kobject_close(struct kobject *)`, 

  `int kobject_size(struct kobject *, int *, int)`: 

  * A case for named pipe is added. 

`kernel/named_pipe.h`, `kernel/named_pipe.c`:

* `int named_pipe_create(char *)`: creates a named pipe at the given path.

* `int named_pipe_destroy(char *)`: deletes the file pointing to the named pipe.

* `struct named_pipe *named_pipe_open(struct fs_dirent *)`: opens a named pipe binded with the given file.

* `int named_pipe_size(struct named_pipe *)`: gets the size of pipe used in the given named pipe. 

* `void named_pipe_flush(struct named_pipe *)`: flushs the pipe used in the given named pipe.

* `void named_pipe_close(struct named_pipe *)`: reduces a reference to the given named pipe.

* `struct named_pipe *named_pipe_addref(struct named_pipe *)`: adds a reference to the given named pipe.

* `int named_pipe_read(struct named_pipe *, char *, int)`, 

  `int named_pipe_read_nonblock(struct named_pipe *, char *, int)`,

  `int named_pipe_write(struct named_pipe *, char *, int)`, 

  `int named_pipe_write_nonblock(struct named_pipe *, char *, int)`: 

  * completes the operations of writing and reading similar to pipe.  