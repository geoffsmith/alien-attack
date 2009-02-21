/**
 * TODO:
 *  * Enable back face culling for speed enhancement. Need to sort out particle
 *    normals first though.
 */
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
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
//static int pause_time = 20; // milliseconds
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

void initSDLAudio() {
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) < 0) {
        cout << "Error initialising SDL audio" << endl;
    }
    Mix_Volume(-1, MIX_MAX_VOLUME);
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

    glDisable(GL_TEXTURE_2D);
}

void move() {

    if (doMove) {
        Player::player->moveForward();
        Player::player->moveLaterally();

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
}

void handleKeyboard() {
    // Poll for events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        Player::player->moveLeft();
                        break;
                    case SDLK_RIGHT:
                        Player::player->moveRight();
                        break;
                    case SDLK_SPACE:
                        Player::player->fire();
                        break;
                    case SDLK_p:
                        doMove = !doMove;
                        break;
                    default:
                        // do nothing
                        break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        Player::player->cancelMoveLeft();
                        break;
                    case SDLK_RIGHT:
                        Player::player->cancelMoveRight();
                        break;
                    case SDLK_ESCAPE:
                        exit(0);
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                exit(0);
        }
    }
}

int main(int argc, char** argv) {
    int error = SDL_Init(SDL_INIT_EVERYTHING);

    if (error < 0) {
        cout << "Unable to init SDL: " << SDL_GetError();
        exit(1);
    }
    atexit(SDL_Quit);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_Surface *drawContext;
    const SDL_VideoInfo *info = SDL_GetVideoInfo();
    int bpp = info->vfmt->BitsPerPixel;
    drawContext = SDL_SetVideoMode(500, 500, bpp, SDL_OPENGL);

    reshape(500, 500);

    if (drawContext == 0) {
        cout << "Failed to initialise video" << endl;
        exit(1);
    }

    initSDLAudio();
    init();
    init_objects();

    // Enter the main look
    while (1) {
        // Take input
        handleKeyboard();

        // Make the movements
        move();

        // Draw the scene
        display();

        // Swap the buffers
        SDL_GL_SwapBuffers();

        SDL_Delay(20);
    }
    return 0;
}
