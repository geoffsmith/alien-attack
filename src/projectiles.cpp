#include "projectiles.h"
#include <iostream>

using namespace std;

Projectiles::Projectiles(Player *player) {
    this->_player = player;
}

void Projectiles::_setStartPosition(Particle2 *particle) {
    // Get the current gun position
    float *gun = this->_player->getGunPosition();

    // Set this as the current position
    cout << "Gun 0: " << gun[0] << endl;
    particle->position[0] = gun[0];
    particle->position[1] = gun[1];
    particle->position[2] = gun[2];

    delete[] gun;
}

void Projectiles::_updateParticles() {
    float tmp[4];
    float result[4];
    float *position;
    float *rotationMatrix = new float[16];

    buildYRotationMatrix(rotationMatrix, 5);

    // Rotate every particle around the Y-axis a single step
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        position = this->_particles[i].position;

        tmp[0] = position[0];
        tmp[1] = position[1];
        tmp[2] = position[2];
        tmp[3] = 1;

        matrixMultiply(rotationMatrix, tmp, result);

        position[0] = result[0];
        position[1] = result[1];
        position[2] = result[2];
    }
}
