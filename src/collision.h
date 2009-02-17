#pragma once

class Collidable {

    public:
        Collidable();
        float* getBounds();
        virtual bool checkCollision(Collidable* other);
        virtual ~Collidable() {};
        virtual void triggerCollision() {};

    protected:
        float *_bounds;
};
