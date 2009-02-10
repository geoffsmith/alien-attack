#pragma once

class Collidable {

    public:
        virtual bool checkCollision(Collidable* other);
        virtual float* getBounds() = 0;
        virtual ~Collidable() {};
};
