#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

#define UADDR_BOTTOM ((void*)0x08048000)

static void syscall_handler (struct intr_frame *);
void address_validity(void *addr);							/* check if stack pointer is in user memory
														 		 if not, terminate process, added by JeonHaeSeong*/
void buffer_validity(void *addr, size_t n);					/* check if buffer is valid */

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* f->esp has syscall number */

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
	// check if pointer is valid
	address_validity(f->esp);
	switch(*(int*)(f->esp)) {
		case SYS_HALT:break;
		case SYS_EXIT:break;
		case SYS_EXEC:break;
		case SYS_WAIT:break;
		case SYS_READ:break;
		case SYS_SEEK:break;
		default:break;
	}
    thread_exit ();
}

void
address_validity(void *addr) {
	if(!is_user_vaddr(addr) || addr < UADDR_BOTTOM)
		exit(-1);
}

void
buffer_validity(void *addr, size_t n) {
	int i;
	for(i=0;i<n;i++)
		address_validity(
}

// shutdown pintos
void 
halt(void) {
	shutdown_power_off();
}


// return status to the kernel or parent
void 
exit(int status) {
	struct thread *now = thread_current();

}
/* it works only if fd == 0(stdin) */
int read(int fd, void *buffer, unsigned size) {
	int i;
	char input;
	if(fd == 0) {
		for(i=0;i<size;i++) {
			input = input_getc();
			memcpy(buffer,&input,1);
		}
		return size;
	}
}
/* it works only if fd == 1(stdout) */
int write(int fd, const void *buffer, unsigned size) {
	if(fd == 1) {
		putbuf(buffer, size);
		return size;
	}
}
