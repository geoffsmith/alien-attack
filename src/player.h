#pragma once

#include <vector>
#include "renderable.h"
#include "obj.h"
#include "particle.h"

using namespace std;

class ParticleSystem;

class Player : public Renderable {

    public:
        Player(float altitude);
        void moveLaterally(float lateralMovement);
        void moveForward();
        void setCamera();
        float getRotationRad();
        float getRotation();
        float getAltitude();
        float getLateralDelta();
        float getSway();
        virtual void render();
        virtual void renderShadow() {};
        static void transformPlayer(GLfloat rotation, GLfloat sway, GLfloat lateralDelta, GLfloat altitude);
        static float scale;

    private:
        float _altitude;
        float _rotation;
        float _lateralDelta;
        float _sway;
        float _swayDelta;
        float _maxSway;
        Obj* _model;
        list< ParticleSystem* > _particleSystems;
        void _renderLights();
};
