CC=gcc -Wall

LIBRERIAS= 

SALIDA=CompiladorGo

LIB_PUNTO_H= definiciones.h gestError.h ./lexico/anaLexico.h ./sintactico/anaSintac.h ./tablaSimbolos/tablaID.h ./tablaSimbolos/tablaKeyWords.h ./tablaSimbolos/tablaSimb.h ./tiposDatos/abb.h ./tiposDatos/compLex.h entradaSalida/entradaSalida.h

PUNTO_C= main.c gestError.c ./lexico/anaLexico.c ./sintactico/anaSintac.c ./tablaSimbolos/tablaID.c ./tablaSimbolos/tablaKeyWords.c ./tablaSimbolos/tablaSimb.c ./tiposDatos/abb.c ./tiposDatos/compLex.c entradaSalida/entradaSalida.c

OBJS= $(PUNTO_C:.c=.o)

$(SALIDA): $(OBJS)
	$(CC) -o $(SALIDA) $(OBJS) $(LIBRERIAS)

%.o: %.c $(LIB_PUNTO_H)
	$(CC) -c -o $@ $< $(-I)

cleanall: clean
	rm -f $(SALIDA)

clean:
	rm -f main.o gestError.o ./lexico/anaLexico.o ./sintactico/anaSintac.o ./tablaSimbolos/*.o ./tiposDatos/*.o entradaSalida/entradaSalida.o *.o *~

