#pragma once 

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "collision.h"

class Collidable;

/*
 * The paticle's position is defined by the same parameters as the ship
 */
struct Particle2 {
    float position[3];
    float vector[3];
    float color[4];
    float z;
    unsigned int ticks;
};

/**
 */

class ParticleSystem2 {

    public:
        ParticleSystem2();
        virtual ~ParticleSystem2();
        void tick();
        virtual void render();

        // Create new particles / recycle dead ones
        void emitParticles();

        // Do some collision detection
        void collisionDetect(Collidable *collidable);

    protected:
        Particle2 *_particles;

        // Initialise a new particle
        void _initParticle(Particle2 *particle);

        // This method is used to randomise the start parameters of a particle
        virtual void _setStartPosition(Particle2 *particle) = 0;

        // This method should be overridden to update the position of the particle
        virtual void _updateParticles() = 0;

        virtual void _calculateZ();

        // The lifetime of a particle in terms of number of ticks
        unsigned int _maxTicks;

        // Maxiumum size of a particle sprite
        float _maxSpriteSize;

        // The max number of particles. There shouldn't ever be more than this at any point.
        // The number of particles to emit is this number divided by the number of ticks 
        // before expiry
        unsigned int _maxParticles;

        // The colours used randomly for the particles, the alpha will be decayed based on the
        // ticks of the particle and _maxTicks
        float (*_startColors)[4];
        unsigned int _nColors;

        // For the particle texture
        GLuint spriteTexture;
};
