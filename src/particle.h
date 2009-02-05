#pragma once 

#include <vector>
#include <list>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "renderable.h"

using namespace std;

struct Particle {
    GLfloat coord[3];
    GLfloat color[4];
    GLfloat time;
};

class ParticleSystem : public Renderable {

    public:
        ParticleSystem(GLfloat radius);
        void updatePosition(vector<GLfloat> center, vector<GLfloat> normal);
        virtual void render();
        virtual void renderShadow() {};

    private:
        list<Particle *> _particles;
        vector<GLfloat> _center;
        vector<GLfloat> _normal;
        GLfloat _multiplier;
        GLfloat (*_particleTransform)[4];
        GLfloat _radius;
        GLuint _maxParticles;
        GLint _particleEmission;
        GLfloat _particleTTL;
        GLfloat _particleDelta;
        GLfloat (*_startColors)[4];
        vector<GLfloat> _endColor;

        // A list old particles to be recycled
        list< Particle * > _particleRecycle;

        /**
         * Create new particles. This might create new Particle instances or
         * recycle old ones with an expired TTL
         */
        void _emitParticles();

        /**
         * Initialise a new particle
         */
        void _initParticle(Particle *particle);

        /**
         * Update the particle age, position and colour
         */
        void _updateParticles();
};
