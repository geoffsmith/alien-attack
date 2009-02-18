#include "projectiles.h"
#include <iostream>

using namespace std;

Projectiles::Projectiles(Player *player) {
    this->_player = player;

    // Calculate the rotation matrix
    this->_rotationMatrix = new Matrix;
    this->_rotationMatrix->rotateY(-3);

    // Delete default colors
    delete[] this->_startColors;

    // Set up some colors
    this->_nColors = 1;
    this->_startColors = new float[this->_nColors][4];
    this->_startColors[0][0] = 225;
    this->_startColors[0][1] = 0;
    this->_startColors[0][2] = 0;
    this->_startColors[0][3] = 1;
}

void Projectiles::_setStartPosition(Particle2 *particle) {
    // Get the current gun position
    float *gun = this->_player->getGunPosition();

    // Set this as the current position, randomising a little
    float spread = 3000.0;
    particle->position[0] = gun[0] + (rand() % 200) / spread - spread / 100000.0;
    particle->position[1] = gun[1] + (rand() % 200) / spread - spread / 100000.0;
    particle->position[2] = gun[2] + (rand() % 200) / spread - spread / 100000.0;

    delete[] gun;
}

void Projectiles::_updateParticles() {
    float tmp[4];
    float result[4];
    float *position;

    // Rotate every particle around the Y-axis a single step
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        position = this->_particles[i].position;

        tmp[0] = position[0];
        tmp[1] = position[1];
        tmp[2] = position[2];
        tmp[3] = 1;

        this->_rotationMatrix->multiplyVector(tmp, result);

        position[0] = result[0];
        position[1] = result[1];
        position[2] = result[2];
    }
}
