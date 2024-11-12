#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int pipe_left) __attribute__((noreturn));

int main(int argc, char *argv[])
{
    int _pipe[2]; // inital pipe to store from 2 to 280
    pipe(_pipe);
    if (fork() == 0)
    {
        close(_pipe[1]);
        primes(_pipe[0]);
    }
    else
    {
        close(_pipe[0]); // close read end because we are writing to it
        for (int i = 2; i <= 280; i++)
        {
            write(_pipe[1], &i, sizeof(i));
        }
        close(_pipe[1]);
    }

    wait(0);
    exit(0);
}

void primes(int pipe_left)
{
    int prime = 0;
    if (read(pipe_left, &prime, 4) == 0)
    {
        close(pipe_left);
        exit(0);
    }
    printf("prime %d\n", prime);
    int num = 0;

    int _pipe[2];
    pipe(_pipe);
    if (fork() == 0)
    {
        close(_pipe[1]);
        close(pipe_left);
        primes(_pipe[0]);
    }
    else
    {
        close(_pipe[0]);
        while (read(pipe_left, &num, 4) != 0)
        {
            if (num % prime != 0)
                write(_pipe[1], &num, 4);
        }
        close(pipe_left);
        close(_pipe[1]);
        wait(0);
    }
    exit(0);
}
