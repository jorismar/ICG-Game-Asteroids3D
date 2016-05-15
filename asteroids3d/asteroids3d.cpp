#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "err_handler.h"
#include "model3d.h"

#define WINDOW_WIDTH 640
#define	WINDOW_HEIGTH 480

Model3D * model = new Model3D();

GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 0.0f };

double space_ship_rot_x = 0.0f;
double space_ship_rot_y = 0.0f;
double space_ship_rot_z = 0.0f;

double space_ship_transl_x = 0.1f;
double space_ship_transl_y = 0.1f;
double space_ship_transl_z = 0.1f;

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset MV Matrix

	gluLookAt(	0.0f, 50.0f, 50.0f,	// posição da câmera
				0.0f, 0.0f, 0.0f,	// ponto para o qual a camera está olhando
				0.0f, 1.0f, 0.0f);	// vetor "up"

	//***** Manobras da nave 
	//***** (definir valores máximos) (zerar quando botão não estiver mais pressionado)

	glRotatef(space_ship_rot_x, 1.0f, 0.0f, 0.0f);
	glRotatef(space_ship_rot_y, 0.0f, 1.0f, 0.0f);
	glRotatef(space_ship_rot_z, 0.0f, 0.0f, 1.0f);

	space_ship_rot_y = space_ship_rot_y < 359.0f ? space_ship_rot_y + 0.2f : 0.0f;

	model->render();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	if (!model->importFrmFile("spider.obj")) {
		printErr("Model Import");
		return 1;
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(".: Asteroids 3D :.");

	if (model->loadTexture() < 1) {
		printDbgErr("Texture load");
		return 1;
	}

	model->scale(0.5, 0.5, 0.5);

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(special);

	//atexit(free_mem);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGTH, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	glutMainLoop();

    return 0;
}

