#pragma once

#include <vector>

using namespace std;

class Player {

    public:
        Player(float altitude);
        void moveLaterally(float lateralMovement);
        void moveForward();
        void setCamera();
        float getRotationRad();

    private:
        float _altitude;
        float _rotation;
        float _lateralDelta;

};
