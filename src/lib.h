#pragma once

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

void load_texture(GLuint &tex_name, const char *filename);
void read_obj(const char *filename, GLfloat (*vertices)[3], GLfloat (*textures)[2], GLfloat (*normals)[3]);


/******************************************************************************
 * OpenGL vector math helpers
 *****************************************************************************/
void matrixMultiply(float* matrix, float* vector, float* result);
void buildYRotationMatrix(float *matrix, float angle);
