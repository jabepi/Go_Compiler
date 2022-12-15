#include "entradaSalida.h"
#include "../gestError.h"
#include <stdio.h>
#include <string.h>
#include "stdlib.h"

#define TAMBLOCK 16

//Bloques del doble buffer y variables auxiliares
char bloqueAB[2 * (TAMBLOCK + 1)];              //Array con ambos buffer
int tamLexActual;                               //Registros del tamaño de lexema para trucarlo si es necesario
int primero;                                    //Puntero al primer elemento del lexema leido en un momento
int actual;                                     //Punter al elemento que se está leyendo en un momento dado
int pasoEOF;                                    //Flag que indica si se ha leído un EOF
char ultimaCarga;                               //Flag con el último bloque cargado

//Puntero al archivo abierto
FILE *fichero;

void abrirArchivo(char *nomArchivo) {

    fichero = fopen(nomArchivo, "r");

    if (fichero) {
        //Cargo el primer bloque
        bloqueAB[fread(bloqueAB, sizeof(char), TAMBLOCK, fichero)] = EOF;
        bloqueAB[TAMBLOCK] = EOF;
        bloqueAB[2 * TAMBLOCK + 1] = EOF;

        //Inicializamos los punteros y variables auxiliares
        primero = 0;
        actual = 0;
        tamLexActual = 0;

        //Indicamos que el último bloque cargado es el B para evitar recargas
        ultimaCarga = 'A';
        pasoEOF = -1;

    } else {
        errorLectura(nomArchivo);
        exit(1);
    }
}

//Devolvemos el siguiente caracter
char siguienteCaracter() {

    char caracter;
    tamLexActual++;
    int fin;

    //En caso de que se haya llegado a un EOF pueden darse 3 circunstancias
    if (bloqueAB[actual] == EOF) {

        //Si estoy al final del bloqueA y aún no he cargado el bloqueB
        if (actual == TAMBLOCK) {
            if (ultimaCarga != 'B') {

                //Modifico las variables de control
                ultimaCarga = 'B';              //Indico que se ha cargado el bloque B para no recargarlo al retroceder
                pasoEOF = 1;                    //Indico que se ha cambiado de buffer

                //Cargo el bloque B
                fin = (int) fread(bloqueAB + TAMBLOCK + 1,
                                  sizeof(char), TAMBLOCK, fichero);
                bloqueAB[(TAMBLOCK + 1) + fin] = EOF;

                //En caso de que se aplaste el contendio al que apuntaba primero, actualizamos el puntero
                if (primero > TAMBLOCK) {
                    primero = 0;
                } else if (primero == TAMBLOCK) {
                    primero = TAMBLOCK + 1;
                    pasoEOF = -1;
                }
            }

            //Devolvemos el caracter
            actual++;
            caracter = bloqueAB[actual];
            actual++;
            return caracter;
        }
            //Si estoy al final del bloqueB y aún no he cargado el bloqueA lo cargo
        else if (actual == 2 * TAMBLOCK + 1) {
            if (ultimaCarga != 'A') {

                //Modifico las variables de control
                ultimaCarga = 'A';              //Indico que se ha cargado el bloque a para no recargarlo al retroceder
                pasoEOF = 1;                    //Indico que se ha cambiado de buffer

                //Cargo el bloque A
                fin = (int) fread(bloqueAB,
                                  sizeof(char), TAMBLOCK, fichero);
                bloqueAB[fin] = EOF;

                //En caso de que se aplaste el contendio al que apuntaba primero, actualizamos el puntero
                if (primero <= TAMBLOCK) {
                    primero = TAMBLOCK + 1;
                } else if (bloqueAB[primero] == EOF) {
                    primero = 0;
                    pasoEOF = -1;
                }
            }

            //Vuelvo a empezar al principio del bloque A
            actual = 0;
            caracter = bloqueAB[actual];
            actual++;
            return caracter;
        }
            //Si estoy al final del fichero se lo indico al léxico
        else {
            actual++;
            return EOF;
        }
    }

    //Devolvemos el caracter de la posición actual e incrementamos
    caracter = bloqueAB[actual];
    actual++;
    return caracter;
}

//Enviamos el bloque de caracteres entre primero y actual
char *siguienteBloque() {

    char *cadena;
    int posCorte;

    //Si ambos están en el bloqueA o el bloqueB
    if (pasoEOF == -1) {

        //Copiamos el fragmento del buffer
        cadena = (char *) malloc(sizeof(char) * tamLexActual + 1);
        strncpy(cadena, &bloqueAB[primero], tamLexActual);
        cadena[tamLexActual] = '\0';
    }
    //Si se ha atravesado un EOF y los punteros NO están invertidos
    else if (actual - primero > 0) {

        //En caso de que se solicite un lexema mayor al tamaño de bloque lo truncamos
        if (tamLexActual > TAMBLOCK) {
            primero = actual - (TAMBLOCK + 1);
            tamLexActual = TAMBLOCK;
            errorOverFlow();
        }

        //Copiamos el fragmento del buffer
        posCorte = TAMBLOCK - primero;
        cadena = (char *) malloc(sizeof(char) * tamLexActual + 1);
        strncpy(cadena, &bloqueAB[primero], posCorte);
        strncpy(&cadena[posCorte], &bloqueAB[TAMBLOCK + 1], tamLexActual - posCorte);
        cadena[tamLexActual] = '\0';
        pasoEOF = -1;
    }
        //Si se ha atravesado un EOF y los punteros SÍ están innvertidos
    else {
        //Si es necesario truncamos
        if (tamLexActual > TAMBLOCK) {
            primero = (TAMBLOCK + 1) + actual;
            tamLexActual = TAMBLOCK;
            errorOverFlow();
        }
        posCorte = tamLexActual - actual;
        cadena = (char *) malloc(sizeof(char) * tamLexActual + 1);
        strncpy(cadena, &bloqueAB[primero], posCorte);
        strncpy(&cadena[posCorte], &bloqueAB[0], actual);
        cadena[tamLexActual] = '\0';
        pasoEOF = -1;
    }

    //Reiniciamos el puntero "primero" y la cuenta del tamaño del lexema
    primero = actual;
    tamLexActual = 0;
    return cadena;
}

//Ignorar un conjunto de caracteres en el buffer
void ignorar() {
    primero = actual;
    tamLexActual = 0;
    pasoEOF = -1;
}

//Retroceder n posiciones en el buffer (nunca superior al tamaño máximo del lexema)
void retroceder(int retroceso) {
    actual = actual - retroceso;
    tamLexActual--;
    if (actual < 0) {
        actual = 2 * (TAMBLOCK) + 1 + actual;
        pasoEOF = -1;
        tamLexActual = TAMBLOCK;
    } else if (actual == TAMBLOCK || actual == 2 * TAMBLOCK + 1) {
        actual--;
        pasoEOF = -12;
        tamLexActual = TAMBLOCK;
    }
}

void cerrarArchivo() {
    fclose(fichero);
}

