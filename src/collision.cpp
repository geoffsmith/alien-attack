#include "collision.h"
#include <iostream>

using namespace std;

bool Collidable::checkCollision(Collidable* other) {
    // For each dimension check if max and mins overlap
    float* bounds_a = this->getBounds();
    float* bounds_b = other->getBounds();

    for (int i = 0; i < 6; i += 2) {
        //if (bounds_a[i] <= bounds_b[i + 1] && bounds_a[i + 1] >= bounds_b[i]) {
        if (bounds_a[i] >= bounds_b[i + 1]) return false;
        if (bounds_b[i] >= bounds_a[i + 1]) return false;
    }
    return true;
}
