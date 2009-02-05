#include "lib.h"
#include "jpeg.h"

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


using namespace std;


void load_texture(GLuint &tex_name, unsigned char *texture_image, const char *filename) {
    
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
}

void read_obj(const char *filename, GLfloat (*vertices)[3], GLfloat (*textures)[2], GLfloat (*normals)[3]) {
}
