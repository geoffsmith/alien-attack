#include "planet.h"
#include "lib.h"
#include <iostream>
#include <time.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

using namespace std;

void Planet::render() {
    // Do the opengl rendering here
    glPushMatrix();

    // We use a quadric because it generates texture coords better
    GLUquadric *quad = gluNewQuadric();

    // Do texturing if we have it
    if (this->_texture) {
        glBindTexture(GL_TEXTURE_2D, this->_texture);
        gluQuadricTexture(quad, GL_TRUE);
    }

    gluQuadricNormals(quad, GL_TRUE);

    // Rotate the sphere depeding on current time
    glRotatef(this->_getRotation(), 0, 1, 0);

    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0, this->_radius + 50, 0);
    glVertex3f(0, this->_radius - 50, 0);
    glEnd();

    glRotatef(90, 1, 0, 0);

    gluSphere(quad, this->_radius, this->_sections, this->_sections);

    glPopMatrix();
}

Planet::Planet(int radius, float center_x, float center_y, float center_z) {
    this->_radius = radius;
    vector<float> center(3);
    center[0] = center_x;
    center[1] = center_y;
    center[2] = center_z;
    this->_center = center;

    // Sections in sphere
    this->_sections = 1000;
    this->_texturePath = "";
    this->_texture = NULL;

    // One minute per rotation
    this->_secondsPerRotation = 10;
}

float Planet::_getRotation() {
    time_t seconds;
    seconds = clock();
    float result = (int)seconds % (this->_secondsPerRotation * CLOCKS_PER_SEC);
    result = result / (this->_secondsPerRotation * CLOCKS_PER_SEC) * 360;
    result = 0;
    return result;
}

void Planet::setTexture(string path) {
    this->_texturePath = path;

    // load the texture
    glGenTextures(1, &(this->_texture));
    unsigned char* texture;
    load_texture(this->_texture, texture, this->_texturePath.c_str());
    // TODO: clean up the texture
}

Planet::~Planet() {}
