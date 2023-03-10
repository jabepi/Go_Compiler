# GoCompiler

El código se desarrolló como primera aproximación de un compilador de Go en el que seraliza un análisis 
léxico del lenguaje.

A continuación se explican los detalles más importantes de la organización y estructuras interna
así como estructuras de datos empleadas para el desarrollo del compilador


## Ejecución
	-Para la compilación del código, dentro de la carpeta "codigo", ejecutar en una terminal el comando make, 
	-Para eliminar todos los elementos generados de la compilación excepto el propio binario ejecutar "make clean"
	-Para eliminar todos los elementos generados de la compilación ejecutar "make cleanall"

	-Para ejecutar el compilador, se ejecuta "./CompiladorGo concurrentSum.go" (o el nombre del archivo que contenga
	el código .go que se quiera analizar).


## Estructura del código
Tabla de símbolos:
	El uso de una única tabla hash tiene los mejores resultandos en cuanto a velocidad de búsqueda se refiere
	sin embargo, muestra problemas en cuanto a escalabilidad debido a la gran cantidad de memoria que emplea.
	En cambio los árboles binarios tienen búsquedas menos veloces pero muestran menores costes espaciales.
	
	Es por ello que la tabla de símbolos se crea como una estructura híbrida. Las palabras clave, están acotadas 
	por la especificación del lenguaje, es por ello que se almacenarán en una tabla hash de tamaño fijo para 
	almacenarlas.
	
	En cambio los identificadores, crecen en función del programa por lo que se empleará un árbol binario de
	búsqueda, para almacenarlos y evitar el crecimiento elevado de emplear una hash.

	Tabla de palabras clave:
		1. Las palabras son introducidas directamente en un array en las funciones locales, para su inserción, 
		borrado e impresión. Se triplica esta variable en 3 ocasiones de forma local en las funciones, pese a que pueda 
		generar errores de programación porque es la solución con menor coste computacional y espacial. 
		Si se utilizara un array global, se mantendría un duplicado de las mismas palabras clave durante 
		toda la ejecución del compilador, cuando estas solo son necesarias en las funciones de iniciar y borrar.
		Otra solución sería, en el borrado e impresión, recorrer la tabla entera, pero dado que se sabe de antemano cuales son 
		las palabras clave del lenguaje, parece más lógico dirigirse a la posición de estas y eliminarlas directamente
		siendo el coste lineal con el número de palabras clave y no con el tamaño de la tabla has empleada. 
		Así, es por ello que se introducen directamente las palabras, de forma local en cada función, dado que 
		estas tienen una mayor variabilidad que, por ejemplo los IDs que dependen de la ejecución y no del propio
		lenguaje.
		2. Se utiliza un array de punteros a struct para determinar el lugar de los elementos en la tabla, dado que estos 
		tienen un peso de 8 bytes, frente a los 16 de las propia estructura, de esta forma, dado que el tamaño de la tabla
		es mucho mayor (para evitar colisiones) al numero de palabras clave, se gasta más memoria por palabra clave almacenada,
		pero se reduce a la mitad la empleada por hueco vacío, mejorando el coste de memoria global.

	Tabla de ID:
		1. Almacena todos los identificadores del programa, en una estructura tipo árbol binario, que posteriormente se extenderá 
		a bosque cuando se introduzcan los niveles. Actualmente, está definido como un bosque de un solo bloque. Se define como un 
		árbol, debido al equilibro que presenta entre gasto de memoria y velocidad de búsqueda (en caso de estar bien equilibrado) 
		frente a la variabilidad de número de identificadores de los que puede disponer un programa cualquier programa. 
		2. El tipo de dato de cada nodo del árbol es una estructura. Pese a que en esta primera práctica solo se va a almacenar el 
		lexema y el mismo componente léxico (ID), permite mayor flexibilidad para añadir los posibles atributos, línea, tipo... en 
		versiones futuras.

	TablaSimb.c: esta clase contienen la unión de ambas implementando, de forma que la estructura híbrida sea opaca de cara a las distintas
	etapas con el fin de facilitar la implementación


Recuento de líneas:
	Técnicamente, lo más lógico sería que fuese el sistema de entrada el que nos informase en qué linea nos encontramos, sin embargo eso implicaría 
	comprobarlo cada vez que se obtiene un caracter, lo que supone un gasto de recursos innecesario, por ello la cuenta se lleva a cabo en el 
	analizador léxico propiamente, dentro de la detección de caracteres ignorados, de forma que se comprobará al principio de la obtención
	del siguiente componente léxico y no en la obtención de cada caracter.

Analizador léxico:
	Se estructura con una única función pública (siguienteComponente), esta función actua como un autómata que decido qué tipo de componente léxico
	se está formado en función de su primer caracter, redireccionando la detección al subautómata correspondiente. También se encarga de la detección
	de errores léxicos

Errores léxicos:
	-Si el programa entró en un autómata, implica que ese autómata es el único que podía aceptar el primer caracter de ese elemento, por ello en caso de que 
	se detecte un error de construcción como, por ejemplo escribir el número 12e (sin contenido en el exponente), el programa 
	indicará que se detectó un error léxico, la línea del archivo donde se produjo y el componente léxico que "cree" que se quería formar
	en función del último autómata que procesó la cadena.
	
	-Si se detecta un caracter que no puede ser procesado por ningún autómata, se imprime por pantalla su valor ascii junto con la línea en la que 
	se encontraba.

Errores sistema de entrada:
	-Si el lexema des demasiado grande, se devuelven el últimos caracteres correspondientes con el tamaño máximo permitido. Y se indica que 
	que se ha truncado el lexema por consola
	
	-Si no se ha podido localizar el archivo de entrada se muestra un error por pantalla.
	

Entrada Salida:
	En el sistema de entrada/salida existe una macro superior que permite modificar la longitud del buffer, por defecto se encuentra a 16bytes. En 
	caso de que el sistema de entrada detecte un tamaño de cadena solicitado por el analizador superior al máximo, este la truncará a los últimos 
	caracteres con tamaño máximo, el tamaño de buffer definido.


**El resto de aspectos a destacar quedan explicitados en el propio código**
	


	 
