#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int pid1 = getpid(); // Parent PID
    int pid2 = fork();   // Child PID

    if (pid2 < 0)
    {
        printf("fork failed\n");
        exit(1);
    }

    if (pid2 == 0)
    { // Child
        for (;;)
        {
            int value = co_yield(pid1, 1);
            printf("Child received: %d\n", value);
        }
    }
    else
    { // Parent
        for (;;)
        {
            int value = co_yield(pid2, 2);
            printf("parent received: %d\n", value);
        }
    }

    exit(0);
}