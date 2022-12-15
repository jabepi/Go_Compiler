#include "tablaKeyWords.h"
#include "../definiciones.h"
#include "ctype.h"

int numKey;

/*Variable con la tabla hash de keywords*/
keyWord* t[Tam];

/*Función Hash*/
int Hash(char *cad) {
    int i, suma = 0;
    int K = 500;
    for (i = (int) strlen(cad) - 1; i >= 0; i--) {
        suma = (suma * K + cad[i]) % Tam;
    }
    return suma;
}

/*Devuelve la posisicón de un elemento en la tablaSimbolos*/
int posicion(char *cad) {

    int ini, i, aux;
    ini = Hash(cad);

    for (i = 0; i < Tam; i++) {
        aux = (ini + i * i) % Tam;
        if (t[aux] == VACIO) {
            return aux;
        }
        if (!strcasecmp(t[aux]->key, cad)) {
            return aux;
        }
    }
    return ini;
}


/* Busca y devuelve un elemento si está en la tablaSimbolos (devuelve 1 si está en la tablaSimbolos y 0 si
 * no está*/
int BusquedaTablaKey(char *cad, int *e) {

    int pos = posicion(cad);

    if (t[pos] == VACIO)
        return 0;
    else {
        if (strcasecmp(t[pos]->key, cad) == 0) {
            *e = t[pos]->id;
            return 1;
        } else return 0;
    }
}

/*Inserta un elemento en la tablaSimbolos Hash*/
void InsertarTablaKey(keyWord e) {
    int pos;
    pos = posicion(e.key);

    if (t[pos] == VACIO) {
        t[pos] = malloc(sizeof(keyWord));
        *t[pos] = e;
    } else if (strcmp(t[pos]->key, e.key) != 0) {
        printf("No se encuentra posicion libre.\n");
    }
}

/*Incializamos la tablaSimbolos Hash con las palabras clave*/
void InicializarTablaKey() {

    //Definimos las palabras reservadas
    char *keywords[] = {"package", "import", "func", "chan",
                        "for", "go", "var", "range"};
    int codigos[] = {PACKAGE, IMPORT, FUNC, CHAN, FOR, GO, VAR, RANGE};
    numKey = sizeof(codigos)/sizeof(int);

    /*Insertamos en la tabla hash las palabras reservadas*/
    for (int i = 0; i < numKey; i++) {
        keyWord a;
        a.key = (char *) malloc(sizeof(keywords[i]));
        strcpy(a.key, keywords[i]);
        a.id = codigos[i];
        InsertarTablaKey(a);
    }
}

/*Liberamos toda la memoria de la palabras clave*/
void BorrarTablaKey() {
    /* Para evitar tener las palabras claves almacenadas como una variable global y para no tener que recorrer
     * toda la tabla en el borrado, volvemos a definir las keyword para ahorrar tiempo de cálculo a costa de mayor
     * dificultad en la actualización de las palabras clave*/
    char *keywords[] = {"package", "import", "func", "chan",
                        "for", "go", "var", "range"};

    int pos;
    for (int i = 0; i < numKey; i++) {
        pos = posicion(keywords[i]);
        free(t[pos]->key);
        free(t[pos]);
    }
}


//Imprimimos las palabras clave y el indentificador que tienen asociado
void imprimirTablaKey(){
    char *keywords[] = {"package", "import", "func", "chan",
                        "for", "go", "var", "range"};

    for (int i = 0; i < numKey; i++) {
        printf("\t< %-10s, %d >\n",keywords[i], t[posicion(keywords[i])]->id);
    }

}

