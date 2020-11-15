#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

typedef struct arreglopipe
{
    int i;
    int d;
    int g;
} arreglo;

void escribirPipe(int i)
{
    int x,
        fd;
    x = i * i;
    char nombrepipe[20];
    sprintf(nombrepipe, "pipe_%d", i);
    fd = open(nombrepipe, O_WRONLY);
    if (fd == -1)
    {
        perror("pipe");
        exit(0);
    }
    if (write(fd, &x, sizeof(int)) == -1)
    {
        perror("pipe 3");
        exit(0);
    }
    close(fd);
    return;
}

void leerPipe(int i)
{
    int fd,
        z;
    char nombrepipe[20];
    sprintf(nombrepipe, "pipe_%d", i);
    fd = open(nombrepipe, O_RDONLY);
    if (fd == -1)
    {
        perror("pipe");
        exit(0);
    }
    if (read(fd, &z, sizeof(int)) == -1)
    {
        perror("pipe 3");
        exit(0);
    }
    printf("el valor de x es igula a: %d\n", z);
    close(fd);
}

int main(int argc, char *argv[])
{

    int fd,
        x,
        z,
        id;
    char nombrepipe[20];
    for (int i = 0; i < 3; i++)
    {
        sprintf(nombrepipe, "pipe_%d", i);
        mknod(nombrepipe, S_IFIFO | S_IRUSR | S_IWUSR, 0);
    }
    for (int i = 0; i < 3; i++)
    {
        id = fork();
        if (id == 0)
        {
            escribirPipe(i);
            exit(0);
        }
    }
    for (int i = 0; i < 3; i++)
    {
        id = fork();
        if (id == 0)
        {
            leerpipe(i);
            exit(0);
        }
    }
}
