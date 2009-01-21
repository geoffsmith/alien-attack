#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include <planet.h>

#include <list>
#include <string>

using namespace std;

static list<Renderable *> renderables;

void initObjects() {
    renderables = new list<Renderable *>();
    Planet *earth = new Planet(10, 0, 0, 0);
    renderables.push_back(earth);
}

void initGlut(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(1.5);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
}

void displayGlut(void) {
}


void reshapeGlut(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {

    // Extract any command line options intended for GLUT
    glutInit(&argc, argv);
    
    // Request an initial window size of 512 x 512
    glutInitWindowSize(512, 512);

    // Request an initial display mode with RGB color and double buffer
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA);

    // Create a window with given title
    glutCreateWindow("OpenGL tutorial 1");
    initGlut();

    // Register a display callback function
    glutDisplayFunc(displayGlut);
    glutReshapeFunc(reshapeGlut);

    // Register an idle callback function for animations
    //glutIdleFunc(idle);
    //glutTimerFunc(pause_time, move, 0);

    // Enter the main GLUT event processing loop
    glutMainLoop();

    return 0;
}
