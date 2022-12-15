/* Para ahorrar condicionales en la medida de lo posible los código coinciden con los
 * código ascii de los componentes léxicos que representan (si es un único caracter).
 * En caso de que representen condiciones más complejas que un único caracter, se
 * utiliza un código de tres cifras que comienza por 4*/

#define ID 401

#define FIN_FICHERO -1


//------KEYWORDS------
#define PACKAGE 405
#define IMPORT 406
#define FUNC 407
#define CHAN 408
#define FOR 409
#define GO 410
#define VAR 411
#define RANGE 412


//------LITERALES------

//Literales enteros
#define INT_LIT 413

//Literales de punto flotante
#define FLOAT_LIT 416

//Literales imaginarios
#define IMAGINARY_LIT 418

//Literales strings
#define STRING_LIT 96
#define INTERPRETED_STRING_LIT 34


//------OPERADORES Y PUNTUACIÓN------
#define PARENTESISI 40
#define PARENTESISD 41
#define POR 42
#define MAS 43
#define COMA 44
#define MENOS 45
#define PUNTO 46
#define BACKSLASH 47

#define DOSPUNTOS 58
#define SEMICOLON 59
#define IGUAL 61

#define CORCHETEI 91
#define CORCHETED 93

#define LLAVEI 123
#define LLAVED 125

#define MENOR_MENOS 420
#define MASIGUAL 421
#define DOSPUNTOS_IGUAL 422












