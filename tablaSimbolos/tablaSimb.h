#include "../tiposDatos/abb.h"

//Iniciamos la tabla, reservando memoria e introduciendo los elementos inciales
void iniciarTabla();

//Borramos toda la memoria de la tabla
void borrarTabla();

//Insertamos un elemento en la tablaSimbolos
void insertaElemetento(char* lexema, int nivel, int compLexico);

/* Devuelve -1 si no se encuentra el lexema buscado
 * Devuelve el componente léxico que es si se encuentra en la tablaSimbolos*/
int buscarLexema(char* lexema, int nivel);

//Imprimir la tabla de símbolos
void imprimirTabla();

