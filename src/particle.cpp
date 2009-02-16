#include "particle.h"
#include <math.h>
#include <iostream>

#define PI 3.14159265

void yRotationMatrix(GLfloat angle, GLfloat (*yRotation)[4]) {
    yRotation[0][0] = cos(angle);
    yRotation[0][1] = 0;
    yRotation[0][2] = sin(angle);
    yRotation[0][3] = 0;

    yRotation[1][0] = 0;
    yRotation[1][1] = 1;
    yRotation[1][2] = 0;
    yRotation[1][3] = 0;

    yRotation[2][0] = -1 * sin(angle);
    yRotation[2][1] = 0;
    yRotation[2][2] = cos(angle);
    yRotation[2][3] = 0;
    
    yRotation[3][0] = 0;
    yRotation[3][1] = 0;
    yRotation[3][2] = 0;
    yRotation[3][3] = 1;
}

void xRotationMatrix(GLfloat angle, GLfloat xRotation[][4]) {
    xRotation[0][0] = cos(angle);
    xRotation[0][1] = -1 * sin(angle);
    xRotation[0][2] = 0;
    xRotation[0][3] = 0;

    xRotation[1][0] = sin(angle);
    xRotation[1][1] = cos(angle);
    xRotation[1][2] = 0;
    xRotation[1][3] = 0;

    xRotation[2][0] = 0;
    xRotation[2][1] = 0;
    xRotation[2][2] = 1;
    xRotation[2][3] = 0;
    
    xRotation[3][0] = 0;
    xRotation[3][1] = 0;
    xRotation[3][2] = 0;
    xRotation[3][3] = 1;
}

void applyMatrixVertex(GLfloat (*matrix)[4], GLfloat vertex[]) {
    GLfloat tmp[4];
    for (int i = 0; i < 3; ++i) {
        tmp[i] = vertex[i];
    }

    GLfloat acc;
    for (int row = 0; row < 4; ++row) {
        acc = 0;
        for (int col = 0; col < 4; ++col) {
            acc += matrix[row][col] * tmp[col];
        }
        if (row < 3) vertex[row] = acc;
    }
}

void applyMatrixMatrix(GLfloat (*matrix1)[4], GLfloat (*matrix2)[4]) {
    GLfloat tmp[4][4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            tmp[i][j] = matrix2[i][j];
        }
    }

    GLfloat acc;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            acc = 0;
            for (int k = 0; k < 4; ++k) {
                acc += matrix1[i][k] * tmp[k][j];
            }
            matrix2[i][j] = acc;
        }
    }
}

void ParticleSystem::updatePosition(vector<GLfloat> center, vector<GLfloat> normal) {
    // Calculate the rotations needed to put new particles on the perpendicular
    // plane to the normal and translated to the center
    this->_normal = normal;
    this->_center = center;

    GLfloat xAngle, yAngle;

    // Y-angle is normal . (0, 1, 0)
    yAngle = acos(this->_normal[1]);

    // X-angle need to start by rotating by - yAngle
    GLfloat (*yRotation)[4] = new GLfloat[4][4];
    yRotationMatrix(-1 * yAngle, yRotation);
    GLfloat tmpNormal[3] = { this->_normal[0], this->_normal[1], this->_normal[2] };
    applyMatrixVertex(yRotation, tmpNormal);
    xAngle = acos(tmpNormal[0]);

    // Now we have the angles we create a matrix for the particle transforms
    this->_particleTransform = new GLfloat[4][4];
    xRotationMatrix(xAngle, (this->_particleTransform));
    yRotationMatrix(yAngle, yRotation);
    applyMatrixMatrix(yRotation, this->_particleTransform);
}

ParticleSystem::ParticleSystem(GLfloat radius, Player* player) {
    this->_radius = radius * 2000;
    this->_particleTTL = 10;
    this->_particleDelta = 0.01;
    this->_particleEmission = 40;
    this->_maxParticles = 1000;
    this->_startColors = new GLfloat[2][4];
    this->_startColors[0][0] = 0.837;
    this->_startColors[0][1] = 0.916;
    this->_startColors[0][2] = 0.351;
    this->_startColors[0][3] = 1;

    this->_startColors[1][0] = 0.486;
    this->_startColors[1][1] = 0.89;
    this->_startColors[1][2] = 0.988;
    this->_startColors[1][3] = 1;

    this->_multiplier = 200000;

    this->_player = player;
}


void ParticleSystem::render() {
    this->_updateParticles();

    // Render a globe for each particle
    glDisable(GL_LIGHTING);
    list< Particle * >::iterator it = this->_particles.begin();
    Particle *particle;
    glPushAttrib(GL_LIGHTING_BIT);
    glPushMatrix();

    // Transform into player coords
    glMultMatrixf(this->_player->getTransformationMatrix()->getMatrix());
    
    glBegin(GL_POINTS);
    for (; it != this->_particles.end(); ++it) {
        particle = *it;


        glColor4f(particle->color[0], particle->color[1], particle->color[2], particle->color[3]);
        glVertex3f(particle->coord[0], particle->coord[1], particle->coord[2]);

    }
    glEnd();

    glPopMatrix();
    glPopAttrib();

    glEnable(GL_LIGHTING);
}

void ParticleSystem::_emitParticles() {
    // Work out how many particles we want to emit
    unsigned int newParticles = this->_particleEmission;

    unsigned int currentParticles = this->_particles.size() - this->_particleRecycle.size();
    if (currentParticles + newParticles > this->_maxParticles) {
        newParticles = this->_maxParticles - currentParticles;
    }

    // Go through the recycle list first
    list< Particle * >::iterator it = this->_particleRecycle.begin();
    for (; it != this->_particleRecycle.end() && newParticles > 0; ++it) {
        this->_initParticle(*it);
        this->_particleRecycle.erase(it);
        --newParticles;
    }

    // Create any new ones if necessary
    for (unsigned int i = 0; i < newParticles; ++i) {
        Particle *newParticle = new Particle;
        this->_initParticle(newParticle);
        this->_particles.push_back(newParticle);
    }
}

void ParticleSystem::_initParticle(Particle *particle) {
    // The initial position should be somewhere random in the circle defined by
    // the center, radius and normal
    float angle = (rand() % 360) * PI / 180.0;
    float length = ( ( rand() % 100 ) / 100.0 ) * this->_radius;
    GLfloat point[3];
    point[0] = cos(angle) * length;
    point[1] = sin(angle) * length;
    point[2] = 0;

    // Apply the transformation
    //applyMatrixVertex(this->_particleTransform, point);
    
    // Assign new coord and add translation
    particle->coord[0] = point[0] + this->_center[0];
    particle->coord[1] = point[1] + this->_center[1];
    particle->coord[2] = point[2] + this->_center[2];

    // Reset the color
    int startColor = rand() % 2;
    particle->color[0] = this->_startColors[startColor][0];
    particle->color[1] = this->_startColors[startColor][1];
    particle->color[2] = this->_startColors[startColor][2];
    particle->color[3] = this->_startColors[startColor][3];

    // Reset the time
    particle->time = 0;

    // Set the current params of the player
    particle->rotation = this->_player->getRotation();
    particle->altitude = this->_player->getAltitude();
    particle->lateralDelta = this->_player->getLateralDelta();
    particle->sway = this->_player->getSway();
}

void ParticleSystem::_updateParticles() {
    // Emit particles
    this->_emitParticles();

    // Update the particles
    list<Particle *>::iterator it = this->_particles.begin();
    Particle *particle;
    GLfloat randomMultiplier;
    for(; it != this->_particles.end(); ++it) {
        // Check that the particle is not too old
        particle = *it;
        if (particle->time < this->_particleTTL) {
            // Increase the time
            particle->time += 1;

            randomMultiplier = (rand() % 100) / 100.0;
            // Move the particle one delta along the normal
            particle->coord[0] += randomMultiplier * this->_particleDelta * this->_normal[0] * this->_multiplier;
            particle->coord[1] += randomMultiplier * this->_particleDelta * this->_normal[1] * this->_multiplier;
            particle->coord[2] += randomMultiplier * this->_particleDelta * this->_normal[2] * this->_multiplier;

            // Decay the alpha so that it fades
            particle->color[3] -= 0.1;
        } else if (particle->time == this->_particleTTL) {
            particle->time += 1;
            this->_particleRecycle.push_back(particle);
        }
    }
}
