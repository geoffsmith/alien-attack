#include "planet.h"
#include "lib.h"
#include <iostream>
#include <time.h>
#include <math.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#define PI 3.14159265

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

    glRotatef(90, 1, 0, 0);

    gluSphere(quad, this->_radius, this->_sections, this->_sections);
    gluDeleteQuadric(quad);

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

void Planet::renderShadow() {

    float delta = 9.0;
    float bottom = 20;
    float radiusOffset = 4;
    float side, radians, sideAngle, sideAngleRadians;
    float x, y;

    glColor4f(1, 1, 1, 1);

    // calculate how wide the side needs to be
    radians = delta * PI / 180.0;
    sideAngle = (180.0 - delta) / 2.0;
    sideAngleRadians = sideAngle * PI / 180;
    side = this->_radius * sin(radians);
    side = side / sin(sideAngleRadians);

    // sides
    for (int i = 0; i < 360; i += delta) {
        glPushMatrix();
        glRotatef(i, 0, 0, 1);
        glTranslatef(this->_radius, 0, 0);
        glRotatef(-1 * (90 - sideAngle), 0, 0, -1);
        glNormal3f(-1, 0, 0);
        glBegin(GL_POLYGON);
        glVertex3f(0, 0, -1 * bottom);
        glVertex3f(0, side, -1 * bottom);
        glVertex3f(0, side, -1 * this->_radius + radiusOffset);
        glVertex3f(0, 0, -1 * this->_radius + radiusOffset);
        glEnd();
        glPopMatrix();
    }

    // top
    glPushMatrix();
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, -1);
    for (int i = 0; i < 360; i += delta) {
        radians = i * PI / 180;
        x = this->_radius * sin(radians);
        y = this->_radius * cos(radians);

        glVertex3f(x, y, -1 * bottom);
    }
    glEnd();
    glPopMatrix();

    // bottom
    glPushMatrix();
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    for (int i = 0; i < 360; i += delta) {
        radians = i * PI / 180;
        x = this->_radius * sin(radians);
        y = this->_radius * cos(radians);

        glVertex3f(x, y, -1 * this->_radius + radiusOffset);
    }
    glEnd();
    glPopMatrix();
}
