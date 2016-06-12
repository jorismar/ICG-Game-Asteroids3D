#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <map>
#include <cmath>
#include <thread>
#include <cstdlib>

#include "err_handler.h"
#include "utils.h"
#include "game.h"

std::map<unsigned char, KeyboardEvent> keyboard;

//************************ CONFIG ************************

#define WINDOW_WIDTH			1360.0f		// Window width resolution
#define	WINDOW_HEIGTH			768.0f			// Window heigth resolution

//***********************************************************************************************************************
void regKeys() {
	keyboard['s'] = { false, &spaceshipMoveUp, &spaceshipReleaseMoveUp };
	keyboard['w'] = { false, &spaceshipMoveDown, &spaceshipReleaseMoveDown };
	//keyboard['q'] = { false, &spaceshipMoveLeft, &spaceshipReleaseMoveLeft };
	//keyboard['e'] = { false, &spaceshipMoveRight, &spaceshipReleaseMoveRight };
	keyboard['a'] = { false, &spaceshipMoveLeft, &spaceshipReleaseMoveLeft };
	keyboard['d'] = { false, &spaceshipMoveRight, &spaceshipReleaseMoveRight };
	keyboard['j'] = { false, &spaceshipShootLasers, NULL };
}

void keyPressed(unsigned char key, int x, int y) {
	std::map<unsigned char, KeyboardEvent>::iterator it = keyboard.find(key);
	
	// Check if key is valid
	if(it != keyboard.end())
		keyboard[key].is_pressed = true;

	glutPostRedisplay();
}

//***********************************************************************************************************************

void keyRelease(unsigned char key, int x, int y) {
	std::map<unsigned char, KeyboardEvent>::iterator it = keyboard.find(key);

	// Check if key is valid
	if (it != keyboard.end())
		keyboard[key].is_pressed = false;

	glutPostRedisplay();
}

//***********************************************************************************************************************

void OnMouseClick(int button, int state, int x, int y) {
	if (button == 0)
		spaceshipShootLasers();

	glutPostRedisplay();
}

//*******************************************************************************************************************************

void interact() {
	if (keyboard['w'].is_pressed && (keyboard['w'].pressed != NULL))
		keyboard['w'].pressed();
	else if (keyboard['s'].is_pressed && (keyboard['s'].pressed != NULL))
		keyboard['s'].pressed();
	else if (keyboard['w'].released != NULL)
		keyboard['w'].released();
	else if (keyboard['s'].released != NULL)
		keyboard['s'].released();
	
	if (keyboard['a'].is_pressed && (keyboard['a'].pressed != NULL))
		keyboard['a'].pressed();
	else if (keyboard['d'].is_pressed && (keyboard['d'].pressed != NULL))
		keyboard['d'].pressed();
	else if (keyboard['a'].released != NULL)
		keyboard['a'].released();
	else if (keyboard['d'].released != NULL)
		keyboard['d'].released();

	if (keyboard['j'].is_pressed && (keyboard['j'].pressed != NULL))
		keyboard['j'].pressed();
}

//*******************************************************************************************************************************

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset MV Matrix
	
	interact();
	runGame();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
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

	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[] = { 10.0f, 30.0f, 0.0f, 0.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);
}

int main(int argc, char **argv)
{
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

	glutMainLoop();

    return 0;
}

