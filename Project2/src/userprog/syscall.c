

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

// included to use funtions for Project2
#include "filesys/filesys.h" //file system syscalls
#include "threads/synch.h"

struct file
{
  struct inode *inode;        /* File's inode. */
  off_t pos;                  /* Current position. */
  bool deny_write;            /* Has file_deny_write() been called? */
};

static void syscall_handler (struct intr_frame *);

struct lock file_lock;

void
syscall_init (void) 
{
    lock_init(&file_lock);
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
    //-----------------------------------
    // system call, system call handler implemented by HJL
    // Sogang OS Pintos Project 2 2020 Fall
        case SYS_CREATE:
          check_user_mem_acc(f->esp + 16);
          check_user_mem_acc(f->esp + 20);
          f-> eax = create((const char *)*(uint32_t *)(f->esp + 16), (unsigned)*(uint32_t *)(f->esp + 20));
          break;
        case SYS_REMOVE:
          check_user_mem_acc(f->esp + 4);
          f-> eax = remove((const char*)*(uint32_t *)(f->esp + 4));
          break;
        case SYS_OPEN:
          check_user_mem_acc(f->esp + 4);
          f-> eax = open((const char*)*(uint32_t *)(f->esp + 4));
          break;
        case SYS_CLOSE:
          check_user_mem_acc(f->esp + 4);
          close((int)*(uint32_t *)(f->esp + 4));
          break;
        case SYS_FILESIZE:
          check_user_mem_acc(f->esp + 4);
          f-> eax = filesize((int)*(uint32_t *)(f->esp + 4));
          break;
        case SYS_SEEK:
          check_user_mem_acc(f->esp + 16);
          check_user_mem_acc(f->esp + 20);
          seek((int)*(uint32_t *)(f->esp + 16), (unsigned)*(uint32_t *)(f->esp + 20));
          break;
        case SYS_TELL:
          check_user_mem_acc(f->esp + 4);
          f-> eax = tell((int)*(uint32_t *)(f->esp + 4));
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
    // Sogang OS Pintos Project 2 2020 Fall
    for (int i = 3; i < 128; i++)
        if (thread_current()->fd[i] != NULL) close(i);
    //-------------------------------------
    thread_exit();
}

tid_t exec( const char *cmd_line){
    return process_execute(cmd_line);
}

int wait(tid_t pid){
    return process_wait(pid);
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

//-----------------------------------
// system call, system call handler implemented by HJL
// Sogang OS Pintos Project 2 2020 Fall

bool create(const char *file, unsigned initial_size){
    check_user_mem_acc(file);
    return filesys_create(file, initial_size);
}

bool remove(const char *file){
    check_user_mem_acc(file);
    return filesys_remove(file);
}

int open(const char *file){
    int i, ans;
    struct file* f;
    
    check_user_mem_acc(file);
    
    //critical section
    lock_acquire(&file_lock);
    f = filesys_open(file);
    
    if (f == NULL) ans = -1;
    else{
        for(i = 3; i < 128; i++)
            if (thread_current()->fd[i] == NULL) {
                if (!strcmp(thread_current()->name, file))
                    file_deny_write(f); // executable files
                thread_current()->fd[i] = f;
                ans = i;
                break;
        }
    }
    lock_release(&file_lock);
    //critical section
    
    return ans;
}

void close(int fd){
    struct file* f = thread_current()->fd[fd];
    if (f == NULL) exit(-1);
    
    thread_current()->fd[fd] = NULL;
    return file_close(f);
}

int filesize(int fd){
    struct file* f = thread_current()->fd[fd];
    if (f == NULL) exit(-1);
    
    return file_length(f);
}

int read(int fd, void *buffer, unsigned size){
    int ans, i;
    struct file* f;
    
    check_user_mem_acc(buffer);
    if (fd == 0){ //stdin
        
        //critical section
        lock_acquire(&file_lock);
        for(i=0; i<size; i++)
            if(input_getc() == '\0') break;
        lock_release(&file_lock);
        //critical section
        
        return i;
    }
    else if (fd >= 3){
        f = thread_current()->fd[fd];
        if (f == NULL) exit(-1);
        
        //critical section
        lock_acquire(&file_lock);
        ans = file_read(f, buffer, size);
        lock_release(&file_lock);
        //critical section
        
        return ans;
    }
    return -1;
}

int write(int fd, const void *buffer, unsigned size){
    int ans;
    struct file* f;
    
    check_user_mem_acc(buffer);
    if(fd ==1 ){ //stdout
        
        //critical section
        lock_acquire(&file_lock);
        putbuf(buffer, size);
        lock_release(&file_lock);
        //critical section
        
        return size;
    }
    else if (fd >= 3){
        f = thread_current()->fd[fd];
        if (f == NULL) exit(-1);
        
        //critical section
        lock_acquire(&file_lock);
        ans = file_write(f, buffer, size);
        lock_release(&file_lock);
        //critical section
        
        return ans;
    }
    return -1;
}

void seek(int fd, unsigned position){
    struct file* f = thread_current()->fd[fd];
    if (f == NULL) exit(-1);
    
    file_seek(f, position);
}

unsigned tell(int fd){
    struct file* f = thread_current()->fd[fd];
    if (f == NULL) exit(-1);
    
    return file_tell(f);
}

