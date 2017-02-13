OBJECTS=glm.o gltx.o
CC=cc
FLAGS=-g -std=c99 -Wall

cube : cube.c $(OBJECTS)
	$(CC) $(FLAGS) -o cube cube.c $(OBJECTS)
glm.o : glm.c glm.h
	$(CC) $(FLAGS) -c glm.c
gltx.o : gltx.c gltx.h
	$(CC) $(FLAGS) -c gltx.c
clean:
	rm la $(OBJECTS)
memcheck:
	./la valgrind