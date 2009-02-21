#include "player.h"
#include "obj.h"
#include "opponent.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include <math.h>
#include <iostream>

#define PI 3.14159265

using namespace std;

GLfloat Player::scale = 0.00002;
Player *Player::player = NULL;

Player::Player(float altitude) {
    this->_altitude = altitude;
    this->_rotation = 0;
    this->_lateralDelta = 0;

    Obj* obj = Obj::makeObj("resources/fighter/SFighter.obj");
    this->_model = obj;

    // Set up the sway
    this->_swayDelta = 0.05;
    this->_maxSway = 2;
    this->_sway = 0;

    // Set up the particle system
    Exhaust *exhaust = new Exhaust(this, 1019);
    this->_exhauseParticleSystems.push_back(exhaust);

    exhaust = new Exhaust(this, 2496);
    this->_exhauseParticleSystems.push_back(exhaust);

    // And the gun particle system
    this->_gunParticleSystem = new Projectiles(this);

    this->_modelViewMatrix = new float[16];
    this->_transformationMatrix = new Matrix();

    // Set up a default of 3 lives
    this->_lives = 3;

    // The start movement deltas
    this->_moveRight = false;
    this->_moveLeft = false;
    this->_movementDelta = 0.5;

    // Load the light flare sprite
    glGenTextures(1, &(this->_spriteTexture));
    load_texture(this->_spriteTexture, "resources/particle.jpg");

    // Load the fire sound
    this->_fireSound = Mix_LoadWAV("resources/fire.wav");
    this->_engineSound = Mix_LoadWAV("resources/engine.wav");
    if (this->_engineSound == NULL) {
        cout << "Error loading fire.wav" << endl;
        cout << "There was an error playing this sound: " << Mix_GetError() << endl;
    }

    Mix_PlayChannel(-1, this->_engineSound, -1);
}

void Player::moveLaterally() {
    if (this->_moveLeft) this->_lateralDelta -= this->_movementDelta;
    if (this->_moveRight) this->_lateralDelta += this->_movementDelta;
}

void Player::moveForward() {
    this->_rotation += 0.5;
    if (this->_rotation >= 360) this->_rotation -= 360;

    // Increment the sway
    this->_sway += this->_swayDelta;
    //if (this->_sway > this->_maxSway) this->_swayDelta * -1;

    // Tick the particle systems
    this->_gunParticleSystem->tick();
    list< Exhaust * >::iterator it = this->_exhauseParticleSystems.begin();
    for (; it != this->_exhauseParticleSystems.end(); ++it) {
        (*it)->tick();
        (*it)->emitParticles();
    }

}

void Player::setCamera() {
    // Rotate the camera up
    glRotatef(-45, 1, 0, 0);
    //glRotatef(-90, 1, 0, 0);

    // Get the position of the camera
    glTranslatef(0, 0, -1 * (this->_altitude));

    // Move right / left
    glRotatef(-1 * this->_lateralDelta * 2 / 3, 0, 1, 0);

    // Move the camera up
    glRotatef(-90, 0, 0, 1);

    // Move the camera for the movement along
    glRotatef(this->_rotation - 2, 0, 1, 0);
}

void Player::render() {

    // Move the model to the right place
    glPushMatrix();

    Player::transformPlayer(this->_rotation, this->_sway, this->_lateralDelta, this->_altitude);
    glMultMatrixf(this->_transformationMatrix->getMatrix());

    // Save the transformation matrix
    glGetFloatv(GL_MODELVIEW_MATRIX, this->_modelViewMatrix);

    this->_model->render();
    this->_renderLights();
    glPopMatrix();

    // Render particle systems
    this->_gunParticleSystem->render();

    list< Exhaust * >::iterator it = this->_exhauseParticleSystems.begin();
    for (; it != this->_exhauseParticleSystems.end(); ++it) {
        (*it)->render();
    }

    // Update the bounds
    this->_model->calculateBounds(this->_transformationMatrix, this->_bounds);
}

void Player::transformPlayer(GLfloat rotation, GLfloat sway, GLfloat lateralDelta, GLfloat altitude) {

    // Reset the transformation matrix to the identity matrix
    this->_transformationMatrix->reset();

    // Rotate the craft around the planet
    this->_transformationMatrix->rotateY(-1.0 * rotation - 7.0);

    // Sway the craft
    float _sway = sin(sway - 1) / 2;
    this->_transformationMatrix->rotateX(-1 * lateralDelta + _sway);

    // Move the craft to altitude
    this->_transformationMatrix->translate(0, 0, altitude - 1.7);

    // Get the craft to point the right way
    this->_transformationMatrix->rotateX(90);
    this->_transformationMatrix->rotateY(-90);

    // Scale the model down
    this->_transformationMatrix->scale(Player::scale);
}

void Player::_renderLights() {
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
    glPointSize(15);

    // The alpha of a point is calculated to allow the fading of points 
    // instead of shrinking them past a defined threshold size. The threshold 
    // is defined by GL_POINT_FADE_THRESHOLD_SIZE_ARB and is not clamped to 
    // the minimum and maximum point sizes.
    glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );

    glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, 2 );

    // Specify point sprite texture coordinate replacement mode for each 
    // texture unit
    glTexEnvf(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    glEnable(GL_POINT_SPRITE_ARB);

    glBegin(GL_POINTS);

    vector<GLfloat> lightVertex = this->_model->getVertex(1381);
    glColor4f(1, 0, 0, 1);
    glVertex3f(lightVertex[0], lightVertex[1], lightVertex[2]);

    lightVertex = this->_model->getVertex(2811);
    glColor4f(0, 1, 0, 1);
    glVertex3f(lightVertex[0], lightVertex[1], lightVertex[2]);
    glEnd();

    glDisable(GL_POINT_SPRITE_ARB);
    glPopAttrib();
    glPopAttrib();
    glPopAttrib();
}

float Player::getRotationRad() {
    return this->_rotation * PI / 180;
}

float Player::getRotation() {
    return this->_rotation;
}

float Player::getAltitude() {
    return this->_altitude;
}

float Player::getLateralDelta() {
    return this->_lateralDelta;
}

float Player::getSway() {
    return this->_sway;
}

Matrix* Player::getTransformationMatrix() {
    return this->_transformationMatrix;
}

void Player::checkOpponentCollision() {
    // Pass all the opponents to the gun particle system
    list<Opponent *>::iterator it = Opponent::opponents.begin();
    for (; it != Opponent::opponents.end(); ++it) {
        if (this->checkCollision(*it)) {
            // Lose a life if we collide with a missile
            this->_lives--;
            // TODO: lose game when this reaches 0

            // Remove the missile
            (*it)->remove();
        }
    }
}

void Player::_getTransformedVertex(unsigned int vertexIndex, float *position) {
    float tmp[4];

    vector<GLfloat> vertex = this->_model->getVertex(vertexIndex);
    tmp[0] = vertex[0];
    tmp[1] = vertex[1];
    tmp[2] = vertex[2];
    tmp[3] = 1;

    // Transform the vertex with the current modelview matrix
    this->_transformationMatrix->multiplyVector(tmp, position);
}

/******************************************************************************
 * Guns
 *****************************************************************************/
float * Player::getGunPosition() {
    float *result = new float[4];
    this->_getTransformedVertex(1135, result);
    return result;
}

void Player::fire() {
    this->_gunParticleSystem->emitParticles();

    // Play the fire sound
    int error = Mix_PlayChannel(-1, this->_fireSound, 0);
    if (error == -1) {
        cout << "There was an error playing this sound: " << Mix_GetError() << endl;
    }
}

void Player::checkOpponentHit() {
    // Pass all the opponents to the gun particle system
    list<Opponent *>::iterator it = Opponent::opponents.begin();
    for (; it != Opponent::opponents.end(); ++it) {
        this->_gunParticleSystem->collisionDetect(*it);
    }
}

/****************************************************************************************
 * Exhaust
 ***************************************************************************************/
void Player::getExhaustPosition(int vertex, float *position, float *normal) {
    this->_getTransformedVertex(vertex, position);
    
    // The normal in this case is pointing backwards, we just transform (0, 0, 1);
    float tmp[4];
    tmp[0] = 0;
    tmp[1] = 0;
    tmp[2] = -1;
    tmp[3] = 1;

    // Transform the vertex with the current modelview matrix
    this->_transformationMatrix->multiplyVectorSkipTranslation(tmp, normal);
}

/****************************************************************************************
 * Game related methods
 ***************************************************************************************/
void Player::increaseScore() {
    this->_score += 100;
}

unsigned int Player::getScore() {
    return this->_score;
}

unsigned int Player::getLives() {
    return this->_lives;
}

/****************************************************************************************
 * Movement
 ***************************************************************************************/
void Player::moveLeft() {
    this->_moveLeft = true;
}
void Player::moveRight() {
    this->_moveRight = true;
}
void Player::cancelMoveLeft() {
    this->_moveLeft = false;
}

void Player::cancelMoveRight() {
    this->_moveRight = false;
}
