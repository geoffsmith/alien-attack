/**
 * TODO:
 *  * Enable back face culling for speed enhancement. Need to sort out particle
 *    normals first though.
 */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include <FTGL/FTGLPixmapFont.h>

#include "planet.h"
#include "renderable.h"
#include "player.h"
#include "star.h"
#include "shadow.h"
#include "opponent.h"

#include <list>
#include <sstream>
#include <iostream>
#include <time.h>
#include <string>


using namespace std;

static list<Renderable *> renderables;
static list< Opponent* > opponents;
static int pause_time = 20; // milliseconds
static GLfloat light_position[] = { 0.0, 0.0, 20.0, 0 };
static GLfloat moon_light_position[] = { 0, 0, -20, 0 };
static bool doMove = true;
static string fontPath = "resources/airstrip.ttf";
static FTGLPixmapFont* font;

void setupLighting();

void loadFont() {
    font = new FTGLPixmapFont(fontPath.c_str());
    if (font->Error()) {
        cout << "There was an error loading the font: " << fontPath << endl;
    }
}

void initPointSprite() {
    // Check that the extension is availble
    char *extensions = (char *)glGetString(GL_EXTENSIONS);
    
    // Check for GL_ARB_point_sprite
    if (strstr(extensions, "GL_ARB_point_sprite") == NULL) {
        cout << "ERROR: GL_ARB_point_sprite is not avaiable" << endl;
        exit(1);
    }

    // Check for GL_ARB_point_parameters
    if (strstr(extensions, "GL_ARB_point_sprite") == NULL) {
        cout << "ERROR: GL_ARB_point_parameters is not available" << endl;
        exit(1);
    }

}

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

    loadFont();

    initPointSprite();
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
    Player::player = new Player(12);
    renderables.push_back(Player::player);

    // Create the stars
    StarGenerator *stars = new StarGenerator();
    renderables.push_back(stars);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void renderScore() {

    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, 10, 0, 10, 0, 10);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);

    glColor4f(1, 0, 0, 1);

    glRasterPos3f(0.5, 9, 0);
    font->FaceSize(36);

    stringstream outputStream;
    outputStream << Player::player->getScore();


    font->Render(outputStream.str().c_str());

    // Show the lives
    stringstream livesOutputStream;
    glRasterPos3f(0.5, 0.5, 0);
    font->FaceSize(18);

    livesOutputStream << "Lives left: " << Player::player->getLives();
    font->Render(livesOutputStream.str().c_str());

    glMatrixMode(GL_PROJECTION);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void display(void) {
    // display the planets
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);

    renderScore();

    glColor3f(1.0, 1.0, 1.0);
    glLoadIdentity(); // clear the matrix

    // Viewing transformation
    //gluLookAt(0.0, 5.0, -19.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    Player::player->setCamera();


    // iterate through the renderables
    list<Renderable *>::iterator it = renderables.begin();
    while (it != renderables.end()) {
        (*it)->render();
        ++it;
    }

    Opponent::renderAll();

    //renderables.front()->renderShadow();

    // render shadows (planet is the first renderable)
    //renderShadow(renderables.front());

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, moon_light_position);

    renderScore();

    // Swap and flush
    glutSwapBuffers();
    glDisable(GL_TEXTURE_2D);
}

void move(int i) {

    if (doMove) {
        Player::player->moveForward();

        // Move all the opponents
        Opponent::moveForwardAll();

        Player::player->checkOpponentCollision();
        // TODO: Check for collision with player
        /*
        list< Opponent* >::iterator it = opponents.begin();
        for (; it != opponents.end(); ++it) {
            // Check for collision with player
            if ((*it)->checkCollision(player)) {
                score += 100;
            }
        }
        */
        
        // Check for projectile collisions with opponents
        Player::player->checkOpponentHit();

        // Generate new opponents
        Opponent::generate();
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
        Player::player->moveLaterally(1);
    } else if (key == GLUT_KEY_LEFT) {
        Player::player->moveLaterally(-1);
    } else if (key == GLUT_KEY_UP) {
        doMove = !doMove;
    } else if (key == GLUT_KEY_DOWN) {
        Player::player->fire();
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
