#include <vector>
#include "renderable.h"
#include "obj.h"

using namespace std;

class Opponent : public Renderable {

    public:
        Opponent();
        void moveForward();
        virtual void render();
        virtual void renderShadow() {};
        static Opponent* generate();

        // Probability of generating a new opponent on each cycle
        static const float generateProbability;
        static const unsigned int maxOpponents;
        static list< Opponent* > opponents;

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

};
