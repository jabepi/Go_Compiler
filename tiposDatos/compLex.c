#include "compLex.h"
#include "stdlib.h"
#include "string.h"

struct comp {
    int comLex;
    char *lexema;
};

/*Inserción en la estrucutra*/
void constCompLex(compLexico *componente) {
    *componente = (struct comp*) malloc(sizeof(struct comp));
    (*componente)->comLex = 0;
    (*componente)->lexema = NULL;
}

void limpiarComponente(compLexico *componente) {
    (*componente)->comLex = 0;
    free((*componente)->lexema);
    (*componente)->lexema = NULL;
}

void destruirComponente(compLexico *componente){
    free((*componente)->lexema);
    free((*componente));
}

void insertarLexema(compLexico *componente, char *lexema) {
    (*componente)->lexema = lexema;
}

void insertarComLex(compLexico *compLex, int comLex) {
    (*compLex)->comLex = comLex;
}

/*Obtención de datos de la estructura*/
char *getLexema(compLexico *componente) {
    return (*componente)->lexema;
}

int getCompLex(compLexico *componente) {
    return (*componente)->comLex;
}
