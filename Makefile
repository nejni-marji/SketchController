CC = g++
CFLAGS = -g -Wall -pthread
INCLUDES = -I/usr/include/SDL2 -I/usr/include/X11 -I/usr/include/X11/extensions/
LIBS = -lSDL2 -lX11 -lXtst
TARGET = sketchcontroller

all:
	make clean
	make $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) $(TARGET).cpp $(INCLUDES) $(LIBS) -o $(TARGET)

clean:
	$(RM) $(TARGET)
