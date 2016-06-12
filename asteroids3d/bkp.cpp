/*
#ifdef WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
//#include <SDL.h>
//#include <SDL2/SDL_opengl.h>
#include <map>
#include <cmath>
#include <thread>
#include <cstdlib>

#include "err_handler.h"
#include "utils.h"
#include "game.h"

SDL_Window * window;
SDL_Renderer * renderer;

std::map<unsigned int, KeyboardEvent> keyboard;

//************************ CONFIG ************************

#define WINDOW_WIDTH			1360.0f		// Window width resolution
#define	WINDOW_HEIGTH			768.0f			// Window heigth resolution
#define FULLSCREEN				false

//***********************************************************************************************************************
void regKeys() {
keyboard[22].pressed = &spaceshipMoveUp;
keyboard[26].pressed = &spaceshipMoveDown;
keyboard[4].pressed = &spaceshipMoveLeft;
keyboard[7].pressed = &spaceshipMoveRight;
//	keyboard[].pressed = &spaceshipTurnLeft;
//	keyboard[].pressed = &spaceshipTurnRight;
keyboard[13].pressed = &spaceshipShootLasers;
//keyboard[''].pressed = &pauseGame;

keyboard[22].released = &spaceshipRelease;
keyboard[26].released = &spaceshipRelease;
keyboard[4].released = &spaceshipRelease;
keyboard[7].released = &spaceshipRelease;
//keyboard[].released = &spaceshipRelease;
//keyboard[].released = &spaceshipRelease;
//keyboard['j'].released = &spaceshipShootLasers;
//keyboard[''].pressed = &pauseGame;
}
/*
void keyPressed(unsigned char key, int x, int y) {
std::map<unsigned char, KeyboardEvent>::iterator it = keyboard.find(key);

// Check if key is valid
if(it != keyboard.end())
keyboard[key].pressed();

glutPostRedisplay();
}

//***********************************************************************************************************************

void keyRelease(unsigned char key, int x, int y) {
std::map<unsigned char, KeyboardEvent>::iterator it = keyboard.find(key);

// Check if key is valid
if (it != keyboard.end())
keyboard[key].released();

glutPostRedisplay();
}

//***********************************************************************************************************************

void OnMouseClick(int button, int state, int x, int y) {
if (button == 0)
spaceshipShootLasers();

glutPostRedisplay();
}
*
//*******************************************************************************************************************************

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset MV Matrix

	runGame();

	glFlush();
	SDL_GL_SwapWindow(window);
	//SDL_RenderPresent(renderer);
	//glutSwapBuffers();
	//glutPostRedisplay();
}

void sdlEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			// Check if key is valid
			if (event.key.keysym.scancode == 26)
				spaceshipMoveDown();
			if (event.key.keysym.scancode == 22)
				spaceshipMoveUp();
			if (event.key.keysym.scancode == 4)
				spaceshipMoveLeft();
			if (event.key.keysym.scancode == 7)
				spaceshipMoveRight();
			if (event.key.keysym.scancode == 13)
				spaceshipShootLasers();
			break;
		default:
			break;
		case SDL_QUIT:
			exit(0); //Fechamos a apliação
			break;
		}
	}
}

void initOpenGL() {
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGTH);

	// Culling
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, WINDOW_WIDTH / WINDOW_HEIGTH, 0.1f, 1500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth Buffer Setup
	glClearDepth(1.0f);

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Depth Buffer Setup
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[] = { 10.0f, 30.0f, 0.0f, 0.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

int main(int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "Error on initiate SDL video: " << SDL_GetError() << std::endl;
		return 1;
	}
	//throw std::runtime_error(SDL_GetError());

	if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGTH, SDL_WINDOW_OPENGL, &window, &renderer) < 0) {
		std::cout << "Error on creating window and renderer: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	if (glcontext == NULL) {
		std::cerr << "There was an error creating OpenGL context: " << SDL_GetError() << std::endl;
		return 1;
	}

	const unsigned char *version = glGetString(GL_VERSION);
	if (version == NULL) {
		std::cerr << "There was an error with OpenGL configuration:" << std::endl;
		return 1;
	}

	SDL_RendererInfo renderer_info;

	if (SDL_GetRendererInfo(renderer, &renderer_info) < 0) {
		std::cout << "Error get renderer information: " << SDL_GetError() << std::endl;
		return 1;
	}

	/*TODO: Check that we have OpenGL *
	if ((renderer_info.flags & SDL_RENDERER_ACCELERATED) == 0 || (renderer_info.flags & SDL_RENDERER_TARGETTEXTURE) == 0) {
		std::cout << "Handle this. We have no render surface and not accelerated." << std::endl;
	}

	atexit(SDL_Quit);

	Audio::init();
	regKeys();

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//SDL_GL_MakeCurrent(window, glcontext);

	initOpenGL();

	std::cout << "Loading game..." << std::endl;
	if (!loadGame())
		return 1;

	std::cout << "Starting game..." << std::endl;

	while (true) {
		sdlEvents();
		display();
	}

	SDL_Delay(5000);
	SDL_Quit();

	/*
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(".: Asteroids 3D :.");
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(OnMouseClick);

	Audio::init();
	regKeys();

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMotionFunc(motion);
	//glutSpecialFunc(special);

	//atexit(free_mem);
	initOpenGL();

	std::cout << "Loading game..." << std::endl;
	if(!loadGame())
	return 1;

	std::cout << "Starting game..." << std::endl;

	startSDLKeyboardEvent();

	glutMainLoop();
	*
	return 0;
}




*/