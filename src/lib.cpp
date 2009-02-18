#include "lib.h"
#include "jpeg.h"

#include <math.h>
#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


using namespace std;


void load_texture(GLuint &tex_name, const char *filename) {
    unsigned char *texture_image;
    struct jpeg_decompress_struct cinfo;

    // Load the mars texture
    char file[strlen(filename)];
    strcpy(file, filename);
    read_jpeg_file(file, &texture_image, cinfo);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, tex_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cinfo.image_width,
            cinfo.image_height, 0, GL_RGB, GL_UNSIGNED_BYTE,
            texture_image);

    GLuint error = glGetError();
    if (error > 0) {
        cout << "Texture Error: " << gluErrorString(error) << " (" << error << ") in texture " << filename << endl;
    }

    // TODO: Clean up image
}

void read_obj(const char *filename, GLfloat (*vertices)[3], GLfloat (*textures)[2], GLfloat (*normals)[3]) {
}

void matrixMultiply(float* matrix, float* vector, float* result) {
    float sum;
    for (int i = 0; i < 4; ++i) {
        sum = 0;
        for (int j = 0; j < 4; ++j) {
            sum += matrix[j * 4 + i] * vector[j];
        }
        result[i] = sum;
    }
}

void buildYRotationMatrix(float *matrix, float angle) {
    matrix[0] = cos(angle);
    matrix[1] = 0;
    matrix[2] = -1 * sin(angle);
    matrix[3] = 0;

    matrix[4] = 0;
    matrix[5] = 1;
    matrix[6] = 0;
    matrix[7] = 0;

    matrix[8] = sin(angle);
    matrix[9] = 0;
    matrix[10] = cos(angle);
    matrix[11] = 0;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}
