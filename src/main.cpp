#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "planet.h"
#include "renderable.h"

#include <list>
#include <iostream>


using namespace std;

static list<Renderable *> renderables;
static int pause_time = 50; // milliseconds

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.5);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}

void init_objects() {
    renderables = list<Renderable *>();
    Planet *earth = new Planet(10, 0, 0, 0);
    // TODO: need to sort out the relative paths
    earth->setTexture("resources/earthmap1k.jpg");
    renderables.push_back(earth);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}


void display(void) {
    // display the planets
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity(); // clear the matrix

    // Viewing transformation
    gluLookAt(0.0, 16.0, -19.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // iterate through the renderables
    list<Renderable *>::iterator it = renderables.begin();
    while (it != renderables.end()) {
        (*it)->render();
        ++it;
    }

    // Swap and flush
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

void move(int i) {
    // Mark the normal plane of the current window as needing to be redisplayed
    glutPostRedisplay();

    // do this again in pause msecs
    glutTimerFunc(pause_time, move, 0);
}


int main(int argc, char** argv) {

    // Extract any command line options intended for GLUT
    glutInit(&argc, argv);
    
    // Request an initial window size of 512 x 512
    glutInitWindowSize(512, 512);

    // Request an initial display mode with RGB color and double buffer
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);

    // Create a window with given title
    glutCreateWindow("OpenGL tutorial 1");
    init();
    init_objects();

    // Register a display callback function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Register an idle callback function for animations
    //glutIdleFunc(idle);
    glutTimerFunc(pause_time, move, 0);


    // Enter the main GLUT event processing loop
    glutMainLoop();

    return 0;
}
