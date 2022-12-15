#include <stdlib.h>
#include <stdio.h>

#include "anaLexico.h"
#include "../definiciones.h"
#include "../tablaSimbolos/tablaSimb.h"
#include "../entradaSalida/entradaSalida.h"
#include "../gestError.h"


/*Variables globales*/
int linea = 1;      //Obtener la línea en cada momento
char Error = 0;


/*Funciones auxiliares*/
int isLetra(char caracter);

int isNumero(char caracter);

int isCaracterIgnorado(char caracter);

int isString(char caracter);

int isSimb(char caracter, int *a);

int isComment(char caracter, int *tipo);

char hexLit();

char decLit();

void insertCharSimp(char caract, compLexico comp);

void automataFlotaImgPar(compLexico comp);


/*Autómatas*/
void automataKEY_ID(char a, compLexico comp);

void automataString(compLexico comp, char caract);//diferenciar entre los dos tipos de string

void automataSimb(char caracter, compLexico comp, int tipo);

void automataNum(char caracter, compLexico comp);

void automataFlotaImgComp(compLexico comp);

void automataComment(int tipo);


/*-------------------------------------Funciones publicas-----------------------------------*/
compLexico siguienteComponente() {

    compLexico compLexico;
    constCompLex(&compLexico);                           //Inciamos la variable componente léxico
    int tipo;

    //Repetimos hasta poder mandar un componente léxico exitente
    do {
        limpiarComponente(&compLexico);
        Error = 0;                                               //Reiniciamos el error
        char caract = siguienteCaracter();                       //Obtenemos el caracter que toca procesar

        /*--------------------------------Elementos que no son componentes L----------------------------------------*/

        do {
            if (isCaracterIgnorado(caract)) {                    //Saltamos los caracteres ignorados
                do {
                    caract = siguienteCaracter();
                } while (isCaracterIgnorado(caract));
                retroceder(1);
                ignorar();
                caract = siguienteCaracter();
            }
            if (isComment(caract, &tipo)) {                 //Detectamos si hay un comentario
                automataComment(tipo);
                caract = siguienteCaracter();
            }
        } while (isCaracterIgnorado(caract));


        /*--------------------------------Autómata para elección de autómata----------------------------------------*/

        if (isLetra(caract)) {                      //Si es una letra es un KEYWORD o un ID llamamos al autómata
            automataKEY_ID(caract, compLexico);

        } else if (isString(caract)) {              //Si es una tilde invertida o comillas es un string
            automataString(compLexico, caract);

        } else if (isSimb(caract, &tipo)) {      //Si es un símbolo aceptado
            automataSimb(caract,
                         compLexico, tipo);
        }
            /*Detección de números*/
        else if (caract == '.') {                           //Si empiezan . solo puede ser un imaginario o un float
            automataFlotaImgComp(compLexico);
        } else if (isNumero(caract)) {              //Si empieza por un número puede ser cualquier número
            automataNum(caract,
                        compLexico);
        }
            /*Fin de fichero*/
        else if (caract == EOF) {                           //Si se ha llegado al fin de fichero
            insertarLexema(&compLexico,
                           NULL);
            insertarComLex(&compLexico,
                           FIN_FICHERO);
            return compLexico;
        }
            /*Gestión de errores*/
        else {
            Error = 2;
        }
        if (Error == 1) {                                    //1. Coincidencia posible, lexema mal formado
            lexemaMalFormado(compLexico, linea);
        } else if (Error == 2) {                             //2. Ningún autómana encontró noincidencia.
            caracterNoIdentficado(caract, linea);
            ignorar();
        }

    } while (Error != 0);


    return compLexico;
}



/*-------------------------------------Funciones privadas-----------------------------------*/

/*-------------Automatas-------------*/
//Autómata para detectar palabras reservada e identificadores
void automataKEY_ID(char a, compLexico comp) {

    //Variables auxiliares
    char *lexema;
    int compLex;

    //Si son solo letras es un KeyWord o un ID
    do {
        //Obtenemos el siguiente caracter
        a = siguienteCaracter();
    } while (isLetra(a) || isNumero(a));

    //Almacenamos el lexema encontrado
    retroceder(1);
    lexema = siguienteBloque();

    //Obtengo el código del componente lexico
    compLex = buscarLexema(lexema, 0);

    //Insertamos el lexema y el componente léxico en la estructura que enivamos al analizador léico
    insertarLexema(&comp, lexema);
    insertarComLex(&comp, compLex);
}

//Autómata para detectar Strings literales e interpretados
void automataString(compLexico comp, char caract) {

    //Variables auxiliares
    char *lexema;
    char aux;

    if (caract == STRING_LIT) {                              //Si es un string literal
        do {
            caract = siguienteCaracter();
        } while (caract != STRING_LIT);
    } else {                                                 //Si es un string interpretado
        do {
            do {
                aux = caract;
                caract = siguienteCaracter();
            } while (caract != INTERPRETED_STRING_LIT);
        } while (aux == '\\');
    }

    //Obtenemos el lexema que forma el string
    lexema = siguienteBloque();

    //Insertamos el lexema y el componente léxico en la estructura que enivamos al analizador léico
    insertarLexema(&comp, lexema);
    insertarComLex(&comp, caract);
}

//Autómata para detectar símbolos y operadores
void automataSimb(char caracter, compLexico comp, int tipo) {

    //Variables auxiliares
    char *lexema;

    switch (tipo) {

        case 1:
            //Si es del grupo 1, solo puede poseer varios caracteres si es un +
            if (caracter == MAS) {
                caracter = siguienteCaracter();

                //Si el siguiente caracter es un "=" el lexema es "+="
                if (caracter == IGUAL) {
                    lexema = siguienteBloque();
                    insertarLexema(&comp, lexema);
                    insertarComLex(&comp, MASIGUAL);
                }
                    //En caso contrario solo era un +
                else {
                    retroceder(1);
                    insertCharSimp('+', comp);
                }
            }
                //Si no es un más el caracter es el lexema por sí mismo
            else {
                insertCharSimp(caracter, comp);
            }
            break;

            //Solo hay dos lexemas pertencecientes al grupo 2
        case 2:
            //Si son dos puntos
            if (caracter == DOSPUNTOS) {
                caracter = siguienteCaracter();

                //Si el siguiente caracter es un "=" el lexema es ":="
                if (caracter == IGUAL) {
                    lexema = siguienteBloque();
                    insertarLexema(&comp, lexema);
                    insertarComLex(&comp, DOSPUNTOS_IGUAL);
                }
                    //En caso contrario solo eran ":"
                else {
                    retroceder(1);
                    insertCharSimp(DOSPUNTOS, comp);
                }
            }//Si es un <, el lexema debería ser <-
            else if (caracter == '<') {
                caracter = siguienteCaracter();

                //Si el siguiente caracter es un "-" es el lexema "<-"
                if (caracter == MENOS) {
                    lexema = siguienteBloque();
                    insertarLexema(&comp, lexema);
                    insertarComLex(&comp, MENOR_MENOS);
                }
                    //Si no, no hay coincidencia para ese componente léxico.
                else {
                    retroceder(1);
                    insertCharSimp('<', comp);
                    Error = 1;
                }
            } else {
                insertCharSimp(caracter, comp);
            }
            break;

            //Caracteres que no son combinación de varios
        case 3:
            insertCharSimp(caracter, comp);
            break;
        default:
            break;
    }
}

//Autómata para detectar principio y final de comentarios
void automataComment(int tipo) {

    char sig, caracter;

    if (tipo == 1) {
        sig = siguienteCaracter();
        //Evitamos el uso de condicionales dentro de un bucle almacenando el elemento anterior en un doble while
        do {
            do {
                caracter = sig;                               //Almaceno el caracter anterior
                sig = siguienteCaracter();                    //Obtengo el caracter siguiente
                if (sig == '\n') { linea++; }
            } while (sig != '/');
        } while (caracter != '*');
    } else {
        do {
            sig = siguienteCaracter();
        } while (sig != '\n');
    }
    ignorar();                                                //Ignoramos el contenido del comentario
}

/*Automatas de números*/
//1. Autómata de cualquier número
void automataNum(char caracter, compLexico comp) {

    char carS = siguienteCaracter();

    //En caso se que sea un 0 seguido de un x, solo puede ser un hexadecimal.
    if ((caracter == '0') && (carS == 'x')) {
        Error = hexLit();                            //Devuelve si el hexadecimal estaba bíen construido
        insertarComLex(&comp, INT_LIT);
        insertarLexema(&comp, siguienteBloque());
    } else {
        //Detectamos la parte entera
        retroceder(1);
        Error = decLit();
        carS = siguienteCaracter();

        //En caso de que al entero le siga un punto puede ser un imaginario o un float
        if (carS == '.') {
            automataFlotaImgComp(comp);
        } else if (carS == 'e') {
            automataFlotaImgPar(comp);
        }
            //Si el caracter es una i, es un imaginario
        else if (carS == 'i') {
            insertarComLex(&comp, IMAGINARY_LIT);
            insertarLexema(&comp, siguienteBloque());
        } else {
            retroceder(1);
            insertarComLex(&comp, INT_LIT);
            insertarLexema(&comp, siguienteBloque());
        }
    }
}

//2. Autómatas de flotantes e imaginarios
//Parcial: en caso de haber detectado un float por exponente, sin parte decimal
void automataFlotaImgPar(compLexico comp) {
    char carat = siguienteCaracter();
    if (carat == '+' || carat == '-') { //Después de la E se puede especificar el signo
        carat = siguienteCaracter();
        if (isNumero(carat)) {    //Tiene que habér un número después del signo
            Error = decLit();
            carat = siguienteCaracter();
        } else {
            Error = 1;
        }
    } else if (isNumero(carat)) { //Tamnbién es posible no especificar el signo
        Error = decLit();
        carat = siguienteCaracter();
    } else {                             //Si no hay nada relativo a un número después, está mal construido
        Error = 1;
    }
    if (carat == 'i') {
        insertarComLex(&comp, IMAGINARY_LIT);
        insertarLexema(&comp, siguienteBloque());
    } else {
        retroceder(1);
        insertarComLex(&comp, FLOAT_LIT);
        insertarLexema(&comp, siguienteBloque());
    }
}

//Completo: en caso de haber detectado el float con parte decimal y posiblmente exponente
void automataFlotaImgComp(compLexico comp) {

    //Detectamos todos los decimales que se encuentren después del punto
    Error = decLit();

    char carat = siguienteCaracter();

    //Cuando dejamos de detectar un decimal hay varias opciones//
    //1. Se va a definir un exponente
    if (carat == 'e' || carat == 'E') {
        carat = siguienteCaracter();
        if (carat == '+' || carat == '-') { //Después de la E se puede especificar el signo
            carat = siguienteCaracter();
            if (isNumero(carat)) {    //Tiene que habér un número después del signo
                Error = decLit();
                carat = siguienteCaracter();
            } else {
                Error = 1;
            }
        } else if (isNumero(carat)) { //Tamnbién es posible no especificar el signo
            Error = decLit();
            carat = siguienteCaracter();
        } else {                             //Si no hay nada relativo a un número después, está mal construido
            Error = 1;
        }
    }

    //2. si es una i, es un imaginario, si no un float
    if (carat == 'i') {
        insertarComLex(&comp, IMAGINARY_LIT);
        insertarLexema(&comp, siguienteBloque());
    } else {
        retroceder(1);
        insertarComLex(&comp, FLOAT_LIT);
        insertarLexema(&comp, siguienteBloque());
    }

}


/*---------Funciones axiliares--------*/
int isLetra(char caracter) {
    return ((65 <= caracter) && (caracter <= 90)) || ((97 <= caracter) && (caracter <= 122)) || (caracter == '_');
}

int isNumero(char caracter) {
    return (48 <= caracter) && (caracter <= 57); //Rango ascci de los número del 0 al 9
}

int isHexa(char caracter) {
    return isNumero(caracter) || ((65 <= caracter) && (caracter <= 70)) || ((97 <= caracter) && (caracter <= 102));
}

int isCaracterIgnorado(char caracter) {

    if (caracter == '\n') {
        linea++;
        return 1;
    }
    return caracter == ' ' || caracter == '\t';
}

int isString(char caracter) {
    return caracter == STRING_LIT || caracter == INTERPRETED_STRING_LIT;
}

int isComment(char caracter, int *tipo) {

    if (caracter == '/') {
        char sig = siguienteCaracter();
        if (sig == '*') {
            *tipo = 1;
            return 1;
        } else if (sig == '/') {
            *tipo = 2;
            return 1;
        } else {
            retroceder(1);
        }
    }
    return 0;
}

void insertCharSimp(char caract, compLexico comp) {
    ignorar();
    char *lexema;
    lexema = (char *) malloc(2 * sizeof(char));
    lexema[0] = caract;
    lexema[1] = '\0';
    insertarLexema(&comp, lexema);
    insertarComLex(&comp, caract);
}

char hexLit() {

    //Variables auxiliares
    char E = 0;
    char caracter, previo;

    //Mientras sigua pudiendo ser un dígito hexadecimal bien contruido
    do {
        caracter = siguienteCaracter();

        //Si no se sigue la sintaxis correctar para introdcuir "_" salgo
        if (caracter == '_') {
            previo = '_';
            caracter = siguienteCaracter();
        } else {
            previo = '\0';
        }

    } while (isHexa(caracter));

    //Si el último elemento es un "_" la construcción es incorrecta
    if (previo == '_') {
        E = 1;
    }
    retroceder(1);

    return E;
}

char decLit() {

    //Detecciónd e errores
    char E = 0;

    //Variables auxiliares
    char caracter, previo;


    //Mientras sigua pudiendo ser un dígito hexadecimal bien contruido
    do {

        caracter = siguienteCaracter();
        //Si no se sigue la sintaxis correctar para introdcuir "_" salgo
        if (caracter == '_') {
            previo = '_';
            caracter = siguienteCaracter();
        } else {
            previo = '\0';
        }

    } while (isNumero(caracter));


    //Si el último elemento es un "_" la construcción es incorrecta
    if (previo == '_') {
        E = 1;                                              //Indicamos que el componente está mal construido
    }
    retroceder(1);
    return E;
}

int isSimb(char caracter, int *a) {


    if ((40 <= caracter) && (caracter <= 47))                   //Caracteres: ( ) * + , - . /
    {                                                           //(Caracteres que se pueden introducir de golpe)

        if (caracter == 46) {                                   //Nos aseguramos de que "." es un operador
            caracter = siguienteCaracter();
            if (isNumero(caracter)) {
                return 0;
            }
            retroceder(1);
        }
        *a = 1;
        return 1;
    } else if ((58 <= caracter) && (caracter <= 61)) {          //Caracteres: : ; < =
        *a = 2;                                                 //(Caracteres secuenciales que pueden o no ser más de 1)
        return 1;
    } else if                                                   //Caracteres individuales no secuenciales
            (caracter == '[' || caracter == ']'
             || caracter == '}' || caracter == '{') {
        *a = 3;
        return 1;
    }
    return 0;
}








