#pragma once

#include "lib.h"
#include "matrix.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <string>
#include <vector>
#include <list>

/**
  * TODO:
  *     * Clean up faces when object is destroyed
  *     * Add other components to the Material
  *     * Clean up textures when they are loaded
  *     * Convert to use Boost string libraries everywhere
***/

using namespace std;

struct Material {
    string name;
    GLuint textureMap;
};

struct Face {
    vector<GLfloat>* vertices[3];
    vector<GLfloat>* normals[3];
    vector<GLfloat>* textures[3];
    Material* material;
};


class Obj {
    public:
        Obj(const char *filename, unsigned int displayList);
        void render();
        vector<GLfloat> getVertex(const unsigned int index);
        string filename;
        void calculateBounds(Matrix *transformationMatrix, float *bounds);

        // A cache for loaded objects
        static list< Obj* > objectCache;
        static Obj* makeObj(const char* filename);

    private:
        vector< vector<GLfloat>* > _vertices;
        list< vector<GLfloat>* > _verticesList;
        vector< vector<GLfloat>* > _textureCoords;
        vector< vector<GLfloat>* > _normals;
        list< Material * > _materials;
        list< Face * > _faces;

        void _addVertex(string line);
        void _addTextureCoord(string line);
        void _addNormal(string line);
        void _addFace(string line, Material* material);

        // The display list for this object
        unsigned int _displayList;

        /**
         * Parse a mtllib line in the obj file. Kicks off a MTL file parse. Return
         * NULL if not found.
         */
        void _addMTL(string line);

        /**
         * Get a pointer to the named material
         */
        Material* _findMaterial(string name);

        // Parse a line into a vector of length length
        vector<GLfloat> _parseLine(string line, const unsigned int length, const char seperator=' ');
        vector<string> _splitLine(string line, const unsigned int length, const char seperator=' ');

        // Create the display list
        void _createDisplayList();

};
