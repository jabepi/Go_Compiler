#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Macros de la tabla HASH*/
#define Tam 300
#define VACIO NULL

typedef struct {
    int id;
    char *key;
} keyWord;


void InicializarTablaKey();
int BusquedaTablaKey(char *cad, int *e);
void InsertarTablaKey(keyWord e);
void BorrarTablaKey();
void imprimirTablaKey();