# The Basekernel Operating System Kernel

Course Project for CS3103 Operating Systems, Semester B 2023/24, City University of Hong Kong.

> [!WARNING]
> All code in this project is for educational purposes only. Please adhere to the principles of academic integrity.

This project is an extension of the [Basekernel Operating System Kernel](https://github.com/dthain/basekernel). The extension includes two parts: priority-based scheduling and named pipe. Please visit our [Changelog](CHANGELOG.md) for more information on the changes.

![Priority-based scheduling](./screenshots/scheduler_test.png)

![Named pipe](./screenshots/named_pipe_test.png)

To learn more about Basekernel, see the [Basekernel Wiki](https://github.com/dthain/basekernel/wiki).

## Usage

A cross-compiler is required to build the kernel.

```bash
$ ./build-cross-compiler.sh
$ export PATH="$PWD/cross/bin:$PATH"
$ make run
```

Alternatively, you can download the kernel image built by an automated workflow in the [Releases](https://github.com/Soohti/basekernel/releases) and run it with QEMU:

```bash
$ qemu-img create disk.img 10M
$ qemu-system-i386 -cdrom basekernel.iso -hda disk.img
```
