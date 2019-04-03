CC = g++
CFLAGS = -Wall -g

#LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm
#lxi and lXmu were removed since on local system they are not required
#and in fact cause build errors.

LIBRARIES = -lglut -lGL -lGLU 

All:	robotGame

robotGame:	robotGame.o
	$(CC) $(CFLAGS) robotGame.o -o robotGame $(LIBRARIES)

robotGame.o:	robotGame.c
		$(CC) $(CFLAGS) -c robotGame.c

clean:
		$(RM) *.o *~ robotGame
