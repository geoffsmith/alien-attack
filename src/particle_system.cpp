#include "particle_system.h"
#include "lib.h"
#include "matrix.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

int compare(const void *a, const void *b) {
    float result = ( ((Particle2*)a)->z - ((Particle2*)b)->z);
    if (result > 0.0) return 1;
    else if (result < 0.0) return -1;
    else return 0;
}

ParticleSystem2::ParticleSystem2() {
    // Set up the variables
    this->_maxTicks = 20;
    this->_maxParticles = 400;
    this->_maxSpriteSize = 5;

    // Pre-allocate the particles
    this->_particles = new Particle2[this->_maxParticles];

    // Initialise all particles with the max ticks
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        this->_particles[i].ticks = this->_maxTicks;
    }

    // Default colors
    this->_nColors = 2;
    this->_startColors = new float[this->_nColors][4];
    this->_startColors[0][0] = 1;
    this->_startColors[0][1] = 1;
    this->_startColors[0][2] = 1;
    this->_startColors[0][3] = 1;

    this->_startColors[1][0] = 0.486;
    this->_startColors[1][1] = 0.89;
    this->_startColors[1][2] = 0.988;
    this->_startColors[1][3] = 1;

    // Load the particle sprite
    glGenTextures(1, &(this->spriteTexture));
    load_texture(this->spriteTexture, "resources/particle.jpg");
}

ParticleSystem2::~ParticleSystem2() {
    delete[] this->_particles;
}

void ParticleSystem2::_initParticle(Particle2 *particle) {
    // Reset the ticks
    particle->ticks = 0;

    // Set a random color
    // Reset the color
    int startColor = rand() % this->_nColors;
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


    glPushAttrib(GL_POINT_BIT);
    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_COLOR_BUFFER_BIT);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glBindTexture(GL_TEXTURE_2D, this->spriteTexture);


    // How the sprite will be modified by distance from viewer
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);

    // Query the max size supported by the hardware
    float maxSize = 0.0f;
    glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxSize);
    // Clamp to 100
    if (maxSize > this->_maxSpriteSize) maxSize = this->_maxSpriteSize;

    // Make the points bigger
    glPointSize(maxSize);

    // The alpha of a point is calculated to allow the fading of points 
    // instead of shrinking them past a defined threshold size. The threshold 
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
    // the minimum and maximum point sizes.
    glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );

    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, maxSize );

    // Specify point sprite texture coordinate replacement mode for each 
    // texture unit
    glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable(GL_POINT_SPRITE_ARB);

    // We need to sort the particles first
    qsort(this->_particles, this->_maxParticles, sizeof(Particle2), compare);

    glBegin(GL_POINTS);
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        // Make sure this particle is still valid
        if (this->_particles[i].ticks < this->_maxTicks) {
            // Get the position and color of this particle
            position = this->_particles[i].position;
            color = this->_particles[i].color;

            // Render the position
            glColor4f(color[0], color[1], color[2], color[3]);
            glVertex3f(position[0], position[1], position[2]);
        }
    }
    glEnd();

    glDisable(GL_POINT_SPRITE_ARB);

    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

void ParticleSystem2::emitParticles() {
    // calculate how many particles we want to create
    unsigned int nCreate = this->_maxParticles / this->_maxTicks;

    // Save ourselves referencing this in each iteration
    unsigned int maxTicks = this->_maxTicks;

    // go through the particles looking for particles which are expired
    for (unsigned int i = 0; i < this->_maxParticles && nCreate > 0; ++i) {
        if (this->_particles[i].ticks >= maxTicks) {
            this->_initParticle(&(this->_particles[i]));

            // Decrease ncreate so that we can perhaps finish early
            --nCreate;
        }
    }
}

void ParticleSystem2::tick() {
    Particle2* particles = this->_particles;

    // Increase the tick for each particle
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        if (particles[i].ticks < this->_maxTicks) {
            particles[i].ticks++;
        }
    }


    this->_updateParticles();

    this->_calculateZ();
}

void ParticleSystem2::_calculateZ() {
    // Load the current modelview matrix
    GLfloat *modelView = new GLfloat[16];
    float tmp[4];
    float result[4];
    tmp[3] = 1;

    glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
    Matrix matrix(modelView);

    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        if (this->_particles[i].ticks < this->_maxTicks) {
            tmp[0] = this->_particles[i].position[0];
            tmp[1] = this->_particles[i].position[1];
            tmp[2] = this->_particles[i].position[2];

            matrix.multiplyVector(tmp, result);

            this->_particles[i].z = result[2];
        }
    }
}


/******************************************************************************
 * Collision detection
 *****************************************************************************/
void ParticleSystem2::collisionDetect(Collidable *collidable) {
    // Check for a collision for each particle in each collidable
    float *bounds;
    bool boundsCheck = true;
    unsigned int maxTicks = this->_maxTicks;
    Particle2 *particle;

    bounds = collidable->getBounds();
    for (unsigned int i = 0; i < this->_maxParticles; ++i) {
        particle = &(this->_particles[i]);
        // Only check particle if it is alive
        if (particle->ticks < maxTicks) {
            boundsCheck = true;
            // Check if this particle falls in the bounds of the current object
            for (int j = 0; j < 3 && boundsCheck; ++j) {
                if (particle->position[j] < bounds[2 * j]) boundsCheck = false;
                if (particle->position[j] > bounds[2 * j + 1]) boundsCheck = false;
            }
            
            // If the boundsCheck is still valid, trigger a collision
            if (boundsCheck) {
                collidable->triggerCollision();
                return;
            }
        }
    }
}
