proj:			assembler.o functions.o pre_processor.o first_pass.o second_pass.o
	gcc -g -ansi -pedantic -Wall assembler.o functions.o pre_processor.o first_pass.o second_pass.o -o proj
assembler.o:	assembler.c header.h
	gcc -g -c -ansi -pedantic -Wall assembler.c -o assembler.o
functions.o:	functions.c header.h
	gcc -g -c -ansi -pedantic -Wall functions.c -o functions.o
pre_processor.o:	pre_processor.c header.h pre_processor.h
	gcc -g -c -ansi -pedantic -Wall pre_processor.c -o pre_processor.o
first_pass.o:	first_pass.c header.h first_pass.h
	gcc -g -c -ansi -pedantic -Wall first_pass.c -o first_pass.o
second_pass.o:	second_pass.c header.h second_pass.h
	gcc -g -c -ansi -pedantic -Wall second_pass.c -o second_pass.o

