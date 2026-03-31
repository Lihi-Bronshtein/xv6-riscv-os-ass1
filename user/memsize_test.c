#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    printf("%d\n", memsize());
    int *ptr = malloc(20000);
    printf("%d\n", memsize());
    free(ptr);
    printf("%d\n", memsize());
    exit(0);
}