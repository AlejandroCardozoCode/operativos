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

int main(int argc, char *argv[])
{
    char nombrepipe[20];
    mode_t fifo_mode = S_IRUSR | S_IWUSR;
    int fd
        , x,
        z,
        ff;
    for (int i = 0; i < 3; i++)
    {
        sprintf(nombrepipe, "pipe_%d", i);
        ff = mknod(nombrepipe, S_IFIFO | S_IRUSR | S_IWUSR ,0);
        if (ff == -1)
        {
           perror("error en el mknod\n");
           exit(0);
        }
        
    }

    for (int i = 0; i < 3; i++)
    {
        x = i;
        sprintf(nombrepipe, "pipe_%d", i);
        fd = open(nombrepipe, O_WRONLY);
        printf("el valor de x es igula a: %d\n",x);
        write(fd, &x, sizeof(int));
        close(fd);
    }
    for (int i = 0; i < 3; i++)
    {
        sprintf(nombrepipe, "pipe_%d", i);
        fd = open(nombrepipe, O_RDONLY);
        read(fd, &z, sizeof(int));
        printf("el valor de x es igula a: %d\n",z);
        close(fd);
    }
    
}
