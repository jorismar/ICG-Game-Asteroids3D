#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

#include "err_handler.h"
#include "model3d.h"

//************************ CONFIG ************************

#define WINDOW_WIDTH 1360					// Window width resolution
#define	WINDOW_HEIGTH 768					// Window heigth resolution

#define LASER_MOVE_SPEED		10.0f		// Lasers movement speed
#define SCENE_ROTATE_SPEED		2.0f		// Scene rotate speed
#define SPACESHIP_ROTATE		8.0f		// Spaceship rotate speed
#define SPACESHIP_MOVE_SPEED	8.0f		// Spaceship movement speed

#define LIMITE_FLY_ZONE			400.0f		// Flight zone limit in each direction
#define SPACESHIP_Z_POSITION	700.0f		// Spaceship start Z position
#define MAX_LASERS_SHOTS		1000		// Max lasers shots (restored after reaching the distance limit - Z = 0)

//************************ STRUCT ************************ <<--- Implementar classes mais tarde

struct Laser {
	double x;
	double y;
	double z;
	double rot;
};

//************************ MODELS ************************

Model3D * bg = new Model3D();			// Background
Model3D * laser = new Model3D();		// Lasers
Model3D * crosshair = new Model3D();	// Crosshair
Model3D * spaceship = new Model3D();	// Spaceship
Model3D * asteroid = new Model3D();		// Asteroids

//************************ GLOBAL ************************

double spaceship_rot_x = 0.0f;			// Spaceship rotation on X
double spaceship_rot_y = 0.0f;			// Spaceship rotation on Y
double spaceship_rot_z = 0.0f;			// Spaceship rotation on Z

double scene_rot_z = 0.0f;				// Scene rotation on Z

double spaceship_transl_x = 0.0f;		// Spaceship move on X
double spaceship_transl_y = 0.0f;		// Spaceship move on Y

Laser lasers_pos[MAX_LASERS_SHOTS];		// Lasers List
int laser_count = 0;					// Lasers count
int laser_list_first = 0;				// Last Laser Shot still alive

//***********************************************************************************************************************

void shot() {
	lasers_pos[laser_count].x = laser_count % 2 == 0 ? -(spaceship_transl_x + 3.3f) : -(spaceship_transl_x - 3.3f);
	lasers_pos[laser_count].y = -spaceship_transl_y;
	lasers_pos[laser_count].z = SPACESHIP_Z_POSITION - 2.0f;
	lasers_pos[laser_count].rot = scene_rot_z;
	laser_count = (laser_count + 1) % MAX_LASERS_SHOTS;
}

//***********************************************************************************************************************

void keyPressed(unsigned char key, int x, int y) {
	if (key == 'w') {
		spaceship_rot_x = 360.0f - SPACESHIP_ROTATE;
		spaceship_transl_y += spaceship_transl_y < LIMITE_FLY_ZONE ? SPACESHIP_MOVE_SPEED : 0.0f;
	}
	else if (key == 's') {
		spaceship_rot_x = SPACESHIP_ROTATE;
		spaceship_transl_y += spaceship_transl_y > -LIMITE_FLY_ZONE ? -SPACESHIP_MOVE_SPEED : 0.0f;
	}
	else if (key == 'a') {
		spaceship_rot_z = SPACESHIP_ROTATE + 1.0f;
		scene_rot_z = scene_rot_z < 360.0f ? scene_rot_z + SCENE_ROTATE_SPEED : scene_rot_z - 360.0f;
	}
	else if (key == 'd') {
		spaceship_rot_z = 360.0f - SPACESHIP_ROTATE + 1.0f;
		scene_rot_z = scene_rot_z > 0.0f ? scene_rot_z - SCENE_ROTATE_SPEED : 360.0f - scene_rot_z;
	}
	else if (key == 'q') {
		spaceship_rot_y = SPACESHIP_ROTATE;
		spaceship_transl_x += spaceship_transl_x < LIMITE_FLY_ZONE ? SPACESHIP_MOVE_SPEED : 0.0f;
	}
	else if (key == 'e') {
		spaceship_rot_y = 360.0f - SPACESHIP_ROTATE;
		spaceship_transl_x += spaceship_transl_x > -LIMITE_FLY_ZONE ? -SPACESHIP_MOVE_SPEED : 0.0f;
	}
	else if (key == 'j')
		shot();

	glutPostRedisplay();
}

//***********************************************************************************************************************

void keyRelease(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'q' || key == 'e') {
		spaceship_rot_x = 0.0f;
		spaceship_rot_y = 0.0f;
		spaceship_rot_z = 0.0f;
	}

	glutPostRedisplay();
}

//***********************************************************************************************************************

void OnMouseClick(int button, int state, int x, int y) {
	if (button == 0)
		shot();

	glutPostRedisplay();
}

//***********************************************************************************************************************

void renderLaser() {
	unsigned int i = laser_list_first;

	while (lasers_pos[i].z > 0.0f && i != laser_count) {
		glPushMatrix(); {
			glRotatef(360.0f - lasers_pos[i].rot, 0.0f, 0.0f, 1.0f);

			glTranslatef(lasers_pos[i].x, lasers_pos[i].y, lasers_pos[i].z);

			lasers_pos[i].z -= LASER_MOVE_SPEED;

			if (lasers_pos[i].z <= 0.0f)
				laser_list_first = (laser_list_first + 1) % MAX_LASERS_SHOTS;

			i++;
			laser->render();
		} glPopMatrix();
	}
}

//***********************************************************************************************************************

void renderSpaceship() {
	glPushMatrix(); {
		glTranslatef(0.0f, 0.0f, SPACESHIP_Z_POSITION);

		glRotatef(12.0f, 1.0f, 0.0f, 0.0f);

		if (spaceship_rot_x > 0 || spaceship_rot_y > 0 || spaceship_rot_z > 0) {
			glRotatef(spaceship_rot_x, 1.0f, 0.0f, 0.0f);
			glRotated(spaceship_rot_y, 0.0f, 1.0f, 0.0f);
			glRotated(spaceship_rot_z, 0.0f, 0.0f, 1.0f);
		}

		spaceship->render();
	} glPopMatrix();
}

//***********************************************************************************************************************

void renderBackground() {
	glPushMatrix(); {
		bg->render();
	} glPopMatrix();
}

//***********************************************************************************************************************

void renderCrosshair() {
	glPushMatrix(); {
		glTranslatef(0.0f, 9.0f, SPACESHIP_Z_POSITION);
		crosshair->render();
	} glPopMatrix();

}

//***********************************************************************************************************************

void sceneTransformations() {
	glTranslatef(spaceship_transl_x, spaceship_transl_y, 0.0f);

	if (scene_rot_z != 0)
		glRotatef(scene_rot_z, 0.0f, 0.0f, 1.0f);
}

//***********************************************************************************************************************

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset MV Matrix
	
	gluLookAt(0.0f, 10.0f, SPACESHIP_Z_POSITION + 40.0f,	// posição da câmera
		0.0f, 0.0f, 0.0f,									// ponto para o qual a camera está olhando
		0.0f, 1.0f, 0.0f									// vetor "up"
	);

	renderSpaceship();
	renderCrosshair();

	sceneTransformations();

	renderLaser();
	renderBackground();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	lasers_pos[0].z = 0.0f;

	if (!bg->importFrmFile("models/bg.obj")) {
		printErr("Model Import");
		return 1;
	}

	if (!crosshair->importFrmFile("models/crosshair.obj")) {
			printErr("Model Import");
			return 1;
		}

	if (!laser->importFrmFile("models/laser.obj")) {
		printErr("Model Import");
		return 1;
	}

	if (!spaceship->importFrmFile("models/spaceship_specular.obj")) {
		printErr("Model Import");
		return 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(".: Asteroids 3D :.");
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(OnMouseClick);

	if (!bg->loadTexture()) {
		printDbgErr("Load texture failed.");
		return 1;
	}

	if (!crosshair->loadTexture()) {
		printDbgErr("Load texture failed.");
		return 1;
	}

	if (!laser->loadTexture()) {
		printDbgErr("Load texture failed.");
		return 1;
	}

	if (!spaceship->loadTexture()) {
		printDbgErr("Load texture failed.");
		return 1;
	}
	
	bg->scale(2.2, 2.2, 2.2);
	spaceship->scale(0.7, 0.7, 0.7);
	laser->scale(1.2, 1.2, 1.2);
	crosshair->scale(0.7f, 0.7f, 0.7f);

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMotionFunc(motion);
	//glutSpecialFunc(special);

	//atexit(free_mem);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGTH);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)WINDOW_WIDTH / (GLfloat)WINDOW_HEIGTH, 0.1f, 1500.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
	glutMainLoop();

    return 0;
}

