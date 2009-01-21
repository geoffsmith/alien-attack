#include "renderable.h"
#include <OpenGL/gl.h>
#include <vector>
#include <string>

using namespace std;

class Planet : public Renderable {
    public:
        virtual void render();
        Planet(int radius, float center_x, float center_y, float center_z);
        void setTexture(string path);
        ~Planet();

    private:
        int _radius;
        int _sections;
        vector<float> _center;
        string _texturePath;
        GLuint _texture;

        int _secondsPerRotation;

        // Get the planet rotation
        float _getRotation();

};
