/*Abrimos el archivo que vamos a leer*/
void abrirArchivo(char* nomArchivo);

/*Obtenemos el siguiente caracter del archivo */
char siguienteCaracter();

/*Obtenemos la cadena de caracteres leída desde la última llamada a "siguienteBloque"*/
char* siguienteBloque();

/*Retrocedemos el número de caracteres indicado en el lector*/
void retroceder(int retroceso);

/*Cerramos el archivo del sistema de entrada*/
void cerrarArchivo();

/*Ignoramos caracteres que no vayamos a procesar (nuna superior al tamaño de un buffer)*/
void ignorar();