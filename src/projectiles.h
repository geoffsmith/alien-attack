/**
 * TODO:
 *  * Cache the gun position for each tick
 */
#pragma once

#include "player.h"
#include "particle_system.h"
#include "matrix.h"

class Player;
class Matrix;

class Projectiles : public ParticleSystem2 {
    public:
        Projectiles(Player *player);

    protected:
        virtual void _setStartPosition(Particle2 *particle);
        virtual void _updateParticles();

    private:
        Player *_player;
        Matrix *_rotationMatrix;
};
