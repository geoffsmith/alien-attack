#include "particle_system.h"
#include <stdlib.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

ParticleSystem2::ParticleSystem2() {
    // Set up the variables
    this->_maxTicks = 100;
    this->_maxParticles = 1000;

    // Pre-allocate the particles
    this->_particles = new Particle2[this->_maxParticles];

    // Default colors
    this->_startColors = new float[2][4];
    this->_startColors[0][0] = 0.837;
    this->_startColors[0][1] = 0.916;
    this->_startColors[0][2] = 0.351;
    this->_startColors[0][3] = 1;

    this->_startColors[1][0] = 0.486;
    this->_startColors[1][1] = 0.89;
    this->_startColors[1][2] = 0.988;
    this->_startColors[1][3] = 1;
}

ParticleSystem2::~ParticleSystem2() {
    delete[] this->_particles;
}

void ParticleSystem2::_initParticle(Particle2 *particle) {
    // Reset the ticks
    particle->ticks = 0;

    // Set a random color
    // Reset the color
    int startColor = rand() % 2;
    particle->color[0] = this->_startColors[startColor][0];
    particle->color[1] = this->_startColors[startColor][1];
    particle->color[2] = this->_startColors[startColor][2];
    particle->color[3] = this->_startColors[startColor][3];

    this->_setStartPosition(particle);
}

void ParticleSystem2::render() {
    // Render each particle
    float *position;
    float *color;
    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        // Get the position and color of this particle
        position = this->_particles[i].position;
        color = this->_particles[i].color;

        // Render the position
        glColor4f(color[0], color[1], color[2], color[3]);
        glVertex3f(position[0], position[1], position[2]);
    }
    glEnd();
}

void ParticleSystem2::emitParticles() {
    // calculate how many particles we want to create
    unsigned int nCreate = this->_maxParticles / this->_maxTicks;

    // Save ourselves referencing this in each iteration
    unsigned int maxTicks = this->_maxTicks;

    // go through the particles looking for particles which are expired
    for (unsigned int i = 0; i < this->_maxParticles && nCreate > 0; ++i) {
        if (this->_particles[i].ticks > maxTicks) {
            this->_initParticle(&(this->_particles[i]));

            // Decrease ncreate so that we can perhaps finish early
            --nCreate;
        }
    }
}

void ParticleSystem2::tick() {
    Particle2* particles = this->_particles;
    // Increase the tick for each particle
    for (unsigned int i = 0; i < this->_maxTicks; ++i) {
        particles[i].ticks++;
    }
}
