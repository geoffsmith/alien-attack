#include "collision.h"
#include <iostream>
#include <cmath>

using namespace std;

Collidable::Collidable() {
    this->_bounds = new float[6];
}

float* Collidable::getBounds() {
    return this->_bounds;
}

bool Collidable::checkCollision(Collidable* other) {
    // For each dimension check if max and mins overlap
    float* bounds_a = this->getBounds();
    float* bounds_b = other->getBounds();
    float T, a, b;

    for (int i = 0; i < 6; i += 2) {
        a = (bounds_a[i + 1] - bounds_a[i]) / 2.0;
        b = (bounds_b[i + 1] - bounds_b[i]) / 2.0;
        T = fabs((bounds_a[i] + a) - (bounds_b[i] + b));
        if (T > a + b) return false;
    }
    return true;
}
