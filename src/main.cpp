#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

using namespace std;

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

    // Swap and flush
    glutSwapBuffers();

    glDisable(GL_TEXTURE_2D);
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

    // Register a display callback function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Register an idle callback function for animations
    //glutIdleFunc(idle);
    //glutTimerFunc(pause_time, move, 0);

    // Enter the main GLUT event processing loop
    glutMainLoop();

    return 0;
}
