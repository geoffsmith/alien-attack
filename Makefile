TARGET = bin/alien-attack

LIBS = -lglut -lGLU -lGL -ljpeg -lboost_filesystem-mt -lboost_system-mt
INCS = -I/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/include/GL/ -I/opt/local/include/GL -I/opt/local/include -L/opt/local/lib -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/ -I/Users/geoffsmith/usr/local/include/ -L/Users/geoffsmith/usr/local/lib

CC = /usr/bin/g++

default: $(TARGET)

all: default

$(TARGET): src/main.cpp src/planet.cpp src/renderable.h src/planet.h \
	src/jpeg.h src/jpeg.cpp src/lib.h src/lib.cpp src/player.h src/player.cpp \
	src/obj.h src/obj.cpp src/star.h src/star.cpp src/shadow.h src/shadow.cpp \
	src/particle.h src/particle.cpp src/opponent.h src/opponent.cpp
	$(CC) -Wall $(INCS) $(LIBS) -o $(TARGET) src/main.cpp src/planet.cpp src/lib.cpp src/jpeg.cpp src/player.cpp src/obj.cpp src/star.cpp src/shadow.cpp src/particle.cpp src/opponent.cpp


clean:
	-rm -f *.o $(TARGET)
