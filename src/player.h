/**
 * Class representing the player
 *
 * TODO:
 *  * refactor transform player to not need arguments
 */
#pragma once

#include <vector>
#include "renderable.h"
#include "obj.h"
#include "particle.h"
#include "collision.h"
#include "projectiles.h"
#include "matrix.h"

using namespace std;

class ParticleSystem;
class Projectiles;

class Player : public Renderable, public Collidable {

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
        void transformPlayer(GLfloat rotation, GLfloat sway, GLfloat lateralDelta, GLfloat altitude);
        Matrix* getTransformationMatrix();
        static float scale;

        // Game related methods
        void increaseScore();
        unsigned int getScore();
        unsigned int getLives();


        // Collision method
        void checkOpponentCollision();

        // Get the position of the gun nozzle (for projectiles)
        float* getGunPosition();
        void fire();
        void checkOpponentHit();

        // Add a static member so we can get the player anywhere
        static Player *player;

    private:
        float _altitude;
        float _rotation;
        float _lateralDelta;
        float _sway;
        float _swayDelta;
        float _maxSway;
        Obj *_model;
        list< ParticleSystem* > _particleSystems;
        Projectiles *_gunParticleSystem;
        void _renderLights();
        float *_modelViewMatrix;
        Matrix *_transformationMatrix;

        // Game members
        unsigned int _score;
        unsigned int _lives;
};
