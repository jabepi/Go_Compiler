#include "tablaID.h"
#include "stdlib.h"

//Variables para contrololar los nivel del bosque (utilizadas en próximas entregas)
#define intiNiveles 1
//int n_niveles;

//Variable con todos los niveles del bosque
abb *tablaIDs;

/*Reservamos memoria para intiNiveles niveles del bosque en la tablaIDs (la estructura se
 *modificará cuando se introduzan bloques)*/
void iniciarTablaID() {
    tablaIDs = (abb *) malloc(intiNiveles * sizeof(abb));
    crear(&tablaIDs[0]);
    //n_niveles = intiNiveles;
}

//Liberamos la memorias reservada por el bosque
void borrarTablaID() {
    destruir(&tablaIDs[0]);
    free(tablaIDs);
}

//Imprimimos el bosque
void imprimirTablaID() {
    imprimirArbol(tablaIDs[0]);
}

//Insertar un nodo en un nivel concreto del bosque
void insertarTablaID(int nivel, tipoelem nodo) {
    insertar(&tablaIDs[nivel], nodo);
}

//Busca un nodo en el bosque
void buscarTablaID(int nivel, char *lexema, tipoelem* nodo) {
    buscar_nodo(tablaIDs[nivel], lexema, nodo);
}