#include <limits.h>

struct Parametros //estructura que contiene los parametros ingresados por el usuario
{
    char logfile[PATH_MAX]; 
    int lineas;             //numero de lineas del registro
    int nmappers;           //numero de mappers que desea el usuario
    int nreducers;          //numero de reducers que solicita el usuario
    int intermedios;        //numero de archivos intermedios definidos por usuario
};

struct Consulta
{
  int columna;
  int signo; // 0 = >, 1 = <, 2 = >=, 3 = <=, 4 = = 
  int valor;
};

typedef struct pipeMapper
{
    struct Consulta consul;
    int a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r;
} pMapper;

typedef struct pipeReducer
{
    int valor;
} pReducer;

typedef struct buffer
{
    int valor, key;
} BufferP;

typedef struct datosBuffer
{
    int id, cantidadLineas;
} DatosEnbuffer;

