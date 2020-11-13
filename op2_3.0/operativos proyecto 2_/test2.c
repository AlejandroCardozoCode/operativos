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
#include <signal.h>
#include <wait.h>


typedef struct arreglopipe
{
    int i;
    int d;
    int g;
} arreglo;

void signalHandler(int x)
{

  printf("El proceso central recibio la señal SIGUSR1\n");
  //free(&x);
  exit(0);
}

void hijo(int x)
{
    signal(SIGUSR1,signalHandler);
    printf("soy el hijo\n");
    int j = x;
    pause();
    signalHandler(j);
}

int main(int argc, char *argv[])
{
    int id, status;
    for (int i = 0; i < 3; i++)
    {
        id = fork();
        if (id == 0)
        {
            hijo(i);
        }
        else
        {
            wait(NULL); //el programa espera a que los procesos acaben la asignacion de los datos
        }
    }
    int senal_1  = kill(id, SIGUSR1);
    for (int i = 0; i < 2; i++)
     {
        wait(&status);
        exit(0);
    }
    
}
