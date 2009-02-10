#pragma once

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void load_texture(GLuint &tex_name, unsigned char *texture_image, const char *filename);
void read_obj(const char *filename, GLfloat (*vertices)[3], GLfloat (*textures)[2], GLfloat (*normals)[3]);


/******************************************************************************
 * OpenGL vector math helpers
 *****************************************************************************/
void crossProduct(float* matrix, float* vector, float* result);
