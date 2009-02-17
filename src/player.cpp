#include "player.h"
#include "obj.h"
#include "opponent.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <math.h>
#include <iostream>

#define PI 3.14159265

using namespace std;

GLfloat Player::scale = 0.00002;

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
    vector<GLfloat> lightVertex = this->_model->getVertex(1375);
    vector<GLfloat> leftExaust = this->_model->getVertex(1019);
    vector<GLfloat> rightExaust = this->_model->getVertex(2496);
    vector<GLfloat> normal; normal.push_back(0); normal.push_back(0); normal.push_back(-1);
    ParticleSystem *particleSystem = new ParticleSystem(0.5, this);
    particleSystem->updatePosition(leftExaust, normal);
    this->_particleSystems.push_back(particleSystem);

    particleSystem = new ParticleSystem(0.5, this);
    particleSystem->updatePosition(rightExaust, normal);
    this->_particleSystems.push_back(particleSystem);

    // And the gun particle system
    this->_gunParticleSystem = new Projectiles(this);

    this->_modelViewMatrix = new float[16];
    this->_transformationMatrix = new Matrix();
}

void Player::moveLaterally(float lateralMovement) {
    this->_lateralDelta += lateralMovement;
}

void Player::moveForward() {
    this->_rotation += 0.5;
    if (this->_rotation >= 360) this->_rotation -= 360;

    // Increment the sway
    this->_sway += this->_swayDelta;
    //if (this->_sway > this->_maxSway) this->_swayDelta * -1;

    // Tick the particle systems
    this->_gunParticleSystem->tick();
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
    // Render particle systems
    list< ParticleSystem * >::iterator it = this->_particleSystems.begin();
    for (; it != this->_particleSystems.end(); ++it) {
        (*it)->render();
    }

    // Move the model to the right place
    glPushMatrix();

    Player::transformPlayer(this->_rotation, this->_sway, this->_lateralDelta, this->_altitude);
    glMultMatrixf(this->_transformationMatrix->getMatrix());

    // Save the transformation matrix
    glGetFloatv(GL_MODELVIEW_MATRIX, this->_modelViewMatrix);

    this->_renderLights();
    this->_model->render();
    glPopMatrix();

    this->_gunParticleSystem->render();

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
    // Get the right wing
    glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);
    glPushAttrib(GL_TEXTURE_BIT);
    vector<GLfloat> lightVertex = this->_model->getVertex(1375);
    GLfloat tmp[4];
    tmp[0] = lightVertex[0];
    tmp[1] = lightVertex[1];
    tmp[2] = lightVertex[2];
    tmp[3] = 1;
    glTranslatef(tmp[0], tmp[1], tmp[2]);
    glLightfv(GL_LIGHT2, GL_POSITION, tmp);
    glColor3f(1, 0, 0);
    GLfloat emission[] = { 1, 0, 0, 0 };
    GLfloat emission2[] = { 1, 0, 0, 0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT, emission2);
    glutSolidSphere(500, 5, 5);
    glPopMatrix();
    glPushMatrix();

    lightVertex = this->_model->getVertex(2811);
    tmp[0] = lightVertex[0];
    tmp[1] = lightVertex[1];
    tmp[2] = lightVertex[2];
    tmp[3] = 1;
    glTranslatef(tmp[0], tmp[1], tmp[2]);
    glLightfv(GL_LIGHT2, GL_POSITION, tmp);
    glColor3f(0, 1, 0);
    GLfloat emission3[] = { 0, 1, 0, 0 };
    GLfloat emission4[] = { 0, 1, 0, 0 };
    glMaterialfv(GL_FRONT, GL_EMISSION, emission4);
    glMaterialfv(GL_FRONT, GL_AMBIENT, emission3);
    glutSolidSphere(500, 5, 5);
    glPopAttrib();
    glPopAttrib();
    glPopMatrix();
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

/******************************************************************************
 * Guns
 *****************************************************************************/
float * Player::getGunPosition() {
    float *result = new float[4];
    float *tmp = new float[4];

    vector<GLfloat> vertex = this->_model->getVertex(1135);
    tmp[0] = vertex[0];
    tmp[1] = vertex[1];
    tmp[2] = vertex[2];
    tmp[3] = 1;

    // Transform the vertex with the current modelview matrix
    this->_transformationMatrix->multiplyVector(tmp, result);

    return result;
}

void Player::fire() {
    this->_gunParticleSystem->emitParticles();
}

void Player::checkOpponentHit() {
    // Pass all the opponents to the gun particle system
    list<Opponent *>::iterator it = Opponent::opponents.begin();
    for (; it != Opponent::opponents.end(); ++it) {
        this->_gunParticleSystem->collisionDetect(*it);
    }
}
