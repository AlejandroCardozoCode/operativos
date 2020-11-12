#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "main.h"

typedef struct arreglopipe
{
    int i;
    int d;
    int g;
} arreglo;



int main(int argc, char *argv[])
{
    int i = 0;
    int fd;
    arreglo pipestr[3];
    arreglo ff[9];
    arreglo  infoEnviar [3];

    char nombrePipe[100];

    //fumcion de creacion de pipes 

    for (i = 0; i < 3; i++)
    {
        sprintf(nombrePipe, "pipe_%d", i);
        fd = open(nombrePipe, O_CREAT, S_IRUSR | S_IWUSR);
        if (fd == -1)
        {
            perror("pipe");
            exit(1);
        }
        close(fd);
    }

    i = 0;
    for (i = 0; i < 9; i++)
    {
        ff[i].i = i;
        ff[i].d = i * 2;
        ff[i].g = i * 3;
    }

    i = 0;

    int j = 0;
    int auxcontador = 0;
    

    //funcion para crear n pipes y meter detro de cada uno un arraay de una estructura

    for (i = 0; i < 3; i++) // este tres es por el numero de pipes 
    {
        sprintf(nombrePipe, "pipe_%d", i);
        fd = open(nombrePipe, O_WRONLY);
        if (fd == -1)
        {
            perror("pipe");
            exit(1);
        }

        for (j = 0; j < 3; j++) // este tres es por el numero del tamaño del arreglo 
        {
            infoEnviar[j].i = ff[auxcontador].i;
            infoEnviar[j].d = ff[auxcontador].d;
            infoEnviar[j].g = ff[auxcontador].g;
            auxcontador++;
        }
        memcpy(pipestr, infoEnviar, sizeof(arreglo)*3); // este tres es por el tamaño de arreglo dentro de la estructura
        write(fd, &pipestr, sizeof(pipestr));
        close(fd);
    }

    i = 0;
    int banderaLectura;

    arreglo infoRecibida[3];


    //funcion para leer n pipes e imprimir la informacion que tiene cada uno 

    for (i = 0; i < 3; i++) //este tres es por el numero de pipes
    {
        sprintf(nombrePipe, "pipe_%d", i);
        fd = open(nombrePipe, O_RDONLY);
        if (fd == -1)
        {
            perror("pipe");
            exit(1);
        }
        banderaLectura = read(fd, &infoRecibida, sizeof(arreglo)*3);
        if (banderaLectura == -1)
        {
            perror("proceso lector:");
            exit(1);
        }
        printf("la informacion del pipe %d es la siguiente\n", i);
        j = 0;
        for ( j = 0; j < 3; j++) //este tres es por el numero del tamaño de arreglo
        {
            printf("el valor de el primer dato es: %d el valor del segundo dato es: %d y el valor del tercer dato es: %d \n", infoRecibida[j].i, infoRecibida[j].d, infoRecibida[j].g);

        }

        close(fd);
    }
}
