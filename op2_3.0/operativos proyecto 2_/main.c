///////////////////////////////////////////////////////////////////////////////
// Proyecto 2 Sistemas Operativos                                             //
// Profesora Mariela Curiel                                                   //
// Diego Alejandro Cardozo Rojas y Brayan Estiben Giraldo Lopez               //
////////////////////////////////////////////////////////////////////////////////

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
#include "main.h"

int verificarInt(char segmento[])
{
    int siEs = 0;
    int tamano = strlen(segmento);
    for (int i = 0; i < tamano; i++) //analiza cada dato ingresado por el usuario
    {
        if (!isdigit(segmento[i])) //si algo no es entero entoces suelta error el programa
        {
            siEs = 1;
        }
    }
    return siEs;
}
int impresion_menu() // funcion que imprime el primer menu del programa
{
    char c[250]; //variable que guarda la entrada de el usuario
    printf("\nMenu del programa");
    printf("\n1) Realizar una consulta");
    printf("\n2) Salir del Sistema");
    printf("\n");
    scanf("%s", c);                    //leemos la entreada de el usario
    int verificador = verificarInt(c); //verificador que se usa para saber si lo ingresado por el usuario es un int
    if (verificador == 1)
    {
        printf("ERROR: El dato ingresado no es un numero entero\n");
        return -1;
    }
    if (atoi(c) == 1) //valida si es 1
    {
        return atoi(c);
    }
    else if (atoi(c) == 2) //valida si el 2
    {
        return atoi(c);
    }
    else //retorna error si el valor no es valido
    {
        return -1;
    }

    //hola esto es una prueba
}

bool isInteger(char *str) //esta funcion fue tomada de internet para poder saber si un char es un dato numerico, se m¿implementa en la funcion de interpretarConsulta https://www.geeksforgeeks.org/c-program-detect-tokens-c-program/
{
    int i, len = strlen(str);

    if (len == 0)
        return (false);
    for (i = 0; i < len; i++)
    {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
            return (false);
    }
    return (true);
}

int interpretarConsulta(struct Consulta *consulta) // funcion que guarda la consulta ingresada en una estructura
{

    char ff[100]; //linea que se lee en la consulta
    char *token;  //token que va a avazar en la linea
    printf("\nMenu de consulta");
    printf("\nIngrese la consulta de la forma: (numero columna, signo, valor)");
    printf("\n");
    scanf("%s", ff);              //guarda lo ingresado en la variable ff
    int comas = 0;                //variable que cuenta las comas que ingreso el usuario para asi saber si los datos estan bien ingresados
    for (int i = 0; i < 100; i++) //analiza cada dato ingresado por el usuario
    {
        if (ff[i] == ',') //si algo no es entero entoces suelta error el programa
        {
            comas++;
        }
    }
    if ((int)strlen(ff) <= 4) //valida que los datos ingresados por el usuario sean necesarios
    {
        printf("ERROR: numero de valores invalido\n");
        return -1;
    }
    token = strtok(ff, ","); // token que va a recorrer lo ingresado hasta encontrar una coma(,)
    if (atoi(token) <= 0)    //valida que se ingrese un numero mayor a 0 en el primer dato
    {
        printf("ERROR: valor de la columna invalido\n");
        return -1;
    }

    consulta->columna = atoi(token); //asigna el dato a la estructura
    token = strtok(NULL, ",");
    if (strcmp(token, ">") == 0) //valida que se ingrese un signo valido en el primer dato
        consulta->signo = 0;     //asigna el dato a la estructura
    else if (strcmp(token, "<") == 0)
        consulta->signo = 1; //asigna el dato a la estructura
    else if (strcmp(token, "<=") == 0)
        consulta->signo = 3; //asigna el dato a la estructura
    else if (strcmp(token, ">=") == 0)
        consulta->signo = 2; //asigna el dato a la estructura
    else if (strcmp(token, "==") == 0)
        consulta->signo = 4; //asigna el dato a la estructura
    else                     //valida que el signo sea uno permitido
    {
        printf("ERROR: valor del signo es invalido\n");
        return -1;
    }
    
    token = strtok(NULL, ",");
    if (isInteger(token) == false) //llama a la funcion isinteger
    {
        printf("ERROR: el tercer dato de la consulta no es valido\n");
        return -1;
    }
    consulta->valor = atoi(token); //asigna el dato a la estructura
    return 0;
}

double encontrarCambiante(struct Parametros *parametros)
{
    int maps = parametros->nmappers;                               //numero de mappers
    int line = parametros->lineas;                                 //numero de reducers
    double lineasArchivos = ((double)line) / ((double)maps);       //calcula las lineas que debe tener cada split
    int parteEntera = ((int)lineasArchivos);                       //parte entera de la variable lineasArchivos
    double sobras = (lineasArchivos - (double)parteEntera) * maps; //calcula cuantas lineas quedan sobrando si la divicion no es exacta

    return sobras;
}

int asignacionPipeMapper(struct Parametros *parametros, struct Consulta *consulta)
{
    FILE *registros;                                                          //variable del archivo de los registros
    registros = fopen(parametros->logfile, "r");                              //abre el archivo de los registros
    int maps, line, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, fd; // variables para asignar los datos del registro
    maps = parametros->nmappers;                                              //numero de mappers
    line = parametros->lineas;                                                //numero de reducers
    double lineasArchivos = ((double)line) / ((double)maps);                  //calcula las lineas que debe tener cada split
    int parteEntera = ((int)lineasArchivos);                                  //parte entera de la variable lineasArchivos
    double sobras = (lineasArchivos - (double)parteEntera) * maps;            //calcula cuantas lineas quedan sobrando si la divicion no es exacta
    int auxSobrantes = 1;                                                     //contador de cuantos sobrantes se han asignado
    char nombrePipe[15];
    char lineaArchivo[1000];
    pMapper infoEnviar[parteEntera];
    pMapper infoEnviar2[parteEntera + 1];

    if (sobras == (double)0) //se ejecuta si no hay lineas sobrantes
    {

        for (int x = 0; x < maps; x++)
        {
            sprintf(nombrePipe, "pipeM_%d", x);
            fd = open(nombrePipe, O_WRONLY);
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe");
                return -1;
            }
            for (int z = 0; z < parteEntera; z++) //copia las variables del archivo de registros a el nuevo split
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
            close(fd); //cierra del pipe
        }
        fclose(registros); //cierra el archivo de registros
    }

    if (sobras != (float)0) //se ejecuta si no hay lineas sobrantes
    {
        printf("\nADVERTENCIA: la divicion entre mapers y las lineas totales no es exacta");
        printf("\nse hara el reparto lo mas equilibrado posible\n");
        for (int z = 0; z < maps; z++)
        {
            sprintf(nombrePipe, "pipeM_%d", z); //ajusta el nombre de el archivo split
            fd = open(nombrePipe, O_WRONLY);    //abre el archivo
            if (fd == -1)
            {
                perror("Error: no se pudo abrir el pipe");
                return -1;
            }
            if (auxSobrantes < sobras)
            {
                auxSobrantes++;
                for (int z = 0; z < parteEntera + 1; z++) //copia las variables del archivo de registros a el nuevo split
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
                close(fd); //cierra del pipe
            }
            else //asigna una linea sobrante a el archivo pipe
            {
                for (int z = 0; z < parteEntera; z++) //copia las variables del archivo de registros a el nuevo pipe
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
                close(fd); //cierra del pipe
            }
        }
        fclose(registros); //cierra el archivo de registros
    }
    return 0;
}

int map(char nombrePipe[], struct Parametros *parametros, int contador)
{

    int fd, key = 0, valor = 0;
    double numCambio = encontrarCambiante(parametros);
    int maps = parametros->nmappers;                         //numero de mappers
    int line = parametros->lineas;                           //numero de reducers
    double lineasArchivos = ((double)line) / ((double)maps); //calcula las lineas que debe tener cada split
    int parteEntera = ((int)lineasArchivos);                 //parte entera de la variable lineasArchivos
    char nombreBuf[15], lineaPipeActual[1000], *token;
    if (contador < numCambio)
    {
        parteEntera++;
    }
    pMapper infoDelPipe[parteEntera];
    fd = open(nombrePipe, O_RDONLY);
    if (fd == -1)
    {
        perror("Error: no se pudo abrir el pipe");
        return -1;
    }
    read(fd, &infoDelPipe, sizeof(pMapper) * (parteEntera));
    struct Consulta consulta;
    
    sprintf(nombreBuf, "Buf_%d.txt", contador); //ajuste de el nombre del buf
    FILE *Buf = fopen(nombreBuf, "w");          //se crea el archivo buf
    if (Buf == NULL)
    {
        perror("Error: no se pudo abrir el archivo del buffer");
        exit(0);
    }
    for (int v = 0; v < parteEntera; v++)
    {
        consulta.columna = infoDelPipe[v].consul.columna;
        consulta.signo = infoDelPipe[v].consul.signo;
        consulta.valor = infoDelPipe[v].consul.valor;
        sprintf(lineaPipeActual, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", infoDelPipe[v].a, infoDelPipe[v].b, infoDelPipe[v].c,
                infoDelPipe[v].d, infoDelPipe[v].e, infoDelPipe[v].f, infoDelPipe[v].g, infoDelPipe[v].h, infoDelPipe[v].i, infoDelPipe[v].j,
                infoDelPipe[v].k, infoDelPipe[v].l, infoDelPipe[v].m, infoDelPipe[v].n, infoDelPipe[v].o, infoDelPipe[v].p, infoDelPipe[v].q,
                infoDelPipe[v].r);
        token = strtok(lineaPipeActual, " "); //tocken que para poder recorrer la columna de la linea
        key = atoi(token);                    // asigna la key a el valor de el token actual
        for (int i = 1; i <= 18; i++)         //for que ejecuta que se ejecuta por cada columna de la linea del archivo
        {
            switch (consulta.signo) //evalua el signo de la consulta
            {
            case 0: //caso de que el signo sea >
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor > (consulta.valor))
                    {
                        //printf("el valor dela key es:%d y el valor es : %d\n", key, valor);
                        fprintf(Buf, "%d %d\n", key, valor);
                    }
                }
                break;
            case 1: //caso de que el signo sea <
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor < (consulta.valor))
                    {
                        fprintf(Buf, "%d %d\n", key, valor); //guarla la key y el valor en el nuevo archivo
                    }
                }
                break;
            case 2: //caso de que el signo sea >=
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor >= (consulta.valor))
                    {
                        fprintf(Buf, "%d %d\n", key, valor); //guarla la key y el valor en el nuevo archivo
                    }
                }
                break;
            case 3: //caso de que el signo sea <=
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor <= (consulta.valor))
                    {
                        fprintf(Buf, "%d %d\n", key, valor); //guarla la key y el valor en el nuevo archivo
                    }
                }
                break;
            case 4: //caso de que el signo sea  =
                if (i == consulta.columna)
                {
                    valor = atoi(token);
                    if (valor == (consulta.valor))
                    {
                        fprintf(Buf, "%d %d\n", key, valor); //guarla la key y el valor en el nuevo archivo
                    }
                }
                break;
            }
            token = strtok(NULL, " ");
        }
    }
    close(fd);
    fclose(Buf);   //cierra el archivo buf
    return 0;
}



int master(struct Parametros *parametros, struct Consulta *consulta) // funcion master que se encarga de todo el proceso de archivos del programa
{
    int validacionConsulta, validacionAsigPipe1;
    struct timeval tiempo_i, tiempo_f, tiempo_tot; //variables para poder medir la ejecucion de un programa
    validacionConsulta = interpretarConsulta(consulta);
    char nombrePipe[100];
    if (validacionConsulta == -1)
    {
        printf("ERROR: ocurrio un error inesperado en la lectura de la consulta\n");
        return -1;
    }

    gettimeofday(&tiempo_i, NULL);                                    //optiene el tiempo inicial de ejecucion
    validacionAsigPipe1 = asignacionPipeMapper(parametros, consulta); //llama a la funcion que hace el reparto de las lineas totales en los pipes

    if (validacionAsigPipe1 == -1)
    {
        printf("ERROR: ocurrio un error inesperado en la asignacion del pipe 1\n");
        return -1;
    }
    
    for (int i = 0; i < parametros->nmappers; i++) //for que se encarga de crear los procesos de los n-mappers
    {
        int id = fork(); //creacion del proceso
        if (id == 0)
        {
            sprintf(nombrePipe, "pipeM_%d", i);   //se ajusta el nombre del archivo para cada mapper
            map(nombrePipe, parametros, i); //cada mapper ejecuta la funcion de map
            exit(0);
        }
        else
        {
            wait(NULL); //el programa espera a que los procesos acaben la asignacion de los datos
        }
    }

    for (int i = 0; i < parametros->nreducers; i++) //for que se encarga de crear los procesos de los n-reducers
    {
        int idR = fork(); //creacion del proceso
        if (idR == 0)
        {
            reduce(parametros, i); //cada reducer ejecuta el codigo de la funcion reduce
            exit(0);
        }
        else
        {
            wait(NULL); //el programa espera a que los procesos acaben la asignacion de los datos
        }
    }
    
    return 0;
}

int main(int argc, char *argv[])
{
    int condicional = 0, opcion_menu, resultado, numMappers = atoi(argv[3]), numReducers = atoi(argv[4]), banderaPipe;
    struct Parametros *parametros = (struct Parametros *)malloc(sizeof(struct Parametros)); //crea la estructura para guardar los parametros
    struct Consulta consulta;                                                               //crea la estructura de la consulta
    char nombrePipe[100];

    if (argc < 6) //valida que se ingresaran datos a el programa
    {
        printf("Alerta los datos se ingresaron de manera erronea\n");
        exit(0);
    }

    strncpy(parametros->logfile, argv[1], PATH_MAX); //en las siguientes lineas se asignan los parametros ingresados a la estructura
    parametros->lineas = atoi(argv[2]);
    parametros->nmappers = atoi(argv[3]);
    parametros->nreducers = atoi(argv[4]);
    parametros->intermedios = atoi(argv[5]);

    pMapper pipeMapper[atoi(argv[3])];
    pReducer pipeReducers[atoi(argv[4])];

    for (int i = 0; i < numMappers; i++)
    {
        sprintf(nombrePipe, "pipeM_%d", i);
        banderaPipe = open(nombrePipe, O_CREAT, S_IRUSR | S_IWUSR);
        if (banderaPipe == -1)
        {
            perror("pipe");
            exit(1);
        }
        close(banderaPipe);
    }

    for (int i = 0; i < numReducers; i++)
    {
        sprintf(nombrePipe, "pipeR_%d", i);
        banderaPipe = open(nombrePipe, O_CREAT, S_IRUSR | S_IWUSR);
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
        opcion_menu = impresion_menu(); //llama a la funcion que pregunta al usuario que quiere hacer con el programa
        if (opcion_menu == 1)           // se ejecuta si el usuario quiere realizar una consulta
        {
            resultado = master(parametros, &consulta); //llama a la funcion master que se encarga de realizar todo el proceso
        }
        else if (opcion_menu == 2) // opcion que finaliza el programa
        {
            condicional = 1;
        }
        else if (opcion_menu == -1)
        {
            condicional = 1;
            printf("ERROR: vuelva a intentar con un valor valido (1,2) por favor\n");
        }
        else // valida que solo se puedan ingresar valores validos
        {
            printf("se ingreso un valor no valido, vuelva a intentarlo\n");
        }
    }
}

//nos quedamos en la creacion de los splits y llamado a la funcion