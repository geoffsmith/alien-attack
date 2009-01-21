#include "planet.h"
#include <vector>

void Planet::render() {
    // Do the opengl rendering here
}

Planet::Planet(int radius, float center_x, float center_y, float center_z) {
    this._radios = radius;
    vector<float> center(3);
    center[0] = center_x;
    center[1] = center_y;
    center[2] = center_z;
    this._center = center;
}

