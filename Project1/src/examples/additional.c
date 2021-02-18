#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>

int
main (int argc, char **argv)
{
  int a, b, c, d, fibo=0, max=0;

    a = atoi(argv[1]);
    b = atoi(argv[2]);
    c = atoi(argv[3]);
    d = atoi(argv[4]);
    
    fibo = fibonacci(a);
    max = max_of_four_int(a, b, c, d);
    
    printf("%d %d\n", fibo, max);
    
    return EXIT_SUCCESS;

}
