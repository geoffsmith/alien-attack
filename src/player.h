#pragma once

#include <vector>
#include "renderable.h"
#include "obj.h"
#include "particle.h"

using namespace std;

class Player : public Renderable {

    public:
        Player(float altitude);
        void moveLaterally(float lateralMovement);
        void moveForward();
        void setCamera();
        float getRotationRad();
        virtual void render();
        virtual void renderShadow() {};

    private:
        float _altitude;
        float _rotation;
        float _lateralDelta;
        float _sway;
        float _swayDelta;
        float _maxSway;
        Obj* _model;
        list< ParticleSystem* > _particleSystems;
};
