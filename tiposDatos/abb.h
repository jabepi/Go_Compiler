#ifndef ABB_H
#define ABB_H

//Elemento empleado para la búsqueda (en nuestro caso el lexema)
typedef char* tipoclave;

//Estructura con el contenido de un nodo
typedef struct {
    int compLex;
    char* lexema;
}tipoelem;

//Estructura privada con organización interna del arbol
typedef struct celda * abb;

//Crear un arbol
void crear(abb *A);

//Liberar la memoria de un arbol
void destruir(abb *A);

//Asignar los valores asociados a un nodo
void asignarValNodo(tipoelem *E, char* lexema, int compLex);

//Buscar un elemento en el árbol
void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo);

//Inserta un nuevo elemento en el árbol QUE NO EXISTÍA PREVIAMENTE
void insertar(abb *A, tipoelem E);

//Suprimir un elmentos concreto del arbol
void suprimir(abb *A,tipoelem E);

//Impimir todos los elmentos del arbol en inorden
void imprimirArbol(abb A);

#endif

