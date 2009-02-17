#pragma once

class Collidable {

    public:
        Collidable();
        float* getBounds();
        virtual bool checkCollision(Collidable* other);
        virtual ~Collidable() {};

    protected:
        float *_bounds;
};
