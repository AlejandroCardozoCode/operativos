
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

# define timersub(a, b, result)						      \
  do {									      \
    (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;			      \
    (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;			      \
    if ((result)->tv_usec < 0) {					      \
      --(result)->tv_sec;						      \
      (result)->tv_usec += 1000000;					      \
    }									      \
  } while (0)

int verificarInt(char segmento[])
{
    int siEs = 0;   
    int tamano = strlen(segmento),
        i;
    for ( i = 0; i < tamano; i++) 
    {
        if (!isdigit(segmento[i])) 
        {
            siEs = 1;
        }
    }
    return siEs;
}
int impresion_menu() 
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
bool isInteger(char *str) 
{
    int i, 
        len = strlen(str);

    if (len == 0)
        return (false);
    for ( i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
            return (false);
    }
    return (true);
}
int interpretarConsulta(struct Consulta *consulta) 
{

    char ff[100]; 
    char *token;  
    printf("\nMenu de consulta");
    printf("\nIngrese la consulta de la forma: (numero columna, signo, valor)");
    printf("\n");
    scanf("%s", ff);              
    int comas = 0,
        i;                
    for ( i = 0; i < 100; i++) 
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

double encontrarCambiante(struct Parametros *parametros)
{
    int maps = parametros->nmappers;                               
    int line = parametros->lineas;                                 
    double lineasArchivos = ((double)line) / ((double)maps);       
    int parteEntera = ((int)lineasArchivos);                       
    double sobras = (lineasArchivos - (double)parteEntera) * maps; 

    return sobras;
}

int asignacionPipeMapper(struct Parametros *parametros, struct Consulta *consulta)
{
    FILE *registros;                                                            
    int maps = parametros->nmappers;                                              
    int line = parametros->lineas;  
    double lineasArchivos = ((double)line) / ((double)maps);   
    double sobras = encontrarCambiante(parametros);                         
    int a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, 
        fd, 
        parteEntera = ((int)lineasArchivos), 
        auxSobrantes = 0,
        x,
        z;     
                                                
    char nombrePipe[15], lineaArchivo[1000];
    pMapper infoEnviar[parteEntera],
        infoEnviar2[parteEntera + 1];

    registros = fopen(parametros->logfile, "r"); 
    if ((int)sobras == 0) 
    {
        for ( x = 0; x < maps; x++)
        {
            sprintf(nombrePipe, "pipeM_%d", x);
            fd = open(nombrePipe, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe");
                return -1;
            }
            for ( z = 0; z < parteEntera; z++) 
            {
                fscanf(registros, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r);
                infoEnviar[z].consul.columna = consulta->columna;
                infoEnviar[z].consul.valor = consulta->valor;
                infoEnviar[z].consul.signo = consulta->signo;
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
        for ( x = 0; x < maps; x++)
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
                for ( z = 0; z < parteEntera + 1; z++) 
                {
                    fscanf(registros, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r);
                    infoEnviar2[z].consul.columna = consulta->columna;
                    infoEnviar2[z].consul.valor = consulta->valor;
                    infoEnviar2[z].consul.signo = consulta->signo;
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
                for ( z = 0; z < parteEntera; z++) 
                {
                    fscanf(registros, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d \n", &a, &b, &c, &d, &e, &f, &g, &h, &i, &j, &k, &l, &m, &n, &o, &p, &q, &r);
                    infoEnviar[z].consul.columna = consulta->columna;
                    infoEnviar[z].consul.valor = consulta->valor;
                    infoEnviar[z].consul.signo = consulta->signo;
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
int map(char nombrePipe[], struct Parametros *parametros, int contador)
{
    int fd, 
        key = 0, 
        valor = 0, 
        fdB,
        maps = parametros->nmappers,
        line = parametros->lineas,
        contadorLineasBuffer = 0,
        i,
        v;
    double numCambio = encontrarCambiante(parametros),
        lineasArchivos = ((double)line) / ((double)maps); 
    int parteEntera = ((int)lineasArchivos);                 
    char nombreBuf[15], lineaPipeActual[1000], *token;
    pMapper infoDelPipe[parteEntera];
    struct Consulta consulta;
    BufferP bufferActual[parteEntera];
    BufferP informacionEnviar[parteEntera];

    if (contador < numCambio)
    {
        parteEntera++;
    }

    fd = open(nombrePipe, O_RDONLY);
    if (fd == -1)
    {
        perror("Error: no se pudo abrir el pipe");
        return -1;
    }
    read(fd, &infoDelPipe, sizeof(pMapper) * (parteEntera));
    for ( i = 0; i < parteEntera; i++)
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
    for ( v = 0; v < parteEntera; v++)
    {
        consulta.columna = infoDelPipe[v].consul.columna;
        consulta.signo = infoDelPipe[v].consul.signo;
        consulta.valor = infoDelPipe[v].consul.valor;
        sprintf(lineaPipeActual, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", infoDelPipe[v].a, infoDelPipe[v].b, infoDelPipe[v].c,
                infoDelPipe[v].d, infoDelPipe[v].e, infoDelPipe[v].f, infoDelPipe[v].g, infoDelPipe[v].h, infoDelPipe[v].i, infoDelPipe[v].j,
                infoDelPipe[v].k, infoDelPipe[v].l, infoDelPipe[v].m, infoDelPipe[v].n, infoDelPipe[v].o, infoDelPipe[v].p, infoDelPipe[v].q,
                infoDelPipe[v].r);
        token = strtok(lineaPipeActual, " "); 
        key = atoi(token);                    
        for ( i = 1; i <= 18; i++)         
        {
            switch (consulta.signo) 
            {
            case 0: 
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor > (consulta.valor))
                    {
                        contadorLineasBuffer++;
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
                        contadorLineasBuffer++;
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                        write(fdB, &bufferActual, sizeof(BufferP));
                    }
                }
                break;
            case 2: 
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor >= (consulta.valor))
                    {
                        contadorLineasBuffer++;
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                        write(fdB, &bufferActual, sizeof(BufferP));
                    }
                }
                break;
            case 3: 
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor <= (consulta.valor))
                    {
                        contadorLineasBuffer++;
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                        write(fdB, &bufferActual, sizeof(BufferP));
                    }
                }
                break;
            case 4: 
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor == (consulta.valor))
                    {
                        contadorLineasBuffer++;
                        bufferActual[v].valor = valor;
                        bufferActual[v].key = key;
                        write(fdB, &bufferActual, sizeof(BufferP) * parteEntera);
                    }
                }
                break;
            }
            token = strtok(NULL, " ");
        }
    }
    memcpy(informacionEnviar, bufferActual, sizeof(BufferP) * parteEntera);
    write(fdB, &informacionEnviar, sizeof(BufferP) * parteEntera);
    close(fd);
    close(fdB); 
    return 0;
}
double calcularSobrasReduce(int mapers, int reducers) 
{

    double sobras = 0;                                                         
    double cantidadReducers = (double)mapers / (double)reducers;     
    int parteEntera = (int)cantidadReducers;                                        
    sobras = (cantidadReducers - (long double)parteEntera) * (long double)reducers; 
    return sobras;
}
int contarLineas(char noombreArch[]) 
{
    char ch;                            
    FILE *fp = fopen(noombreArch, "r"); 
    int lineas = 0;
    if (fp == NULL)
    {
        perror("Error: no se pudo abrir el archivo");
        exit(0);
    }
    while (ch != EOF) 
    {
        ch = getc(fp); 
        if (ch == '\n')
        {
            lineas++; 
        }
    }
    fclose(fp); 
    return lineas;
}
long double hallarNumeroOutput(long double reducer, long double pEntera, long double nuevo) 
{
    long double cantidad_mover = nuevo - reducer;
    long double inicio = reducer * (pEntera + 1);
    long double posi = inicio + cantidad_mover * pEntera;
    
    return posi;
}
void reduce(struct Parametros *parametros, int reduceActual) 
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
        nombreArchivoRegistros[20]; 
    double numCambio = encontrarCambiante(parametros);
    pReducer pipeR;
    FILE *archivoRegistro;
    BufferP bufferActual[parteEntera2];
    if ((int)sobras == 0) 
    {
        sprintf(nombreArchivoRegistros,"intermedio_%d.txt", reduceActual);
        archivoRegistro =  fopen(nombreArchivoRegistros, "a");
        sprintf(nombrePipeOut, "pipeR_%d", reduceActual);
        fd = open(nombrePipeOut, O_WRONLY);
        if (fd == -1)
        {
            perror("Error: no se pudo abrir el pipe del reduce");
            exit(0);
        }
        corrector = reduceActual * (parteEntera); 
        for ( j = 0; j < parteEntera; j++)
        {
            BufferP bufferActual[parteEntera2];
            sprintf(nombreBuf, "Buf_%d", corrector + j);
            fdB = open(nombreBuf, O_RDONLY);
            if (fdB == -1)
            {
                perror("Error: no se pudo abrir el pipe del buffer");
                exit(0);
            }
            read(fdB, &bufferActual, sizeof(BufferP) * parteEntera2);
            corrector = reduceActual * parteEntera; 
            for ( i = 0; i < parteEntera2; i++)      
            {
                if (bufferActual[i].key != 0)
                {
                    if (parametros->intermedios == 1)
                    {
                        key = bufferActual[i].key;
                        valor = bufferActual[i].valor;
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
    }
    if ((int)sobras != 0) 
    {
        if (reduceActual < (int)sobras) 
        {
            sprintf(nombreArchivoRegistros,"intermedio_%d.txt", reduceActual);
            archivoRegistro =  fopen(nombreArchivoRegistros, "a");
            sprintf(nombrePipeOut, "pipeR_%d", reduceActual);
            fd = open(nombrePipeOut, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe del reduce");
                exit(0);
            }
            corrector = reduceActual * (parteEntera + 1); 
            for ( j = 0; j <parteEntera + 1; j++)
            {
                BufferP bufferActual[parteEntera2+1];
                sprintf(nombreBuf, "Buf_%d", corrector + j);
                fdB = open(nombreBuf, O_RDONLY);
                if (fdB == -1)
                {
                    perror("Error: no se pudo abrir el pipe del buffer");
                    exit(0);
                }
                read(fdB, &bufferActual, sizeof(BufferP) * (parteEntera2+1));
                for ( i = 0; i < parteEntera2+1; i++) 
                {
                    
                    
                    if (bufferActual[i].key != 0)
                    {
                        if (parametros->intermedios == 1)
                        {
                            key = bufferActual[i].key;
                            valor = bufferActual[i].valor;
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
            sobras++;
        }
        else
    
        {
            sprintf(nombreArchivoRegistros,"intermedio_%d.txt", reduceActual);
            archivoRegistro =  fopen(nombreArchivoRegistros, "a");   
            sprintf(nombrePipeOut, "pipeR_%d", reduceActual);
            fd = open(nombrePipeOut, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe del reduce");
                exit(0);
            }
            for ( j = 0; j < parteEntera; j++)
            {
                long double po = j + hallarNumeroOutput(((((long double)mapers / (long double)reducers) - (long double)parteEntera) * (long double)reducers), parteEntera, reduceActual);
                po = po + 0.55;
                sprintf(nombreBuf, "Buf_%d", (int)po);
                fdB = open(nombreBuf, O_RDONLY);
                if (fdB == -1)
                {
                    perror("Error: no se pudo abrir el pipe del buffer");
                    exit(0);
                }
                read(fdB, &bufferActual, sizeof(BufferP) * parteEntera2);
                for ( i = 0; i < parteEntera2; i++) 
                {
                    if (bufferActual[i].key != 0)
                    {
                        if (parametros->intermedios == 1)
                        {
                            key = bufferActual[i].key;
                            valor = bufferActual[i].valor;
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
        }
    }
}
void borrarArchivos(struct Parametros *parametros)
{
    int contador = 0,
        i;
    char nombreArch[20];
    for ( i = 0; i < parametros->nmappers; i++)
    {
        sprintf(nombreArch, "pipeM_%d", i);
        remove(nombreArch);
    }
    for ( i = 0; i < parametros->nmappers; i++)
    {
        sprintf(nombreArch, "Buf_%d", i);
        remove(nombreArch);
    }
    for ( i = 0; i < parametros->nreducers; i++)
    {
        sprintf(nombreArch, "pipeR_%d", i);
        remove(nombreArch);
    }
    for ( i = 0; i < parametros->nreducers; i++)
    {
        sprintf(nombreArch, "intermedio_%d.txt", i);
        remove(nombreArch);
    }
}
int calcularValorFinal(struct Parametros *parametros)
{
    pReducer infoRecibida;
    char nombreRedu[15];
    int fd, 
        contador = 0,
        reducers = parametros->nreducers,
        i;
    for ( i = 0; i < reducers; i++)
    {
        sprintf(nombreRedu, "pipeR_%d", i);
        fd = open(nombreRedu, O_RDONLY);
        read(fd, &infoRecibida, sizeof(BufferP) * reducers);
        if (infoRecibida.valor != 0)
        {
            contador = contador + infoRecibida.valor;
        }
    }
    return contador;
}
int master(struct Parametros *parametros, struct Consulta *consulta) 
{
    int validacionConsulta = interpretarConsulta(consulta), 
        validacionAsigPipe1,
        valorFinal,
        reducers = parametros->nreducers,
        maps = parametros->nmappers,
        id,
        idR,
        i;
    struct timeval tiempo_i, tiempo_f, tiempo_tot; 
    char nombrePipe[15];

    if (validacionConsulta == -1)
    {
        printf("ERROR: ocurrio un error inesperado en la lectura de la consulta\n");
        return -1;
    }
    gettimeofday(&tiempo_i, NULL);                                    
    validacionAsigPipe1 = asignacionPipeMapper(parametros, consulta); 
    
    if (validacionAsigPipe1 == -1)
    {
        printf("ERROR: ocurrio un error inesperado en la asignacion del pipe 1\n");
        return -1;
    }
    for ( i = 0; i < parametros->nmappers; i++) 
    {
        id = fork(); 
        if (id == 0)
        {
            sprintf(nombrePipe, "pipeM_%d", i); 
            map(nombrePipe, parametros, i);     
            exit(0);
        }
        else
        {
            wait(NULL); 
        }
    }
    for ( i = 0; i < parametros->nreducers; i++) 
    {
        idR = fork(); 
        if (idR == 0)
        {
            reduce(parametros, i); 
            exit(0);
        }
        else
        {
            wait(NULL); 
        }
    }
    gettimeofday(&tiempo_f, NULL);               
    timersub(&tiempo_f, &tiempo_i, &tiempo_tot); 
    printf("Resultados\n"); 
    printf("El tiempo que tomo el programa fue de: %li segundos %li microsegundos\n", tiempo_tot.tv_sec, tiempo_tot.tv_usec);
    valorFinal = calcularValorFinal(parametros);
    printf("el valor final es: %d\n", valorFinal);
    return 0;
}

int main(int argc, char *argv[])
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
    for ( i = 0; i < numMappers; i++)
    {
        sprintf(nombrePipe, "pipeM_%d", i);
        banderaPipe = open(nombrePipe, O_CREAT, 0400 | 0200);
        if (banderaPipe == -1)
        {
            perror("pipe");
            exit(1);
        }
        close(banderaPipe);

        sprintf(nombrepipeBuffer, "Buf_%d", i);
        banderaPipe = open(nombrepipeBuffer, O_CREAT, 0400 | 0200);
        if (banderaPipe == -1)
        {
            perror("pipe");
            exit(1);
        }
    }
    for ( i = 0; i < numReducers; i++)
    {
        sprintf(nombrePipe, "pipeR_%d", i);
        banderaPipe = open(nombrePipe, O_CREAT, 0400 | 0200);
        if (banderaPipe == -1)
        {
            perror("pipe");
            exit(1);
        }
        close(banderaPipe);
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
            condicional = 1;
            printf("ERROR: vuelva a intentar con un valor valido (1,2) por favor\n");
        }
        else 
        {
            printf("se ingreso un valor no valido, vuelva a intentarlo\n");
        }
    }
}