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
        for (int i = 0; i < 5; i++)
        { // הגבלנו ל-5 פעמים כדי שהטסט יסתיים
            int value = co_yield(pid1, 1);
            printf("Child received: %d\n", value);
        }
    }
    else
    { // Parent
        for (int i = 0; i < 5; i++)
        {
            int value = co_yield(pid2, 2);
            printf("parent received: %d\n", value);
        }
    }

    exit(0);
}