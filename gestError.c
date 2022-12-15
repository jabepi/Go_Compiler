
#include <stdio.h>

#include "gestError.h"
#include "definiciones.h"

//-----Errores del sistema de entrada------
void errorLectura(char* archivo){
    printf("Error (1.1): No se ha podido abrir el fichero \"%s\"\n", archivo);
}
void errorOverFlow(){
    printf("Error (1.2): El siguiente lexema se ha truncado por superar el tamaño máximo permitido\n");
}

//-----Errores léxicos------
void lexemaMalFormado(compLexico comp, int linea) {
    printf("Error (2.1) de construcción: <lexema \"%s\", línea: %d, compLexIntuido: %d>\n",
           getLexema(&comp), linea, getCompLex(&comp));
}
void caracterNoIdentficado(char caracter, int linea){
    printf("Error (2.1) de identificación: <Caracter ASCII: %d, línea: %d>\n", caracter, linea);
}