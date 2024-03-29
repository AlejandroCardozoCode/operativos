
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include <fcntl.h>
#include <wait.h>
#include "auxanalogp.h"
#include <errno.h>

/* funcion que se implemento de una libreria, esta funcion se encarga de poder calcular el tiempo de ejecucion del programa*/
#define timersub(a, b, result)                           \
    do                                                   \
    {                                                    \
        (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;    \
        (result)->tv_usec = (a)->tv_usec - (b)->tv_usec; \
        if ((result)->tv_usec < 0)                       \
        {                                                \
            --(result)->tv_sec;                          \
            (result)->tv_usec += 1000000;                \
        }                                                \
    } while (0)

int verificarInt(char segmento[])   /* funcion que se encarga de saber si un arreglo de caracteres tiene solo numero enteros */
{
    int siEs = 0;
    int tamano = strlen(segmento),
        i;
    for (i = 0; i < tamano; i++)
    {
        if (!isdigit(segmento[i]))
        {
            siEs = 1;
        }
    }
    return siEs;
}

int impresion_menu()    /* funcion que imprime el primer menu del programa */
{
    char c[250];
    printf("\nMenu del programa");
    printf("\n1) Realizar una consulta");
    printf("\n2) Salir del Sistema");
    printf("\n");
    scanf("%s", c);
    int verificador = verificarInt(c);
    if (verificador == 1)
    {
        printf("ERROR: El dato ingresado no es un numero entero\n");
        return -1;
    }
    if (atoi(c) == 1)
    {
        return atoi(c);
    }
    else if (atoi(c) == 2)
    {
        return atoi(c);
    }
    else
    {
        return -1;
    }
}
bool isInteger(char *str)   /*esta funcion fue tomada de internet para poder saber si un char es un dato numerico, se m¿implementa en la funcion de interpretarConsulta https://www.geeksforgeeks.org/c-program-detect-tokens-c-program/*/
{
    int i,
        len = strlen(str);

    if (len == 0)
        return (false);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' &&
                str[i] != '1' &&
                str[i] != '2' &&
                str[i] != '3' &&
                str[i] != '4' &&
                str[i] != '5' &&
                str[i] != '6' &&
                str[i] != '7' &&
                str[i] != '8' &&
                str[i] != '9' ||
            (str[i] == '-' && i > 0))
            return (false);
    }
    return (true);
}
int interpretarConsulta(struct Consulta *consulta)  /* // funcion que guarda la consulta ingresada en una estructura */
{

    char ff[100];
    char *token;
    printf("\nMenu de consulta");
    printf("\nIngrese la consulta de la forma: (numero columna, signo, valor)");
    printf("\n");
    scanf("%s", ff);
    int comas = 0,
        i;
    for (i = 0; i < 100; i++)
    {
        if (ff[i] == ',')
        {
            comas++;
        }
    }
    if ((int)strlen(ff) <= 4)
    {
        printf("ERROR: numero de valores invalido\n");
        return -1;
    }
    token = strtok(ff, ",");
    if (atoi(token) <= 0)
    {
        printf("ERROR: valor de la columna invalido\n");
        return -1;
    }

    consulta->columna = atoi(token);
    token = strtok(NULL, ",");
    if (strcmp(token, ">") == 0)
        consulta->signo = 0;
    else if (strcmp(token, "<") == 0)
        consulta->signo = 1;
    else if (strcmp(token, "<=") == 0)
        consulta->signo = 3;
    else if (strcmp(token, ">=") == 0)
        consulta->signo = 2;
    else if (strcmp(token, "==") == 0)
        consulta->signo = 4;
    else
    {
        printf("ERROR: valor del signo es invalido\n");
        return -1;
    }

    token = strtok(NULL, ",");
    if (isInteger(token) == false)
    {
        printf("ERROR: el tercer dato de la consulta no es valido\n");
        return -1;
    }
    consulta->valor = atoi(token);
    return 0;
}
double encontrarCambianteLM(struct Parametros *parametros) /* funcion que encuantra en que posicion se le necesita agregar una linea mas a los mappers del archivo de registros */
{
    int maps = parametros->nmappers;
    int line = parametros->lineas;
    double lineasArchivos = ((double)line) / ((double)maps);
    int parteEntera = ((int)lineasArchivos);
    double sobras = (lineasArchivos - (double)parteEntera) * maps;

    return sobras;
}

void calcularValorFinal(struct Parametros *parametros)  /* funcion que calcula el valor final requerido por la consulta */
{
    int i,
        valor = 0;
    for (i = 0; i < parametros->nreducers; i++)
    {
        pReducer z;
        int fd;
        char nombreReducer[20];
        sprintf(nombreReducer, "pipeR_%d", i);
        fd = open(nombreReducer, O_RDONLY);
        if (fd == -1)
        {
            perror("no se pudo abrir el reduceer en modo de lectura");
        }
        if (read(fd, &z, sizeof(int)) == -1)
        {
            perror("no se puedo leer el reducer");
        }
        valor = valor + z.valor;
    }
    printf("el valor final es: %d\n", valor);
    return;
}

int asignacionPipeMapper(struct Parametros *parametros, struct Consulta *consulta)  /* funcion que se encarga de poder asignar las linas correspondientes a cada pipeM */
{
    FILE *registros;
    int maps = parametros->nmappers;
    int line = parametros->lineas;
    double lineasArchivos = ((double)line) / ((double)maps);
    double sobras = encontrarCambianteLM(parametros);
    sobras = sobras + 0.55;
    int a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r,
        fd,
        parteEntera = ((int)lineasArchivos),
        auxSobrantes = 0,
        x,
        z;
    int Ccolumna = consulta->columna,
        Cvalor = consulta->valor,
        Csigno = consulta->signo;

    char nombrePipe[15], lineaArchivo[1000];
    pMapper infoEnviar[parteEntera],
        infoEnviar2[parteEntera + 1];

    registros = fopen(parametros->logfile, "r");
    if ((int)sobras == 0)
    {
        for (x = 0; x < maps; x++)
        {
            sprintf(nombrePipe, "pipeM_%d", x);

            fd = open(nombrePipe, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe");
                return -1;
            }
            for (z = 0; z < parteEntera; z++)
            {
                fscanf(registros, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r);
                infoEnviar[z].consul.columna = Ccolumna;
                infoEnviar[z].consul.valor = Cvalor;
                infoEnviar[z].consul.signo = Csigno;
                infoEnviar[z].a = a;
                infoEnviar[z].b = b;
                infoEnviar[z].c = c;
                infoEnviar[z].d = d;
                infoEnviar[z].e = e;
                infoEnviar[z].f = f;
                infoEnviar[z].g = g;
                infoEnviar[z].h = h;
                infoEnviar[z].i = i;
                infoEnviar[z].j = j;
                infoEnviar[z].k = k;
                infoEnviar[z].l = l;
                infoEnviar[z].m = m;
                infoEnviar[z].n = n;
                infoEnviar[z].o = o;
                infoEnviar[z].p = p;
                infoEnviar[z].q = q;
                infoEnviar[z].r = r;
            }
            write(fd, &infoEnviar, sizeof(pMapper) * parteEntera);
            close(fd);
        }
        fclose(registros);
    }

    if ((int)sobras != 0)
    {
        printf("\nADVERTENCIA: la divicion entre mapers y las lineas totales no es exacta");
        printf("\nse hara el reparto lo mas equilibrado posible\n");
        for (x = 0; x < maps; x++)
        {
            sprintf(nombrePipe, "pipeM_%d", x);

            fd = open(nombrePipe, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe");
                return -1;
            }
            if (auxSobrantes < (int)sobras)
            {
                auxSobrantes++;
                for (z = 0; z < parteEntera + 1; z++)
                {
                    fscanf(registros, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r);
                    infoEnviar2[z].consul.columna = Ccolumna;
                    infoEnviar2[z].consul.valor = Cvalor;
                    infoEnviar2[z].consul.signo = Csigno;
                    infoEnviar2[z].a = a;
                    infoEnviar2[z].b = b;
                    infoEnviar2[z].c = c;
                    infoEnviar2[z].d = d;
                    infoEnviar2[z].e = e;
                    infoEnviar2[z].f = f;
                    infoEnviar2[z].g = g;
                    infoEnviar2[z].h = h;
                    infoEnviar2[z].i = i;
                    infoEnviar2[z].j = j;
                    infoEnviar2[z].k = k;
                    infoEnviar2[z].l = l;
                    infoEnviar2[z].m = m;
                    infoEnviar2[z].n = n;
                    infoEnviar2[z].o = o;
                    infoEnviar2[z].p = p;
                    infoEnviar2[z].q = q;
                    infoEnviar2[z].r = r;
                }
                write(fd, &infoEnviar2, sizeof(pMapper) * (parteEntera + 1));
                close(fd);
            }
            else
            {
                for (z = 0; z < parteEntera; z++)
                {
                    fscanf(registros, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r);
                    infoEnviar[z].consul.columna = Ccolumna;
                    infoEnviar[z].consul.valor = Cvalor;
                    infoEnviar[z].consul.signo = Csigno;
                    infoEnviar[z].a = a;
                    infoEnviar[z].b = b;
                    infoEnviar[z].c = c;
                    infoEnviar[z].d = d;
                    infoEnviar[z].e = e;
                    infoEnviar[z].f = f;
                    infoEnviar[z].g = g;
                    infoEnviar[z].h = h;
                    infoEnviar[z].i = i;
                    infoEnviar[z].j = j;
                    infoEnviar[z].k = k;
                    infoEnviar[z].l = l;
                    infoEnviar[z].m = m;
                    infoEnviar[z].n = n;
                    infoEnviar[z].o = o;
                    infoEnviar[z].p = p;
                    infoEnviar[z].q = q;
                    infoEnviar[z].r = r;
                }
                write(fd, &infoEnviar, sizeof(pMapper) * (parteEntera));
                close(fd);
            }
        }
        fclose(registros);
    }

    return 0;
}
int map(char nombrePipe[], struct Parametros *parametros, int contador) /* funcion map que se encarga de poder leer las lineas que le toco a cada buffer y asi guardar los datos validos */
{
    int fd,
        key = 0,
        valor = 0,
        fdB,
        maps = parametros->nmappers,
        line = parametros->lineas,
        contadorLineasBuffer = 0,
        i,
        v,
        j,
        fx;
    double numCambio = encontrarCambianteLM(parametros),
           lineasArchivos = ((double)line) / ((double)maps);
    lineasArchivos = lineasArchivos + 0.05;
    int parteEntera = ((int)lineasArchivos);
    char nombreBuf[15], lineaPipeActual[1000], *token;
    pMapper infoDelPipe[parteEntera];
    struct Consulta consulta;
    numCambio = numCambio + 0.55;
    FILE *auxinforme;
    char nombreAux[20];
    /*sprintf( nombreAux, "informe_pipeM_%d",contador);*/
    /*auxinforme = fopen (nombreAux,"a");*/
    if (contador < (int)numCambio)
    {
        parteEntera++;
    }
    BufferP bufferActual[parteEntera];
    BufferP informacionEnviar[parteEntera];
    fd = open(nombrePipe, O_RDONLY);
    if (fd == -1)
    {
        perror("Error: no se pudo abrir el pipe");
        return -1;
    }
    read(fd, &infoDelPipe, sizeof(pMapper) * (parteEntera));
    for (i = 0; i < parteEntera; i++)
    {
        bufferActual[i].key = 0;
        bufferActual[i].valor = 0;
    }
    sprintf(nombreBuf, "Buf_%d", contador);
    fdB = open(nombreBuf, O_WRONLY);

    if (fdB == -1)
    {
        perror("Error: no se pudo abrir el pipe del buffer");
        exit(0);
    }
    /*fprintf(auxinforme, "imprimiendo informacion del pipeM_%d\n", contador);*/
    for (v = 0; v < parteEntera; v++)
    {
        consulta.columna = infoDelPipe[v].consul.columna;
        consulta.signo = infoDelPipe[v].consul.signo;
        consulta.valor = infoDelPipe[v].consul.valor;
        sprintf(lineaPipeActual, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", infoDelPipe[v].a, infoDelPipe[v].b, infoDelPipe[v].c,
                infoDelPipe[v].d, infoDelPipe[v].e, infoDelPipe[v].f, infoDelPipe[v].g, infoDelPipe[v].h, infoDelPipe[v].i, infoDelPipe[v].j,
                infoDelPipe[v].k, infoDelPipe[v].l, infoDelPipe[v].m, infoDelPipe[v].n, infoDelPipe[v].o, infoDelPipe[v].p, infoDelPipe[v].q,
                infoDelPipe[v].r);
        /*fprintf(auxinforme, "la consulta de este pipe es %d %d %d y sus valores son %s\n", infoDelPipe[v].consul.columna, infoDelPipe[v].consul.signo, infoDelPipe[v].consul.valor, lineaPipeActual);*/
        token = strtok(lineaPipeActual, " ");
        key = atoi(token);
        for (i = 1; i <= 18; i++)
        {
            switch (consulta.signo)
            {
            case 0:
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor > (consulta.valor))
                    {
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                    }
                }
                break;
            case 1:
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor < (consulta.valor))
                    {
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                    }
                }
                break;
            case 2:
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor >= (consulta.valor))
                    {
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                    }
                }
                break;
            case 3:
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor <= (consulta.valor))
                    {
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                    }
                }
                break;
            case 4:
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor == (consulta.valor))
                    {
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                    }
                }
                break;
            }
            token = strtok(NULL, " ");
        }
    }
    memcpy(informacionEnviar, bufferActual, sizeof(BufferP) * parteEntera);
    if (write(fdB, &informacionEnviar, sizeof(BufferP) * parteEntera) == -1)
    {
        perror("error en la escirtura del buf");
    }

    close(fd);
    close(fdB);
    return 0;
}
double calcularSobrasReduce(int mapers, int reducers)   /* funcion que se encarga de poder calcular cuantos buffer tiene que leer cada reduce */
{

    double sobras = 0;
    double cantidadReducers = (double)mapers / (double)reducers;
    int parteEntera = (int)cantidadReducers;
    sobras = (cantidadReducers - (long double)parteEntera) * (long double)reducers;
    return sobras;
}

long double hallarNumeroOutput(long double reducer, long double pEntera, long double nuevo) /* funcion que se encarga de encontrar el numero de la posicion del buffer que va a leer cada output */
{
    long double cantidad_mover = nuevo - reducer;
    long double inicio = reducer * (pEntera + 1);
    long double posi = inicio + cantidad_mover * pEntera;

    return posi;
}
void reduce(struct Parametros *parametros, int reduceActual) /* funcion reduce que se encarga de poder leer los buffers correspondientes de cada reduce y guarda la cantidad de datos validos en un pipe */
{
    int mapers = parametros->nmappers,
        reducers = parametros->nreducers,
        lineas = 0,
        auxLineas = 0,
        fd,
        fdB,
        key,
        valor,
        corrector,
        j,
        i;
    double sobras = calcularSobrasReduce(mapers, reducers) + 0.55;
    float cantidadDeArchivos = mapers / reducers;
    int parteEntera = (int)cantidadDeArchivos;
    int parteEntera2 = parametros->lineas / mapers;
    char nombreBuf[15],
        nombrePipeOut[15],
        nombreArchivoRegistros[20],
        nombreInforme[20];
    double numCambio = encontrarCambianteLM(parametros);
    numCambio = numCambio + 0.55;
    pReducer pipeR;
    FILE *archivoRegistro;
    FILE *auxinforme;
    if ((int)sobras == 0)
    {
        if (parametros->intermedios == 1)
        {
            sprintf(nombreArchivoRegistros, "intermedio_%d.txt", reduceActual);
            archivoRegistro = fopen(nombreArchivoRegistros, "a");
        }
        sprintf(nombrePipeOut, "pipeR_%d", reduceActual);
        fd = open(nombrePipeOut, O_WRONLY);
        if (fd == -1)
        {
            perror("Error: no se pudo abrir el pipe del reduce");
            exit(0);
        }
        corrector = reduceActual * (parteEntera);
        for (j = 0; j < parteEntera; j++)
        {
            if ((corrector + j) < (int)numCambio)
            {
                parteEntera2++;
            }
            BufferP bufferActual[parteEntera2];
            /*sprintf(nombreInforme, "informe_buf_%d",corrector + j);*/
            /*auxinforme = fopen(nombreInforme, "a");*/
            sprintf(nombreBuf, "Buf_%d", corrector + j);
            fdB = open(nombreBuf, O_RDONLY);
            if (fdB == -1)
            {
                perror("Error: no se pudo abrir el pipe del buffer");
                exit(0);
            }
            read(fdB, &bufferActual, sizeof(BufferP) * parteEntera2);
            corrector = reduceActual * parteEntera;
            for (i = 0; i < parteEntera2; i++)
            {
                /*fprintf(auxinforme, "%d %d\n", bufferActual[i].key, bufferActual[i].valor);*/
                if (bufferActual[i].key != 0)
                {
                    key = bufferActual[i].key;
                    valor = bufferActual[i].valor;
                    if (parametros->intermedios == 1)
                    {
                        fprintf(archivoRegistro, "%d %d\n", key, valor);
                    }
                    lineas++;
                }
            }
        }
        write(fd, &lineas, sizeof(int));
        close(fd);
        close(fdB);
        fclose(archivoRegistro);
        return;
    }
    if ((int)sobras != 0)
    {
        if (reduceActual < (int)sobras)
        {
            if (parametros->intermedios == 1)
            {
                sprintf(nombreArchivoRegistros, "intermedio_%d.txt", reduceActual);
                archivoRegistro = fopen(nombreArchivoRegistros, "a");
            }
            sprintf(nombrePipeOut, "pipeR_%d", reduceActual);
            fd = open(nombrePipeOut, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe del reduce");
                exit(0);
            }
            corrector = reduceActual * (parteEntera + 1);
            for (j = 0; j < parteEntera + 1; j++)
            {
                if ((corrector + j) < (int)numCambio)
                {

                    parteEntera2++;
                }
                BufferP bufferActual[parteEntera2];
                /*sprintf(nombreInforme, "informe_buf_%d",corrector + j);*/
                /*auxinforme = fopen(nombreInforme, "a");*/
                sprintf(nombreBuf, "Buf_%d", corrector + j);
                fdB = open(nombreBuf, O_RDONLY);
                if (fdB == -1)
                {
                    perror("Error: no se pudo abrir el pipe del buffer");
                    exit(0);
                }
                read(fdB, &bufferActual, sizeof(BufferP) * (parteEntera2));
                for (i = 0; i < parteEntera2; i++)
                {
                    /*fprintf(auxinforme, "%d %d\n", bufferActual[i].key, bufferActual[i].valor);*/
                    if (bufferActual[i].key != 0)
                    {
                        key = bufferActual[i].key;
                        valor = bufferActual[i].valor;
                        if (parametros->intermedios == 1)
                        {
                            fprintf(archivoRegistro, "%d %d\n", key, valor);
                        }
                        lineas++;
                    }
                }
                if ((corrector + j) < (int)numCambio)
                {
                    parteEntera2--;
                }
            }
            write(fd, &lineas, sizeof(int));
            close(fd);
            close(fdB);
            fclose(archivoRegistro);
            sobras++;
        }
        else

        {
            if (parametros->intermedios == 1)
            {
                sprintf(nombreArchivoRegistros, "intermedio_%d.txt", reduceActual);
                archivoRegistro = fopen(nombreArchivoRegistros, "a");
            }
            sprintf(nombrePipeOut, "pipeR_%d", reduceActual);
            fd = open(nombrePipeOut, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe del reduce");
                exit(0);
            }
            for (j = 0; j < parteEntera; j++)
            {
                long double po = j + hallarNumeroOutput(((((long double)mapers / (long double)reducers) - (long double)parteEntera) * (long double)reducers), parteEntera, reduceActual);
                po = po + 0.55;
                if ((int)po < (int)numCambio)
                {
                    parteEntera2++;
                }
                BufferP bufferActual[parteEntera2];
                /*sprintf(nombreInforme, "informe_buf_%d",(int)po);*/
                /*auxinforme = fopen(nombreInforme, "a");*/
                sprintf(nombreBuf, "Buf_%d", (int)po);
                fdB = open(nombreBuf, O_RDONLY);
                if (fdB == -1)
                {
                    perror("Error: no se pudo abrir el pipe del buffer");
                    exit(0);
                }
                read(fdB, &bufferActual, sizeof(BufferP) * parteEntera2);
                for (i = 0; i < parteEntera2; i++)
                {
                    /*fprintf(auxinforme, "%d %d\n", bufferActual[i].key, bufferActual[i].valor);*/
                    if (bufferActual[i].key != 0)
                    {
                        key = bufferActual[i].key;
                        valor = bufferActual[i].valor;
                        if (parametros->intermedios == 1)
                        {
                            fprintf(archivoRegistro, "%d %d\n", key, valor);
                        }
                        lineas++;
                    }
                }
                if ((int)po < (int)numCambio)
                {
                    parteEntera2--;
                }
            }
            write(fd, &lineas, sizeof(int));
            close(fd);
            close(fdB);
            fclose(archivoRegistro);
        }
    }
    return;
}
void borrarArchivos(struct Parametros *parametros) /* funcion que se encarga del borrado de datos despues de que el usuario ingresa el numero 2 en el menu principal */
{
    int contador = 0,
        i;
    char nombreArch[20];
    for (i = 0; i < parametros->nmappers; i++)
    {
        sprintf(nombreArch, "pipeM_%d", i);
        remove(nombreArch);
    }
    for (i = 0; i < parametros->nmappers; i++)
    {
        sprintf(nombreArch, "Buf_%d", i);
        remove(nombreArch);
    }
    for (i = 0; i < parametros->nreducers; i++)
    {
        sprintf(nombreArch, "pipeR_%d", i);
        remove(nombreArch);
    }
    if (parametros->intermedios == 1)
    {
        for (i = 0; i < parametros->nreducers; i++)
        {
            sprintf(nombreArch, "intermedio_%d.txt", i);
            remove(nombreArch);
        }
    }
    return;
}

int master(struct Parametros *parametros, struct Consulta *consulta) /* funcion master la cual se encarga de la ejecucion de todas las demas funciones y se encarga de la creacion de los procesos */
{
    int validacionConsulta = interpretarConsulta(consulta),
        validacionAsigPipe1,
        valorFinal,
        reducers = parametros->nreducers,
        maps = parametros->nmappers,
        id,
        idR,
        idC,
        i,
        parteEntera = (int)(parametros->lineas / parametros->nmappers),
        status;
    struct timeval tiempo_i, tiempo_f, tiempo_tot;
    char nombrePipe[15];
    if (validacionConsulta == -1)
    {
        printf("ERROR: ocurrio un error inesperado en la lectura de la consulta\n");
        return -1;
    }
    gettimeofday(&tiempo_i, NULL);
    for (i = 0; i < 1; i++)
    {
        id = fork();
        if (id == 0)
        {
            validacionAsigPipe1 = asignacionPipeMapper(parametros, consulta);
            if (validacionAsigPipe1 == -1)
            {
                printf("ERROR: ocurrio un error inesperado en la asignacion del pipe 1\n");
                return -1;
            }
            exit(0);
        }
    }
    for (i = 0; i < parametros->nmappers; i++)
    {
        id = fork();
        if (id == 0)
        {
            sprintf(nombrePipe, "pipeM_%d", i);
            map(nombrePipe, parametros, i);
            exit(0);
        }
    }
    for (i = 0; i < parametros->nreducers; i++)
    {
        idR = fork();
        if (idR == 0)
        {
            reduce(parametros, i);
            exit(0);
        }
    }
    gettimeofday(&tiempo_f, NULL);
    timersub(&tiempo_f, &tiempo_i, &tiempo_tot);
    printf("\nResultados\n");
    printf("El tiempo que tomo el programa fue de: %li segundos %li microsegundos\n", tiempo_tot.tv_sec, tiempo_tot.tv_usec);
    for (i = 0; i < 1; i++)
    {
        idC = fork();
        if (idC == 0)
        {
            calcularValorFinal(parametros);
            exit(0);
        }
        else
        {
            waitpid(idC, &status, 0);
        }
    }
    return 0;
}

int main(int argc, char *argv[])    /* main del programa el cual crea los pipes apenas se pone a correr el programa y se encarga de validar las entradas de el usuario por el menu principal */
{
    int condicional = 0,
        opcion_menu,
        resultado,
        numMappers = atoi(argv[3]),
        numReducers = atoi(argv[4]),
        banderaPipe,
        i;

    struct Parametros *parametros = (struct Parametros *)malloc(sizeof(struct Parametros));
    struct Consulta consulta;
    char nombrePipe[15],
        nombrepipeBuffer[15];

    if (argc < 6)
    {
        printf("Alerta los datos se ingresaron de manera erronea\n");
        exit(0);
    }

    strncpy(parametros->logfile, argv[1], 4096);
    parametros->lineas = atoi(argv[2]);
    parametros->nmappers = atoi(argv[3]);
    parametros->nreducers = atoi(argv[4]);
    parametros->intermedios = atoi(argv[5]);
    for (i = 0; i < numMappers; i++)
    {

        sprintf(nombrePipe, "pipeM_%d", i);
        banderaPipe = mknod(nombrePipe, 0010000 | 0400 | 0200, 0);
        if (banderaPipe == -1)
        {
            if (errno = !EEXIST)
            {
                perror("Creacion del pipe mapper:");
                exit(1);
            }
        }
        sprintf(nombrepipeBuffer, "Buf_%d", i);
        banderaPipe = mknod(nombrepipeBuffer, 0010000 | 0400 | 0200, 0);
        if (banderaPipe == -1)
        {
            if (errno = !EEXIST)
            {
                perror("Creacion del pipe buffer:");
                exit(1);
            }
        }
    }
    for (i = 0; i < numReducers; i++)
    {
        sprintf(nombrePipe, "pipeR_%d", i);
        banderaPipe = mknod(nombrePipe, 0010000 | 0400 | 0200, 0);
        if (banderaPipe == -1)
        {
            if (errno = !EEXIST)
            {
                perror("Creacion del pipe reducer:");
                exit(1);
            }
        }
    }
    if (parametros->nmappers < parametros->nreducers)
    {
        printf("ERROR: los mappers son menores a los reducers\n");
        exit(0);
    }
    while (condicional == 0)
    {
        opcion_menu = impresion_menu();
        if (opcion_menu == 1)
        {
            resultado = master(parametros, &consulta);
        }
        else if (opcion_menu == 2)
        {
            borrarArchivos(parametros);
            free(parametros);
            condicional = 1;
        }
        else if (opcion_menu == -1)
        {
            printf("ERROR: vuelva a intentar con un valor valido (1,2) por favor\n");
        }
        else
        {
            printf("se ingreso un valor no valido, vuelva a intentarlo\n");
            
        }
    }
    exit(0);
}