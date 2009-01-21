#include "player.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <math.h>
#include <iostream>

#define PI 3.14159265

using namespace std;

Player::Player(float altitude) {
    this->_altitude = altitude;
    this->_rotation = 0;
    this->_lateralDelta = 0;
}

void Player::moveLaterally(float lateralMovement) {
    this->_lateralDelta += lateralMovement;
}

void Player::moveForward() {
    this->_rotation += 1;
    if (this->_rotation >= 360) this->_rotation -= 360;
}

void Player::setCamera() {
    // Rotate the camera up
    glRotatef(-45, 1, 0, 0);

    // Get the position of the camera
    glTranslatef(0, 0, -1 * this->_altitude);

    // Move right / left
    glRotatef(this->_lateralDelta, 0, 1, 0);

    // Move the camera up
    glRotatef(-90, 0, 0, 1);

    // Move the camera for the movement along
    glRotatef(this->_rotation, 0, 1, 0);
}

float Player::getRotationRad() {
    return this->_rotation * PI / 180;
}
