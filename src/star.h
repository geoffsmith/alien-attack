#pragma once

#include <list>
#include <vector>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "renderable.h"

#define NUM_STARS 200

using namespace std;

class StarGenerator : public Renderable {
    public:
        StarGenerator();
        ~StarGenerator()  {} ;
        virtual void render();
        virtual void renderShadow() {};
    private:
        list< vector<GLfloat> > _stars;
};
