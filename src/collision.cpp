#include "collision.h"
#include <iostream>

using namespace std;

bool Collidable::checkCollision(Collidable* other) {
    // For each dimension check if max and mins overlap
    float* bounds_a = this->getBounds();
    float* bounds_b = other->getBounds();

    if (bounds_a == NULL or bounds_b == NULL) return false;

    for (int i = 0; i < 6; i += 2) {
        if (bounds_a[i] < bounds_b[i + 1] && bounds_a[i + 1] > bounds_b[i]) {
            /*cout << bounds_a[i] << " < " << bounds_b[i + 1] << endl;
            cout << bounds_a[i + 1] << " > " << bounds_b[i] << endl;*/
            // this an overloap
        } else {
            // anything else is not
            return false;
        }
    }
    return true;
}
