/**
 * Subclass of the particle system to represent the player's ship's exhaust
 */
#pragma once

#include "particle_system.h"
#include "player.h"

class ParticleSystem2;
class Player;

class Exhaust : public ParticleSystem2 {
    public:
	Exhaust(Player *player, unsigned int vertex);

    protected:
        virtual void _setStartPosition(Particle2 *particle);
        virtual void _updateParticles();

    private:
	Player *_player;
	unsigned int _vertex;
};
