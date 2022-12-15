#include "definiciones.h"
#include "tiposDatos/compLex.h"
#include "entradaSalida/entradaSalida.h"
#include "sintactico/anaSintac.h"
#include "tablaSimbolos/tablaSimb.h"


int main(int argc, char* argv[]) {


    //---------------Inciamos el compilador------------------

    //Usamos el sistema de entrada para abrir el archiva
    abrirArchivo(argv[1]);

    //Iniciamos la tablaSimbolos de símbolos
    iniciarTabla();
    imprimirTabla();

    //Iniciamos analizador sintáctico
    inciarSintac();

    //---------------Cerramos el compilador------------------
    //Liberamos toda la memoria
    cerrarArchivo();
    imprimirTabla();
    borrarTabla();

    return 0;
}


