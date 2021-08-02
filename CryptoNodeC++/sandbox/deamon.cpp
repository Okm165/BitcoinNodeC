#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

void demonize()
{
    pid_t pid;

    // Fork off the parent process
    pid = fork();

    // An error occurred
    if (pid < 0)
        exit(EXIT_FAILURE);

    // Success: Let the parent terminate
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // On success: The child process becomes session leader
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    // Set file permissions to rw
    umask(0);

    // Close all open file descriptors
    for (int x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        close(x);
}

int main()
{
    demonize();

    // code!!!
    sleep(20);

    return 0;
}