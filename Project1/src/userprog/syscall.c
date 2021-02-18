

//-----------------------------------
// system call, system call handler implemented by HJL
// Sogang OS Pintos Project 1 2020 Fall

#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h" //thread_name()
#include "devices/shutdown.h"

// included to use funtions
#include "userprog/process.h" // process_execute(), process_wait()
#include "devices/input.h" // input_getc()
#include "lib/kernel/console.h" // putbuf()
#include "userprog/pagedir.h" // pagedir_get_page()
#include "threads/vaddr.h" // is_user_vaddr

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  //Check User Memory Access
    void *addr = (f->esp);
    check_user_mem_acc(addr);
    
    switch(*(uint32_t *)(f->esp)){
        case SYS_HALT :
            halt();
            break;
        case SYS_EXIT :
            check_user_mem_acc(f->esp + 4);
            exit(*(uint32_t *)(f->esp + 4));
            break;
            
        case SYS_EXEC :
            check_user_mem_acc(f->esp + 4);
            f->eax = exec((const char *)*(uint32_t *)(f->esp + 4));
            break;
            
        case SYS_WAIT :
            check_user_mem_acc(f->esp + 4);
            f-> eax = wait((pid_t)*(uint32_t *)(f->esp + 4));
            break;
            
        case SYS_READ :
            check_user_mem_acc(f->esp + 20);
            check_user_mem_acc(f->esp + 24);
            check_user_mem_acc(f->esp + 28);
            f->eax = read((int)*(uint32_t *)(f->esp+20), (void *)*(uint32_t *)(f->esp + 24), (unsigned)*((uint32_t *)(f->esp + 28)));
            break;
            
        case SYS_WRITE :
            check_user_mem_acc(f->esp + 20);
            check_user_mem_acc(f->esp + 24);
            check_user_mem_acc(f->esp + 28);
            f->eax = write((int)*(uint32_t *)(f->esp+20), (void *)*(uint32_t *)(f->esp + 24), (unsigned)*((uint32_t *)(f->esp + 28)));
            break;
            
        case SYS_FIBONACCI :
            check_user_mem_acc(f->esp + 4);
            f->eax = fibonacci((int)*(uint32_t *)(f->esp + 4));
            break;
            
        case SYS_MAX_OF_FOUR_INT :
            check_user_mem_acc(f->esp + 24);
            check_user_mem_acc(f->esp + 28);
            check_user_mem_acc(f->esp + 32);
            check_user_mem_acc(f->esp + 36);
            f->eax = max_of_four_int((int)*(uint32_t *)(f->esp + 24), (int)*(uint32_t *)(f->esp + 28), (int)*(uint32_t *)(f->esp + 32), (int)*(uint32_t *)(f->esp + 36));
            break;
    
        case SYS_CREATE:
          break;
        case SYS_REMOVE:
          break;
        case SYS_OPEN:
          break;
        case SYS_FILESIZE:
          break;
        case SYS_SEEK:
          break;
        case SYS_TELL:
          break;
        case SYS_CLOSE:
          break;
        default :
            break;
    }
  //thread_exit ();
}

void halt(void){
    shutdown_power_off();
}

void exit(int status){
    //process termination message
    printf("%s: exit(%d)\n", thread_name(), status);
    thread_current() -> exit_status = status;
    thread_exit();
}

tid_t exec( const char *cmd_line){
    return process_execute(cmd_line);
}

int wait(tid_t pid){
    return process_wait(pid);
}

int read(int fd, void *buffer, unsigned size){
    
    if (fd == 0){ //stdin
        for(int i=0; i<size; i++)
            ((char *)buffer)[i]=input_getc();
        return size;
    }
    return -1;
}

int write(int fd, const void *buffer, unsigned size){
    
    if(fd ==1 ){
        putbuf(buffer, size);
        return size;
    }
    return -1;
}

//Additional System Call
int fibonacci(int n){
    
    int n1 = 1, n2 = 1;
    int i, temp;

    if( n ==1 || n == 2 ){
      return 1;
    }

    for( i = 3; i <= n; i++ ){
      temp = n1;
      n1 = n2;
      n2 = n1 + temp;
    }
    return n2;
}

int max_of_four_int(int a, int b, int c, int d){
    int max=a;
    
    if(max<b) max=b;
    if(max<c) max=c;
    if(max<d) max=d;
    
    return max;
}

// Check User Memory Access
void check_user_mem_acc(void *addr){
    struct thread *c_thread = thread_current();
    
    //check if NULL pointer
    if(addr == NULL)  exit(-1);
     
    //check if user virtual address
    if(!is_user_vaddr(addr)) exit(-1);
    
    //check if unmapped
    if(!pagedir_get_page(c_thread->pagedir, addr)) exit(-1);
}
