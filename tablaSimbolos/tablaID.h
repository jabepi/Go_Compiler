#ifndef IMP_TABLAID_H
#define IMP_TABLAID_H

#include "../tiposDatos/abb.h"

//Iniciamos la tabla de ID
void iniciarTablaID();

//Liberamos la memorias reservada por la tabla de ID
void borrarTablaID();

//Imprimimos todos los elementos de la tabla de ID por niveles
void imprimirTablaID();

//Insertamos un nodo en la tabla
void insertarTablaID(int nivel , tipoelem nodo);

//Inserta un nodo sen la tabla
void buscarTablaID(int nivel, char *lexema, tipoelem* nodo);

#endif
