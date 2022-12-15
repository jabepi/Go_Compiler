#include <stdio.h>

#include "anaSintac.h"
#include "../definiciones.h"
#include "../lexico/anaLexico.h"
#include "../tiposDatos/compLex.h"


void inciarSintac() {

    compLexico comp = NULL;

    printf("------------Componentes Léxicos-----------\n");

    comp = siguienteComponente();
    while (getCompLex(&comp)!=FIN_FICHERO) {
        printf("<Componente léxico: %3d; "
               "Lexema: \"%s\">\n", getCompLex(&comp), getLexema(&comp));
        destruirComponente(&comp);
        comp = siguienteComponente();
    }

    destruirComponente(&comp);

    printf("------------------------------------------\n\n");
}