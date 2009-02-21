#include "exhaust.h"
#include <iostream>

using namespace std;

Exhaust::Exhaust(Player *player, unsigned int vertex) { 
    this->_player = player;
    this->_vertex = vertex;

    this->_maxSpriteSize = 1.5;
    this->_maxTicks = 3;
}

void Exhaust::_setStartPosition(Particle2 *particle) {
    float position[4];
    float normal[4];

    // Get the current gun position
    this->_player->getExhaustPosition(this->_vertex, position, normal);

    // Set this as the current position, randomising a little
    float spread = 7000.0;
    particle->position[0] = position[0] + ((rand() % 200) - 100) / spread;
    particle->position[1] = position[1] + ((rand() % 200) - 100) / spread;
    particle->position[2] = position[2] + ((rand() % 200) - 100) / spread;

    // Set the vector
    particle->vector[0] = normal[0];
    particle->vector[1] = normal[1];
    particle->vector[2] = normal[2];

    // Add a random movement in the vector direction to simulate a continuous trail
    float scale = (rand() % 100) * 40;
    particle->position[0] += particle->vector[0] * scale;
    particle->position[1] += particle->vector[1] * scale;
    particle->position[2] += particle->vector[2] * scale;
}

void Exhaust::_updateParticles() {
    // Add the vector to the current position
    // TODO: this should probably be the default behaviour
    float *position;
    float *vector;
    float scale = 0.00000001;

    float alphaDelta = 1.0 / this->_maxTicks;
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        if (this->_particles[i].ticks < this->_maxTicks) {
            position = this->_particles[i].position;
            vector = this->_particles[i].vector;
            position[0] += vector[0] * scale;
            position[1] += vector[1] * scale;
            position[2] += vector[2] * scale;

            // decay the alpha
            this->_particles[i].color[3] -= alphaDelta;
        }
    }


}
