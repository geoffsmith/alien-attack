#include <vector>
#include "renderable.h"
#include "obj.h"
#include "collision.h"

#include <SDL/SDL_mixer.h>

using namespace std;

class Opponent : public Renderable, public Collidable {

    public:
        Opponent();
        void moveForward();
        virtual void render();
        virtual void renderShadow() {};

        static Opponent* generate();
        static void renderAll();
        static void moveForwardAll();

        // Probability of generating a new opponent on each cycle
        static const float generateProbability;
        static const unsigned int maxOpponents;
        static list< Opponent* > opponents;

        virtual void triggerCollision();
        void remove();

    private:
        Obj* _model;

        // Altitude measured from the center of the earth
        float _altitude;

        /**
         * Rotation around the planet, this will decrease so that the opponent
         * moves towards the player
         */
        float _rotation;

        /**
         * Rotation north / south. This is a random constance for now +/- angle
         * from player
         */
        float _lateralRotation;

        // Sounds
        Mix_Chunk *_explode;
};
