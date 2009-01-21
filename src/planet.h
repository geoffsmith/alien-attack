#include "renderable.h"
#include <vector>

using namespace std;

class Planet : public Renderable {
    public:
        void render();
        Planet(int radius, float center_x, float center_y, float center_z);

    private:
        int _radius;
        vector<float> _center;
};
