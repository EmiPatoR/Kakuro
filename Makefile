CC := gcc
SOURCES := main.c structures.c input.c outputG.c csp2.c
INCLUDES := structures.h input.h OutputG.h csp2.h
LIBS := -lm `sdl-config --libs` -lSDL -lSDL_ttf -lSDL_image
CFLAGS := -g
OBJS := $(SOURCES:.c=.o)

all: programme

programme: $(OBJS)
	$(CC) $(CFLAGS) -o programme $(OBJS) $(LFLAGS) $(LIBS)

IA/.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

clean:
	rm IA/*.o
	rm IA/*.gch
