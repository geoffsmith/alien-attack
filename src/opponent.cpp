#include "opponent.h"
#include <iostream>
#include "matrix.h"

using namespace std;

const float Opponent::generateProbability = 0.3;
const unsigned int Opponent::maxOpponents = 5;
list< Opponent* > Opponent::opponents;

Opponent::Opponent() {
    // Load the model
    this->_model = Obj::makeObj("resources/missile/GBU-12-reduced.obj");

    // Set up the constants
    this->_altitude = 12.6;
    this->_rotation = rand() % 360;
    this->_lateralRotation = 10 - rand() % 20;

}

void Opponent::render() {
    Matrix matrix;
    float scale = 0.25;

    glPushMatrix();

    matrix.rotateY(-1 * this->_rotation - 7.0);
    matrix.rotateX(-1 * this->_lateralRotation);
    matrix.translate(0, 0, this->_altitude - 2.2);
    matrix.rotateZ(90);
    matrix.scale(scale);

    // Save the modelview matrix before rendering
    glMultMatrixf(matrix.getMatrix());

    this->_model->render();
    glPopMatrix();

    // Calculate the bounds while we have the modelview matrix
    this->_model->calculateBounds(&matrix, this->_bounds);
}

void Opponent::moveForward() {
    this->_rotation += 0.2;
}

void Opponent::triggerCollision() {
    // Remove from list
    Opponent::opponents.remove(this);
}

/******************************************************************************
 * Static methods
 *****************************************************************************/
Opponent* Opponent::generate() {
    if (Opponent::opponents.size() < Opponent::maxOpponents) {
        float prob = ( rand() % 100 ) / 100.0;
        if ( prob > Opponent::generateProbability) {
            Opponent* opp = new Opponent();
            Opponent::opponents.push_back(opp);
            return opp;
        }
    }
    return NULL;
}

void Opponent::renderAll() {
    // Render each opponent
    list<Opponent *>::iterator it = Opponent::opponents.begin();
    for (; it != Opponent::opponents.end(); ++it) {
        (*it)->render();
    }
}

void Opponent::moveForwardAll() {
    // Move each opponent forward
    list<Opponent *>::iterator it = Opponent::opponents.begin();
    for (; it != Opponent::opponents.end(); ++it) {
        (*it)->moveForward();
    }
}
