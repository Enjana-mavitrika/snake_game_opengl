CC = gcc

OPT = -Wall

GL = -lglut -lGLU -lGL

exe : main.c
	$(CC) $(OPT) main.c -o exe $(GL) -lm

clean :
	rm -f *~
