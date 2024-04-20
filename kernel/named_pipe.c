// Path: /kernel/named_pipe.c
// Created by CS3103 Group 70

#include "named_pipe.h"
#include "diskfs.h"
#include "fs.h"
#include "fs_internal.h"
#include "pipe.h"

int named_pipe_create(char *fname)
{
	// create a file which will point to named_pipe
	struct fs_dirent *f = fs_mkfile(fname);

	if (!f)
	{
		return 0;
	}
	struct named_pipe *np = kmalloc(sizeof(*np));
	if (!np)
	{
		fs_dirent_close(f);
		return 0;
	}
	// create a pipe for named_pipe
	np->p = pipe_create();
	np->refcount = 1;

	// connect the inode of file f with named_pipe
	int res = diskfs_inode_bind_named_pipe(f, np);
	fs_dirent_close(f);

	return res;
}

int named_pipe_destroy(char *fname)
{
	// delete the file pointing to named_pipe
	int res = fs_remove(fname);
	return res;
}

struct named_pipe *named_pipe_open(struct fs_dirent *f)
{
	// return named_pipe pointed by the inode of fs_dirent *f
	return f->disk.i_pipe;
}

int named_pipe_size(struct named_pipe *np)
{
	return pipe_size(np->p);
}

void named_pipe_flush(struct named_pipe *np)
{
	pipe_flush(np->p);
}

void named_pipe_delete(struct named_pipe *np)
{
	if (!np)
		return;
	np->refcount--;
	if (np->refcount == 0)
	{
		pipe_delete(np->p);
		kfree(np);
	}
}

struct named_pipe *named_pipe_addref(struct named_pipe *np)
{
	np->refcount++;
	return np;
}

int named_pipe_read(struct named_pipe *np, char *buffer, int size)
{
	return pipe_read(np->p, buffer, size);
}

int named_pipe_read_nonblock(struct named_pipe *np, char *buffer, int size)
{
	return pipe_read(np->p, buffer, size);
}

int named_pipe_write(struct named_pipe *np, char *buffer, int size)
{
	return pipe_write(np->p, buffer, size);
}

int named_pipe_write_nonblock(struct named_pipe *np, char *buffer, int size)
{
	return pipe_write(np->p, buffer, size);
}