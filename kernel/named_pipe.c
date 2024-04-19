// Path: /kernel/named_pipe.c
// Createdd by CS3103 Group 70

#include "named_pipe.h"
#include "diskfs.h"
#include "fs_internal.h"
#include "fs.h"
#include "pipe.h"

struct named_pipe {
	struct pipe *p;
	int refcount;
};

int named_pipe_create(char* fname){
	// create a file which will point to named_pipe
	struct fs_dirent *f = fs_mkfile(fname);

	if(!f){
		return 0;
	}
	struct named_pipe *np = kmalloc(sizeof(*np));
	if(!np){
		fs_dirent_close(f);
		return 0;
	}
	// create a pipe for named_pipe
	np->p=pipe_create();
	np->refcount = 1;

	// connect the inode of file f with named_pipe
	int res = diskfs_inode_bind_named_pipe(f, np);
	fs_dirent_close(f);
	
	return res;
}

struct named_pipe *named_pipe_open(struct fs_dirent *f) {
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
	if(!np) return;
	np->refcount--;
	if(np->refcount==0){
		pipr_delete(np->p);
	}
	kfree(np);
}

struct named_pipe *named_pipe_addref( struct named_pipe *np )
{
	np->refcount++;
	return np;
}


int named_pipe_read(struct named_pipe *np, char *buffer, int size)
{
	return pipe_read_internal(np->p, buffer, size, 1);
}

int named_pipe_read_nonblock(struct named_pipe *np, char *buffer, int size)
{
	return pipe_read_internal(np->p, buffer, size, 0);
}

int named_pipe_write(struct named_pipe *np, char *buffer, int size)
{
	return pipe_write_internal(np->p, buffer, size, 1);
}

int named_pipe_write_nonblock(struct named_pipe *np, char *buffer, int size)
{
	return pipe_write_internal(np->p, buffer, size, 0);
}