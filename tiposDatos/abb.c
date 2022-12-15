#include "abb.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"


/*Estructora que contiene los datos del nodo y los punteros a sus vecinos*/
struct celda {
    tipoelem info;
    struct celda *izq, *der;
};


/*------------------------Funciones privadas------------------------*/
/*Extraer la clave de una celda */
tipoclave _clave_elem(tipoelem *E) {
    return E->lexema;
}

//Comparación de calves (caracteres del lexema)
int comparar_claves(tipoclave cl1, tipoclave cl2) {
    return !strcmp(cl1, cl2) ? 0 : strcmp(cl1, cl2) > 0 ? 1 : -1;
}

// Función privada para comparar las claves
int _comparar_clave_elem(tipoclave cl, tipoelem E) {
    return comparar_claves(cl, _clave_elem(&E));
}

// Funciones para desplazarse dentro del arbol
abb izq(abb A) {
    return A->izq;
}
abb der(abb A) {
    return A->der;
}

//Ver si el árbol está vacío
unsigned es_vacio(abb A) {
    return A == NULL;
}

//Leer el contenido de un nodo del arbol
void leer(abb A, tipoelem *E) {
    *E = A->info;
}

//Liberar toda la memoria reservada en los atributos de un nodo (se modificará cuando se incrementen en prácticas
//posterirores
void _destruir_elem(tipoelem *E) {
    free(E->lexema);
}

// Funcion privada que devuelve mínimo de subárbol dcho
tipoelem _suprimir_min(abb *A) {//Se devuelve el elemento más a la izquierda
    abb aux;
    tipoelem ele;
    if (es_vacio((*A)->izq)) {//Si izquierda vacía, se devuelve valor nodo actual A
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq); //se vuelve a buscar mínimo rama izquierda
    }
}


/*------------------------Funciones públicas------------------------*/

//Imprimir todos los elmentos del arbol con sus atributos
void imprimirArbol(abb A) {
    tipoelem E;
    if (!es_vacio(A)) {
        imprimirArbol(izq(A));
        leer(A, &E);
        printf("\t< %-10s, %d >\n", E.lexema, E.compLex);
        imprimirArbol(der(A));
    }
}

//Crea un nodo, establece los valores que tenga un nodo, en este caso solo lexema y compoennte léxico
void asignarValNodo(tipoelem *E, char *lexema, int compLex) {
    int tam = (int) strlen(lexema);
    E->lexema = (char *) malloc(sizeof(char) * (tam + 1));
    strncpy(E->lexema, lexema, tam);
    E->lexema[tam] = '\0';
    E->compLex = compLex;
}

//Crear el arbol vacío
void crear(abb *A) {
    *A = NULL;
}

//Liberar toda la memoria reservada por el arbol
void destruir(abb *A) {
    if (*A != NULL) {
        destruir(&(*A)->izq);
        destruir(&(*A)->der);
        _destruir_elem(&((*A)->info));
        free(*A);
        *A = NULL;
    }
}

//Bucamos un lexema concreto en el arbol
void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (es_vacio(A)) {
        return;
    }
    int comp = _comparar_clave_elem(cl, A->info);
    if (comp == 0) {
        *nodo = A->info;
    } else if (comp < 0) {
        buscar_nodo(A->izq, cl, nodo);
    } else {
        buscar_nodo(A->der, cl, nodo);
    }
}


// Insertar un nodo en el arbol (sin que haya otro igual)
void insertar(abb *A, tipoelem E) {
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof(struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    }
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp > 0) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

// Funcion que permite eliminar un nodo del arbol (se utilizará en futuras versiones, cuando existan bloques)
void suprimir(abb *A, tipoelem E) {
    abb aux;
    if (es_vacio(*A)) {
        return;
    }
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp < 0) { //if (E < (*A)->info) {
        suprimir(&(*A)->izq, E);
    } else if (comp > 0) { //(E > (*A)->info) {
        suprimir(&(*A)->der, E);
    } else if (es_vacio((*A)->izq) && es_vacio((*A)->der)) {
        _destruir_elem(&((*A)->info));
        free(*A);
        *A = NULL;
    } else if (es_vacio((*A)->izq)) { // pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        _destruir_elem(&aux->info);
        free(aux);
    } else if (es_vacio((*A)->der)) { //pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        _destruir_elem(&aux->info);
        free(aux);
    } else { //ni derecha ni izquierda esta vacio, busco mínimo subárbol derecho
        _destruir_elem(&(*A)->info); //elimino la info pero no libero el nodo,
        //pues en su sitio voy a poner el mínimo del subárbol derecho
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}
