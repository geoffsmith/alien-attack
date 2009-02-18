/**
 * A class to represent a transformation matrix. It has a number of helper functions to help
 * build a transformation. These mostly mirror openGL functions, but give us better access to
 * the resulting matrix (and a nicer interface).
 *
 * Performance wise, putting this functionality in a class should be OK, because there 
 * shouldn't be too many matrices floating around (compared with vertices say). However, 
 * methods performed over large number of vertices need to be very fast i.e. multiplyVector
 */
#pragma once

#include <OpenGL/gl.h>

class Matrix {
    public:
        Matrix();
        Matrix(GLfloat *input);
        ~Matrix();
        void reset();
        float& operator[] (const unsigned int index);
        void rotateY(float angle);
        void rotateX(float angle);
        void rotateZ(float angle);
        void translate(float x, float y, float z);
        void scale(float scale);
        void multiplyVector(float *vector, float *result);
        void multiplyVectorSkipTranslation(float *vector, float *result);
        void multiplyMatrix(Matrix *matrix);
        float* getMatrix();

    private:
        float *_matrix;
};
