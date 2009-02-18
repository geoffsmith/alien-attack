#include "obj.h"
#include <fstream>
#include <iostream>
#include <unistd.h>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

/**
  * Obj format definition: http://people.sc.fsu.edu/~burkardt/txt/obj_format.txt
  * usemtl
  * s: smoothing group, ignore for now
  * newmtl: ka, kd, ks, illum, ns
  * g
**/

using namespace std;
using namespace boost;
using namespace boost::filesystem;
namespace fs = boost::filesystem;

list< Obj* > Obj::objectCache;

Obj* Obj::makeObj(const char* filename) {
    // First check if we already have an object that fits
    list< Obj* >::iterator it = Obj::objectCache.begin();
    for(; it != Obj::objectCache.end(); ++it) {
        if ((*it)->filename == filename) {
            return *it;
        }
    }
    // we didn't find one, so create one here
     Obj* result = new Obj(filename, Obj::objectCache.size() + 1);
     Obj::objectCache.push_back(result);
     return result;
}

Obj::Obj(const char *filename, unsigned int displayList) {
    this->filename = filename;
    this->_displayList = displayList;

    // Load this file in 
    ifstream file(filename);
    if (!file.is_open()) cout << "Error opening obj file\n";
    string line;

    // Keep track of the current material
    Material* currentMaterial;
    vector<string> parts;

    while (!file.eof()) {
        getline(file, line);
        // parse this line and make either a vector, normal, texture coordinate or
        // face
        size_t position = line.find_first_of(' ');
        string command = line.substr(0, position);
        split(parts, line, is_any_of(" "));
        if (command == "v") {
            this->_addVertex(line);
        } else if (command == "vt") {
            this->_addTextureCoord(line);
        } else if (command == "vn") {
            this->_addNormal(line);
        } else if (command == "f") {
            this->_addFace(line, currentMaterial);
        } else if (command == "mtllib") {
            this->_addMTL(line);
        } else if (command == "usemtl") {
            // Try to find the material
            trim(parts[1]);
            currentMaterial = this->_findMaterial(parts[1]);
        } else if (command == "s" || command == "g" || command == "#" || command == "") {
            // ignore for now
        } else {
            cout << "Ignore obj command: " << command << endl;
        }
    }
    file.close();

    // Create the display list
    this->_createDisplayList();
}

void Obj::_addVertex(string line) {
    vector<string> tmp;
    vector<GLfloat>* vertex = new vector<GLfloat>;
    split(tmp, line, is_any_of(" "), token_compress_on);

    for (int i = 0; i < 3; ++i) {
        vertex->push_back(atof(tmp[i + 1].c_str()));
    }

    this->_vertices.push_back(vertex);
    this->_verticesList.push_back(vertex);
}

void Obj::_addTextureCoord(string line) {
    vector<string> tmp;
    vector<GLfloat>* texture = new vector<GLfloat>;
    split(tmp, line, is_any_of(" "), token_compress_on);

    for (int i = 0; i < 2; ++i) {
        texture->push_back(atof(tmp[i + 1].c_str()));
    }

    this->_textureCoords.push_back(texture);
}

void Obj::_addNormal(string line) {
    vector<string> tmp;
    vector<GLfloat>* normal = new vector<GLfloat>;
    split(tmp, line, is_any_of(" "), token_compress_on);

    for (int i = 0; i < 3; ++i) {
        normal->push_back(atof(tmp[i + 1].c_str()));
    }

    this->_normals.push_back(normal);
}

void Obj::_addFace(string line, Material* material) {

    vector<string> vertices;
    split(vertices, line, is_any_of(" "), token_compress_on);

    Face* face = new Face;
    vector<string> vertex;

    // Add the material to the face
    face->material = material;

    vector<GLfloat>* pointer;

    // Skip the first one, its the command
    for (int i = 1; i < 4; ++i) {
        //vertex = this->_splitLine(vertices[i], 3, '/');
        split(vertex, vertices[i], is_any_of("/"));

        // Look up the vertex and add the pointer
        pointer = this->_vertices[atoi(vertex[0].c_str()) - 1];

        face->vertices[i - 1] = pointer;

        if (vertex[1].size() > 0) {
            pointer = this->_textureCoords[atoi(vertex[1].c_str()) - 1];
            face->textures[i - 1] = pointer;
        }
        else {
            face->textures[i - 1] = 0;
        }

        pointer = this->_normals[atoi(vertex[2].c_str()) - 1];
        face->normals[i - 1] = pointer;
    }
    this->_faces.push_back(face);

}

vector<GLfloat> Obj::_parseLine(string line, const unsigned int length, const char seperator) {
    // split up the parts and convert to  vector<GLfloat>
    vector<string> parts = this->_splitLine(line, length + 1, seperator);
    vector<GLfloat> result(length);
    // Skip the first one its the command
    for (unsigned int i = 1; i < length + 1; ++i) {
        result[i - 1] = atof(parts[i].c_str());
    }
    return result;
}

vector<string> Obj::_splitLine(string line, const unsigned int length, const char seperator) {
    string rest = line;
    size_t splitter;

    vector<string> result(length);
    for (unsigned int i = 0; i < length; ++i) {
        splitter = rest.find_first_of(seperator);
        result[i] = rest.substr(0, splitter);
        rest = rest.substr(splitter + 1, rest.length() - splitter);
    }

    return result;
}

void Obj::_createDisplayList() {
    glLoadIdentity();
    Face* face;
    vector<GLfloat>* texture;
    vector<GLfloat>* vertex;
    vector<GLfloat>* normal;
    list< Face * >::iterator it = this->_faces.begin();

    // Keep track of the current texture so that we are not updating too often
    GLuint currentTexture = 0;

    // We're making a display list
    cout << "Creating display list for " << this->filename << ": " << this->_displayList << endl;
    glNewList(this->_displayList, GL_COMPILE);

    // Iterate through all the faces
    while (it != this->_faces.end() ) {
        face = *it;

        // Check if we have a material and it has a texture
        if (face->material != 0
                && face->material->textureMap != 0
                && face->material->textureMap != currentTexture) {

            currentTexture = face->material->textureMap;
            //cout << "Binding texuture: " << currentTexture << endl;
            glBindTexture(GL_TEXTURE_2D, currentTexture);
        }

        // Draw a triangle
        glBegin(GL_TRIANGLES);
        for (int j = 0; j < 3; ++j) {

            normal = face->normals[j];
            glNormal3f((*normal)[0], (*normal)[1], (*normal)[2]);

            texture = face->textures[j];
            if (texture != 0) {
                // We need to convert obj texture corrdinates into openGL so X is 1 - X
                glTexCoord2f((*texture)[0], 1 - (*texture)[1]);
            }

            vertex = face->vertices[j];
            glVertex3f((*vertex)[0], (*vertex)[1], (*vertex)[2]);
        }
        glEnd();
        ++it;
    }

    // Stop making the list
    glEndList();
}

void Obj::render() { 
    glCallList(this->_displayList);
}

vector<GLfloat> Obj::getVertex(const unsigned int index) {
    vector<GLfloat> result;
    if (index < this->_vertices.size()) {
        result = (*this->_vertices[index]);
    } else {
        result.push_back(0);
        result.push_back(0);
        result.push_back(0);
    }
    return result;
}

void Obj::_addMTL(string line) {
    // Get the file path
    path mtlPath(this->filename);
    mtlPath.remove_filename();

    // Add on the mtl file name
    vector<string> parts;
    split(parts, line, is_any_of(" "));

    char *buf = new char[100];
    getcwd(buf, 100);

    path filePath = buf / mtlPath / parts[1];

    // The material file is stateful, we need the current material
    Material* material = 0;

    // This is dirty, there is a load of white space on the end of the string
    string spath = filePath.string();
    trim_right(spath);
    const char* cpath = spath.c_str();

    // parse this file
    ifstream file(cpath);
    if (!file.is_open()) {
        perror("Error opening MTL file: ");
        return;
    }

    string mtlLine;
    while (!file.eof()) {
        getline(file, mtlLine);
        parts.clear();
        split(parts, mtlLine, is_any_of(" "));
        string command = parts[0];
        if (command == "newmtl") {
            // If there was a previous material we push it 
            if (material) {
                this->_materials.push_back(material);
            }
            material = new Material;
            trim(parts[1]);
            material->name = parts[1];
            material->textureMap = 0;
        } else if (command == "map_Kd") {
            // Load up the texture
            path texPath = mtlPath / parts[1];
            glGenTextures(1, &(material->textureMap));
            load_texture(material->textureMap, texPath.string().c_str());
        }
    }
    file.close();
    if (material) {
        this->_materials.push_back(material);
    }
}

Material* Obj::_findMaterial(string name) {
    // iterate through the materials looking for this name
    list< Material * >::iterator it = this->_materials.begin();
    for (; it != this->_materials.end(); ++it) {
        if ((*it)->name == name) {
            return *it;
        }
    }
    return 0;
}

void Obj::calculateBounds(Matrix *transformationMatrix, float *bounds) {
    // Transform all the vertices into the right space.
    // ... and get the max / mins for each dimension
    float* result = new float[4];
    float* vertex = new float[4];
    vector<GLfloat>* pointer;
    vertex[3] = 1;

    // We reset the bounds on the first iteration. This could be done by comparing
    // it to vertices->begin() but this way should be faster.
    bool resetBounds = true;

    list< vector<GLfloat>* >::iterator it = this->_verticesList.begin();
    for (; it != this->_verticesList.end(); ++it) {
        pointer = *it;
        
        // Transform the vertex
        vertex[0] = (*pointer)[0]; vertex[1] = (*pointer)[1]; vertex[2] = (*pointer)[2];
        //matrixMultiply(transformationMatrix, vertex, result);
        transformationMatrix->multiplyVector(vertex, result);

        // Reset the bounds on the first vertex
        if (resetBounds) {
            bounds[0] = result[0];
            bounds[1] = result[0];
            bounds[2] = result[1];
            bounds[3] = result[1];
            bounds[4] = result[2];
            bounds[5] = result[2];
            resetBounds = false;
            continue;
        }

        // Check if it is the largest / smallest of each dimension
        if (result[0] < bounds[0]) {
            bounds[0] = result[0];
        }
        if (result[0] > bounds[1]) {
            bounds[1] = result[0];
        }
        if (result[1] < bounds[2]) {
            bounds[2] = result[1];
        }
        if (result[1] > bounds[3]) {
            bounds[3] = result[1];
        }
        if (result[2] < bounds[4]) {
            bounds[4] = result[2];
        }
        if (result[2] > bounds[5]) {
            bounds[5] = result[2];
        }
    }

    // clean up
    delete vertex;
    delete result;
}
