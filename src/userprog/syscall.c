#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/vaddr.h"
#include "threads/interrupt.h"
#include "threads/thread.h"

#define UADDR_BOTTOM ((void*)0x08048000)

void sys_exit(int status);
void sys_halt();
pid_t sys_exec(const char* );
int sys_read(int fd, void* buffer, size_t size);
int sys_write(int fd, void* buffer, size_t size);

static void syscall_handler (struct intr_frame *);
bool 
address_validity(void *addr);							/* check if stack pointer is in user memory
														 		 if not, terminate process, added by JeonHaeSeong*/
bool 
buffer_validity(void *addr, size_t n);					/* check if buffer is valid */

void 
get_args(struct intr_frame *f, int *args, int cnt);		/* get arguments the syscall need and store at args */

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* f->esp has syscall number */

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	int args[4];
	// check if pointer is valid
	address_validity(f->esp);
	switch(*(int*)(f->esp)) {
		case SYS_HALT:
			{
				halt();
				break;
			}
		case SYS_EXIT:break;
		case SYS_EXEC:break;
		case SYS_WAIT:break;
		case SYS_READ:
			{
				/* args[0] = fd , args[1] = buffer , args[2] = size */
//				hex_dump(0,f->esp,20,true);
				get_args(f,args,3);
				if(!buffer_validity((void*)args[1],(size_t)args[2])) syscall_exit(-1);
				f->eax = sys_read(args[0],(void*)args[1],(size_t)args[2]);
				break;
			}
		case SYS_WRITE:
			{
				/* args[0] = fd , args[1] = buffer , args[2] = size */
				get_args(f,args,3);
				if(!buffer_validity((void*)args[1] , (size_t)args[2])) syscall_exit(-1);
				f->eax = sys_write(args[0],(void*)args[1],(size_t)args[2]);
				break;
			}
		case SYS_FIB:
			{
				break;
			}
		case SYS_SUM:
			{
				break;
			}
		default:break;
	}
    thread_exit ();
}

bool
address_validity(void *addr) {
	if(!is_user_vaddr(addr) || addr < UADDR_BOTTOM)
		return false;
	return true;
}

bool
buffer_validity(void *addr, size_t n) {
	int i;
	for(i=0;i<n;i++)
		if(!address_validity((char*)addr+i))
			return false;
	return true;
}
/* f->esp has syscall_num, +1, +2, +3 has arguments' address */
void
get_args(struct intr_frame *f, int *args, int cnt) {
	int i;
	int *ptr;
	for(i=0;i<cnt;i++) {
		ptr = (int*)f->esp + i + 1;
		address_validity(ptr);
		args[i] = *ptr;
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
	struct thread *now = thread_current();


	printf("%s: exit(%d)\n",now->name,status);
	thread_exit();
}

pid_t
sys_exec(const char *cmd_line) {
	/* process_execute -> thread_create -> kernel_thread -> start_process -> load */
	/* Funcs above need to be modified */
	// TO-DO : 
	buffer_validity(cmd_line);
	return process_execute(cmd_line);
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

struct child_process *insert_child_procesS(pid_t pid) {
	struct child_process *cp = (struct child_process*)malloc(sizeof(struct child_process));

	cp->pid = pid;
	cp->load = NOT_LOADED;
	cp->wait = false;
	list_push( &thread_current()->child_list , &cp->elem );

	return cp;
}
