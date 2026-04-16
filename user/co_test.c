#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    printf("\n--- Starting co_yield Tests ---\n\n");

    printf("[*] Testing Error Conditions:\n");

    // 1. Attempt to yield to an invalid PID (negative or zero)
    if (co_yield(-5, 1) == -1 && co_yield(0, 1) == -1)
    {
        printf("  [PASSED] Invalid PID (Negative/Zero)\n");
    }
    else
    {
        printf("  [FAILED] Invalid PID (Negative/Zero)\n");
    }

    // 2. Attempt to yield to a non-existent PID
    if (co_yield(9999, 1) == -1)
    {
        printf("  [PASSED] Non-existent PID\n");
    }
    else
    {
        printf("  [FAILED] Non-existent PID\n");
    }

    // 3. Attempt to yield to oneself (Self-yield)
    if (co_yield(getpid(), 1) == -1)
    {
        printf("  [PASSED] Self-yield\n");
    }
    else
    {
        printf("  [FAILED] Self-yield\n");
    }

    // 4. Attempt to yield to a killed process
    int killed_child = fork();
    if (killed_child < 0)
    {
        printf("fork failed\n");
        exit(1);
    }

    if (killed_child == 0)
    {
        // The child process simply sleeps so the parent has time to kill it
        sleep(100);
        exit(0);
    }
    else
    {
        // Parent kills the child
        kill(killed_child);

        // Give the kernel a moment to update the process state to KILLED
        sleep(1);

        // Try to yield to the process we just killed
        if (co_yield(killed_child, 1) == -1)
        {
            printf("  [PASSED] Yield to killed process\n");
        }
        else
        {
            printf("  [FAILED] Yield to killed process\n");
        }

        // Clean up the child's zombie process
        wait(0);
    }

    printf("\n[*] Testing Happy Path:\n");

    int pid1 = getpid(); // Parent PID
    int pid2 = fork();   // Child PID

    if (pid2 < 0)
    {
        printf("fork failed\n");
        exit(1);
    }

    if (pid2 == 0)
    {

        for (int i = 0; i < 5; i++)
        {
            int value = co_yield(pid1, 1);
            printf("Child received: %d\n", value);
        }
        exit(0);
    }
    else
    {

        for (int i = 0; i < 5; i++)
        {
            int value = co_yield(pid2, 2);
            printf("parent received: %d\n", value);
        }
    }

    printf("\n--- Tests Completed ---\n");
    exit(0);
}