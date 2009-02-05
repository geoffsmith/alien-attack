#include "opponent.h"
#include <iostream>

using namespace std;

const float Opponent::generateProbability = 0.3;
const unsigned int Opponent::maxOpponents = 20;
list< Opponent* > Opponent::opponents;

Opponent::Opponent() {
    // Load the model
    this->_model = Obj::makeObj("resources/missile/GBU-12-reduced.obj");

    // Set up the constants
    this->_altitude = 13;
    this->_rotation = rand() % 360;
    this->_lateralRotation = 10 - rand() % 20;

}

void Opponent::render() {
    float scale = 0.25;
    glPushMatrix();
    glRotatef(-1 * this->_rotation - 7, 0, 1, 0);
    glRotatef(-1 * this->_lateralRotation, 1, 0, 0);
    glTranslatef(0, 0, this->_altitude - 2.5);
    glRotatef(90, 0, 0, 1);
    glScalef(scale, scale, scale);
    this->_model->render();
    glPopMatrix();
}

void Opponent::moveForward() {
    this->_rotation += 0.2;
}

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
