TARGET = bin/alien-attack

LIBS = -lglut -lGLU -lGL -ljpeg -lboost_filesystem-mt -lboost_system-mt -lftgl -lfreetype
INCS = -I/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/include/GL/ -I/opt/local/include/GL -I/opt/local/include -L/opt/local/lib -L/Developer/SDKs/MacOSX10.4u.sdk/usr/X11R6/lib/ -I/Users/geoffsmith/usr/local/include/ -L/Users/geoffsmith/usr/local/lib -I/opt/local/include/freetype2/

CC = /usr/bin/g++

PROFILE =

default: $(TARGET)

all: default

$(TARGET): src/main.cpp planet.o  \
	jpeg.o lib.o player.o obj.o star.o shadow.o particle.o opponent.o collision.o projectiles.o particle_system.o matrix.o
	$(CC) $(PROFILE) -Wall $(INCS) $(LIBS) -o $(TARGET) src/main.cpp planet.o lib.o jpeg.o opponent.o player.o obj.o star.o shadow.o particle.o collision.o projectiles.o particle_system.o matrix.o

collision.o: src/collision.h src/collision.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o collision.o src/collision.cpp

player.o: src/player.h src/player.cpp src/renderable.h src/collision.h matrix.o src/opponent.h
	$(CC) $(PROFILE) -Wall $(INCS) -c -o player.o src/player.cpp

planet.o: src/planet.h src/planet.cpp src/renderable.h matrix.o
	$(CC) $(PROFILE) -Wall $(INCS) -c -o planet.o src/planet.cpp

obj.o: src/obj.h src/obj.cpp matrix.o
	$(CC) $(PROFILE) -Wall $(INCS) -c -o obj.o src/obj.cpp

particle.o: src/particle.h src/particle.cpp src/renderable.h
	$(CC) $(PROFILE) -Wall $(INCS) -c -o particle.o src/particle.cpp

opponent.o: src/opponent.h src/opponent.cpp src/renderable.h src/collision.h matrix.o src/player.h
	$(CC) $(PROFILE) -Wall $(INCS) -c -o opponent.o src/opponent.cpp

shadow.o: src/shadow.h src/shadow.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o shadow.o src/shadow.cpp

lib.o: src/lib.h src/lib.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o lib.o src/lib.cpp

star.o: src/star.h src/star.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o star.o src/star.cpp

jpeg.o: src/jpeg.h src/jpeg.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o jpeg.o src/jpeg.cpp

particle_system.o: src/particle_system.h src/particle_system.cpp collision.o
	$(CC) $(PROFILE) -Wall $(INCS) -c -o particle_system.o src/particle_system.cpp

projectiles.o: src/projectiles.h src/projectiles.cpp particle_system.o
	$(CC) $(PROFILE) -Wall $(INCS) -c -o projectiles.o src/projectiles.cpp

matrix.o: src/matrix.h src/matrix.cpp
	$(CC) $(PROFILE) -Wall $(INCS) -c -o matrix.o src/matrix.cpp

clean:
	-rm -f *.o $(TARGET)
