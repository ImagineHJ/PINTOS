#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H
#include <stdbool.h>
typedef int pid_t;

void syscall_init (void);
//static void syscall_handler (struct intr_frame *);

//-----------------------------------
// system call, system call handler implemented by HJL
// Sogang OS Pintos Project 1 2020 Fall

void halt(void);
void exit(int status);
pid_t exec( const char *cmd_line);
int wait(pid_t pid);
int read(int fd, void *buffer, unsigned size);
int write(int fd, const void *buffer, unsigned size);
int fibonacci(int n);
int max_of_four_int(int a, int b, int c, int d);

// Sogang OS Pintos Project 2 2020 Fall
bool create(const char *file, unsigned initial_size);
bool remove(const char *file);
int open(const char *file);
void close(int fd);
int filesize(int fd);
void seek(int fd, unsigned position);
unsigned tell(int fd);

//implemented for checking user memory access
void check_user_mem_acc(void *addr);

#endif /* userprog/syscall.h */
