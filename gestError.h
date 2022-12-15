#include "tiposDatos/compLex.h"

//Error del sistema de entrada
void errorLectura(char* archivo);

//El tamaño del lexema es demasiado grande
void errorOverFlow();

//Indicamos que se ha procesado un lexema mal construido y cual creemos que es
void lexemaMalFormado(compLexico comp, int linea);

//Indicamos que no hemos podido procesar el caracter
void caracterNoIdentficado(char caracter, int linea);