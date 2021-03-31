# PINTOS Project
I've been working on the Pintos Project from September 2020 to December 2020.
The purpose of this project is to get a better understanding of computer operating system and experience the implementation of it.
The project is divided into 4 parts which are data structure, user program, file system call, and threads.
C language is used in this project and I have referenced the Pintos Manual book from Stanford Univ.
Each project has a document file that shows what I have done in each project(written in Korean).

Please refer to my personal blog to find out more information about the project.
https://tvimagine.video.blog

  ## Project0
  The aim of this project is to practice and understand the data sturctures of PintOS. This project covers the use of list, hash table, and bitmap. The project is mostly about writing an interactive program that can check the funtionalities of list, hash table and bitmap in Pintos kernel.
  
  
  
 I have built an interactive program where the user command is parsed and the appropriate fuction is called according to the command. The program then produces the right output and shows it to the user.

  ## Project1
  
  The aim of this project is to make the PintOS be able to execute user programs properly. In the pre-project PintOS, system call, system call handler, argument passing, and user stack have not been implemented. Therefore, PintOS is not able to execute the user program due to lack of implementation.
  
  
  
  I needed to understand how the user program works in code level flow and learn about the concept of virtual memory, and system call used in OS. I have implemented argument passing where I parse the user command and allocate it to memory according to 80x86 calling convention. I have implemented valid user memory access function to verify the validity of user-provided pointer and prevent user program to access invalid memory. I have implemented system call handler that handles system call by accessing the user stack. I have implemented exec(), exit(), write(), read() system call and additionally fibonacci()and max_of_four_int()
  

  ## Project2
  
  The aim of this project is to implement system calls about the file system. 
  
  
  
  I needed to understand the base file system of PintOS and its features. Also, I had to learn how file system call works in PintOS at the code level. The concept of file descriptor was significantly used in the project. I implemented create(), remove(), open(), close(), filesize(), read(), write(), seek(), tell() file system calls. I have used semaphore synchronization method to ensure that when one process is executing in its critical section no other proces can execute in its critical section.
  
  
  
  ## Project3
