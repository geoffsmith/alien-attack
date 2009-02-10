TARGET = bin/alien-attack

LIBS = -lglut -lGLU -lGL -ljpeg -lboost_filesystem-mt -lboost_system-mt
INCS = -I/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/include/GL/ -I/opt/local/include/GL -I/opt/local/include -L/opt/local/lib -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/ -I/Users/geoffsmith/usr/local/include/ -L/Users/geoffsmith/usr/local/lib

CC = /usr/bin/g++

PROFILE =

default: $(TARGET)

all: default

$(TARGET): src/main.cpp src/renderable.h planet.o  \
	src/jpeg.h src/jpeg.cpp src/lib.h src/lib.cpp player.o \
	obj.o src/star.h src/star.cpp src/shadow.h src/shadow.cpp \
	src/particle.h src/particle.cpp src/opponent.h src/opponent.cpp collision.o
	$(CC) $(PROFILE) -Wall $(INCS) $(LIBS) -o $(TARGET) src/main.cpp planet.o src/lib.cpp src/jpeg.cpp player.o obj.o src/star.cpp src/shadow.cpp src/particle.cpp src/opponent.cpp collision.o

collision.o: src/collision.h src/collision.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o collision.o src/collision.cpp

player.o: src/player.h src/player.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o player.o src/player.cpp

planet.o: src/planet.h src/planet.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o planet.o src/planet.cpp

obj.o: src/obj.h src/obj.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o obj.o src/obj.cpp

clean:
	-rm -f *.o $(TARGET)
