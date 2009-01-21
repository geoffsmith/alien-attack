TARGET = bin/alien-attack

LIBS = -lglut -lGLU -lGL -ljpeg
INCS = -I/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/include/GL/ -I/opt/local/include/GL -I/opt/local/include -L/opt/local/lib -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/

CC = /usr/bin/g++

default: $(TARGET)

all: default

$(TARGET): src/main.cpp src/planet.cpp src/renderable.h src/planet.h \
	src/jpeg.h src/jpeg.cpp src/lib.h src/lib.cpp
	$(CC) -Wall $(INCS) $(LIBS) -o $(TARGET) src/main.cpp src/planet.cpp src/lib.cpp src/jpeg.cpp


clean:
	-rm -f *.o $(TARGET)
