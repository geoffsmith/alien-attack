#include "star.h"
#include "lib.h"
#include "matrix.h"

#include <time.h>
#include <iostream>

using namespace std;

StarGenerator::StarGenerator() {
    // Seed the random number generator
    srand(clock());
    Matrix matrix;
    float tmp[4];
    float result[4];
    tmp[0] = 0; tmp[1] = 0; tmp[2] = 0; tmp[3] = 1;

    // Generate MAX_STARS stars
    for (int i = 0; i < NUM_STARS; ++i) {
        vector<GLfloat> position;
        matrix.reset();
        matrix.rotateY(rand() % 360);
        matrix.rotateZ(rand() % 360);
        matrix.translate(30, 0, 0);
        matrix.multiplyVector(tmp, result);
        for (int j = 0; j < 3; ++j) position.push_back(result[j]);
        this->_stars.push_back(position);
    }

    glGenTextures(1, &(this->_spriteTexture));
    load_texture(this->_spriteTexture, "resources/particle.jpg");

    this->_spriteSize = 2.5;
}

void StarGenerator::render() {
    glPushAttrib(GL_POINT_BIT);
    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_COLOR_BUFFER_BIT);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );

	glBindTexture(GL_TEXTURE_2D, this->_spriteTexture);


    // How the sprite will be modified by distance from viewer
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    glPointParameterfvARB(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic);

    // Make the points bigger
    glPointSize(this->_spriteSize);

    // The alpha of a point is calculated to allow the fading of points 
    // instead of shrinking them past a defined threshold size. The threshold 
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
    // the minimum and maximum point sizes.
    glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );

    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, this->_spriteSize );

    // Specify point sprite texture coordinate replacement mode for each 
    // texture unit
    glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable(GL_POINT_SPRITE_ARB);

    list< vector< GLfloat > >::iterator it = this->_stars.begin();
    glColor4f(1, 1, 1, 1);

    glBegin(GL_POINTS);

    while ( it != this->_stars.end() ) {
        glVertex3f((*it)[0], (*it)[1], (*it)[2]);
        ++it;
    }


    glEnd();

    glDisable(GL_POINT_SPRITE_ARB);
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}
