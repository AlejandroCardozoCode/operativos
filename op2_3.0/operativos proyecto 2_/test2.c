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
#include <sys/types.h>
#include <sys/wait.h>

typedef struct arreglopipe
{
    int i;
    int d;
    int g;
} arreglo;

void escribirPipe(int i, char nombreAuxContador[])
{
    int x,
        fd,
        valorContador;
    FILE *contador;
    x = i * i;
    char nombrepipe[20];
    sprintf(nombrepipe, "pipe_%d", i);
    for (int i = 0; i < 3; i++)
    {
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
    }
    close(fd);
    contador = fopen(nombreAuxContador, "a");
    fprintf(contador, "%d\n", 1);
    fclose(contador);
    return;
}

void leerPipe(int i, char nombreAuxContador[])
{
    int fd,
        z,
        valorContador;
    char nombrepipe[20];
    FILE *contador;
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
    printf("el valor de x es igual a: %d\n", z);
    close(fd);
    contador = fopen(nombreAuxContador, "a");
    fprintf(contador, "%d\n", 1);
    fclose(contador);
    return;
}

int main(int argc, char *argv[])
{

    int fd,
        x,
        z,
        id,
        contador = 0,
        continuar = 0,
        lines,
        ch;
    char nombrepipe[20],
        nombreaux[20] = "kjkhjkh.txt";
    FILE *ff;
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
            escribirPipe(i, nombreaux);
            exit(0);
        }
    }
    for (int i = 0; i < 3; i++)
    {
        id = fork();
        if (id == 0)
        {
            leerPipe(i, nombreaux);
            exit(0);
        }
    }
    while (continuar == 0)
    {
        ff = fopen(nombreaux, "r");
        while (!feof(ff))
        {
            ch = fgetc(ff);
            if (ch == '\n')
            {
                lines++;
            }
        }
        if (lines == 6)
        {
            continuar = 1;
        }
        lines = 0;
        fclose(ff);
    }
    remove(nombreaux);
}
