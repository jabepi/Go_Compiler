
//Tipo de dato opaco
typedef struct comp * compLexico;

//Constructor
void constCompLex(compLexico *compLex);

//Destructor
void destruirComponente(compLexico *componente);

//Insertar caracter de lexema
void insertarLexema(compLexico *compLex, char* lexema);

//Definir componente léxico
void insertarComLex( compLexico *compLex, int comLex);

//Reiniciar componente léxico
void limpiarComponente(compLexico *componente);

//Obtener lexema
char* getLexema(compLexico* componente);

//Obtener componente léxico
int getCompLex(compLexico* componente);

