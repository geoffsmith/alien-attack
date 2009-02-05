#include "star.h"

#include <time.h>

StarGenerator::StarGenerator() {
    // Seed the random number generator
    srand(clock());

    // Generate MAX_STARS stars
    int angle;
    for (int i = 0; i < NUM_STARS; ++i) {
        // Generate rotations for the x and y axis
        vector< GLfloat > rotations;
        angle = rand() % 360;
        rotations.push_back(angle);
        angle = rand() % 360;
        rotations.push_back(angle);
        this->_stars.push_back(rotations);
    }
}

void StarGenerator::render() {
    list< vector< GLfloat > >::iterator it = this->_stars.begin();
    glColor3f(1, 1, 1);
    glPushAttrib(GL_LIGHTING_BIT);
    GLfloat color[] = { 1, 1, 1, 1 };
    GLfloat color2[] = { 1, 1, 1, 0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, color2);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    while ( it != this->_stars.end() ) {
        glPushMatrix();
        glRotatef((*it)[0], 1, 0, 0);
        glRotatef((*it)[1], 0, 1, 0);
        glTranslatef(30, 0, 0);
        glutSolidSphere(0.05, 10, 10);
        glPopMatrix();
        ++it;
    }
    glPopAttrib();
}
