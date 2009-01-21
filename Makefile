TARGET = bin/alien-attack

LIBS = -lglut -lGLU -lGL -ljpeg
INCS = -I/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/include/GL/ -I/opt/local/include/GL -I/opt/local/include -L/opt/local/lib -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/

CC = /usr/bin/g++

default: $(TARGET)

all: default

$(TARGET): 
	$(CC) -Wall $(INCS) $(LIBS) -o $(TARGET) src/main.cpp src/planet.cpp

clean:
	-rm -f *.o $(TARGET)
