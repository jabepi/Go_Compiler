#include "tablaSimb.h"
#include "../definiciones.h"
#include "tablaKeyWords.h"
#include "tablaID.h"

//Reservamosmos memoria para la tabla
void iniciarTabla(){

    //Inciamos la tabla de ID
    iniciarTablaID();

    //Cargamos las palabras reservada por el lenguaje en la tabla de keywords
    InicializarTablaKey();
}

void borrarTabla(){
    BorrarTablaKey();                    //Borramos la memoria de la tabla de palabras reservadas
    borrarTablaID();                     //Borramos la memoria de la tabla de ID
}

void imprimirTabla(){
    printf("------Tabla de Símbolos------\n");
    imprimirTablaKey();
    imprimirTablaID();
    printf("-----------------------------\n\n");
}

void insertaElemetento(char* lexema, int nivel, int compLexico){

    //Creamos el nodo
    tipoelem nodo;
    asignarValNodo(&nodo, lexema, compLexico);

    //Lo insertamos en el arbol correspondiente (para esta entrega solo trabajamos con un bloque)
    insertarTablaID(nivel, nodo);
}

//Buscar elemento en la tabla de símbolos, devuelve el código asociado
int buscarLexema(char* lexema, int nivel){

    //Buscamos en la tabla de keyWords
    int id;
    if(BusquedaTablaKey(lexema, &id)){
        return id;
    }

    //Si no es una palabra clave puede es un identificador
    //Buscamos el identificador en el árbol
    tipoelem nodo;
    nodo.compLex = -1;
    buscarTablaID(nivel, lexema, &nodo);

    //En caso de no encontrarlo lo insertamos en la tabla
    if(nodo.compLex == -1){
        //Si no está en la tabla de símbolos lo insertamos
        insertaElemetento(lexema, 0, ID);
    }
    return ID;
}






