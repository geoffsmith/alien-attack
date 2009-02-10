/**
 * TODO:
 *  * Enable back face culling for speed enhancement. Need to sort out particle
 *    normals first though.
 */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#include "planet.h"
#include "renderable.h"
#include "player.h"
#include "star.h"
#include "shadow.h"
#include "opponent.h"

#include <list>
#include <iostream>
#include <time.h>


using namespace std;

static list<Renderable *> renderables;
static Player* player;
static list< Opponent* > opponents;
static int pause_time = 50; // milliseconds
static GLfloat light_position[] = { 0.0, 0.0, 20.0, 0 };
static GLfloat moon_light_position[] = { 0, 0, -20, 0 };
static bool doMove = true;

void setupLighting();

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glLineWidth(1.5);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

    setupLighting();

    srand(clock());
}

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    // Add two lights to the ship
    glEnable(GL_LIGHT2);
    //GLfloat ship_left_light[] = { 1, 0, 0 };
    //glLightfv(GL_LIGHT2, GL_DIFFUSE, ship_left_light);
    glEnable(GL_LIGHT3);

    // Set up the ambient light so we can see on the dark side of the planet
    GLfloat light[] = { 0, 0, 0 };
    GLfloat dark[] = { 1, 1, 1};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light);
    glLightfv(GL_LIGHT1, GL_AMBIENT, dark);
}

void init_objects() {
    renderables = list<Renderable *>();
    Planet *earth = new Planet(10, 0, 0, 0);
    // TODO: need to sort out the relative paths
    earth->setTexture("resources/earthmap1k.jpg");
    renderables.push_back(earth);

    // Setup the player
    player = new Player(12);
    renderables.push_back(player);

    // Create the stars
    StarGenerator *stars = new StarGenerator();
    renderables.push_back(stars);

    // Create an opponent
    Opponent* opponent = new Opponent();
    renderables.push_back(opponent);
    opponents.push_back(opponent);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
    glMatrixMode(GL_MODELVIEW);
}


void display(void) {
    // display the planets
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity(); // clear the matrix

    // Viewing transformation
    //gluLookAt(0.0, 5.0, -19.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    player->setCamera();


    // iterate through the renderables
    list<Renderable *>::iterator it = renderables.begin();
    while (it != renderables.end()) {
        (*it)->render();
        ++it;
    }

    //renderables.front()->renderShadow();

    // render shadows (planet is the first renderable)
    //renderShadow(renderables.front());

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, moon_light_position);

    // Swap and flush
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

void move(int i) {
    if (doMove) {
        player->moveForward();

        // Move all the opponents
        list< Opponent* >::iterator it = opponents.begin();
        for (; it != opponents.end(); ++it) {
            // Check for collision with player
            if (player->checkCollision((*it))) {
                cout << "Detected collision!" << endl;
            }

            (*it)->moveForward();
        }

        // Generate new opponents
        if (opponents.size() < 5) {
            Opponent* newOpponent = Opponent::generate();
            if (newOpponent != NULL) {
                opponents.push_back(newOpponent);
                renderables.push_back(newOpponent);
            }
        }
    }

    // Mark the normal plane of the current window as needing to be redisplayed
    glutPostRedisplay();

    // do this again in pause msecs
    glutTimerFunc(pause_time, move, 0);
}

/**
 * Take keyboard input
 */
void keyboardInput(unsigned char key, int x, int y) {
}

void specialKeyInput(int key, int x, int y) {
    if (key == GLUT_KEY_RIGHT) {
        player->moveLaterally(1);
    } else if (key == GLUT_KEY_LEFT) {
        player->moveLaterally(-1);
    } else if (key == GLUT_KEY_UP) {
        doMove = !doMove;
    }
}

int main(int argc, char** argv) {

    // Extract any command line options intended for GLUT
    glutInit(&argc, argv);
    
    // Request an initial window size of 512 x 512
    glutInitWindowSize(512, 512);

    // Request an initial display mode with RGB color and double buffer
    glutInitDisplayMode(GLUT_DEPTH | GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_STENCIL);

    // Create a window with given title
    glutCreateWindow("OpenGL tutorial 1");
    init();
    init_objects();

    // Register a display callback function
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboardInput);
    glutSpecialFunc(specialKeyInput);

    // Register an idle callback function for animations
    //glutIdleFunc(idle);
    glutTimerFunc(pause_time, move, 0);


    // Enter the main GLUT event processing loop
    glutMainLoop();

    return 0;
}
