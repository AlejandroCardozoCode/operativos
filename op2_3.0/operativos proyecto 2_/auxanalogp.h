#include <limits.h>

struct Parametros 
{
    char logfile[4096]; 
    int lineas;             
    int nmappers;           
    int nreducers;          
    int intermedios;        
};

struct Consulta
{
  int columna;
  int signo; 
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

