#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/thread.h"
#include "threads/interrupt.h"


#include <string.h>
#include "devices/shutdown.h"
#include "devices/input.h"
#include "lib/user/syscall.h"
#include "userprog/exception.h"
#include "userprog/process.h"
#define UADDR_BOTTOM ((void*)0x08048000)


void sys_halt(void);
int sys_read(int fd, void* buffer, size_t size);
int sys_write(int fd, const void* buffer, size_t size);
pid_t sys_exec(const char* cmd_line);
int sys_wait(pid_t pid);

static void syscall_handler (struct intr_frame *);

void 
get_args(struct intr_frame *f, void **args, int cnt);		/* get arguments the syscall need and store at args */

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* f->esp has syscall number */

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	void* args[4] = {NULL};
	// check if pointer is valid
	if(!address_validity(f->esp)) sys_exit(-1);
	switch(*(int*)(f->esp)) {
		case SYS_HALT:
			{
				sys_halt();
				break;
			}
		case SYS_EXIT:break;
		case SYS_EXEC:
			{
				printf("EXEC\n");
				get_args(f,args,1);
				f->eax = sys_exec(*(const char**)args[0]);
				break;
			}
		case SYS_WAIT:break;
		case SYS_READ:
			{
				/* args[0] = fd , args[1] = buffer , args[2] = size */
//				hex_dump(0,f->esp,20,true);
				get_args(f,args,3);
				if(!buffer_validity(args[1],*(size_t*)args[2])) sys_exit(-1);
				f->eax = sys_read(*(int*)args[0],args[1],*(size_t*)args[2]);
				break;
			}
		case SYS_WRITE:
			{
				/* args[0] = fd , args[1] = buffer , args[2] = size */
				get_args(f,args,3);
				if(!buffer_validity(args[1] , *(size_t*)args[2])) sys_exit(-1);
				f->eax = sys_write(*(int*)args[0],args[1],*(size_t*)args[2]);
				break;
			}
		case SYS_PIB:
			{
				break;
			}
		case SYS_SUM:
			{
				break;
			}
		default:break;
	}
//    thread_exit ();
}

/* f->esp has syscall_num, +1, +2, +3, +4 has arguments' address */
void
get_args(struct intr_frame *f, void **args, int cnt) {
	int i;
	void *ptr = f->esp;
	for(i=0;i<cnt;i++) {
		ptr = ptr+4;
		address_validity(ptr);
		args[i] = ptr;
	}
}

// shutdown pintos
void 
sys_halt(void) {
	shutdown_power_off();
}


// return status to the kernel or parent
void 
sys_exit(int status) {
	// written by Kwon Myung Joon
	struct thread *cur = thread_current();
	cur->parent->child_status = status; // set child exit status in parent thread


	// delete this thread from parent->child_list
	struct list_elem* e;
	for(e=list_begin(&(cur->parent->child_list));e != list_end(&(cur->parent->child_list));e=list_next(e)) {
		if(cur->parent->cur_child == cur->tid ) {
			list_remove(e);
			break;
		}
	}
	printf("%s: exit(%d)\n",cur->name,status);
	thread_exit();
}

pid_t
sys_exec(const char *cmd_line) {
	/* process_execute -> thread_create -> kernel_thread -> start_process -> load */
	/* Funcs above need to be modified */
	// TO-DO : 
	return process_execute(cmd_line);
}

int
sys_wait(pid_t pid) {
	return process_wait(pid);
}
/* it works only if fd == 0(stdin) */
int 
sys_read(int fd, void *buffer, unsigned size) {
	int i;
	char input;
	if(fd == 0) {
		for(i=0;i<size;i++) {
			input = input_getc();
			memset((char*)buffer+i,input,1);
		}
		return size;
	}
}
/* it works only if fd == 1(stdout) */
int 
sys_write(int fd, const void *buffer, unsigned size) {
	if(fd == 1) {
		putbuf(buffer, size);
		return size;
	}
}
