#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "err_handler.h"
#include "model3d.h"

#define WINDOW_WIDTH 1024
#define	WINDOW_HEIGTH 768

Model3D * model = new Model3D();

double space_ship_rot_x = 0.0f;
double space_ship_rot_y = 0.0f;
double space_ship_rot_z = 0.0f;

double space_ship_transl_x = 0.0f;
double space_ship_transl_y = 0.0f;
double space_ship_transl_z = 0.0f;

void display(void) {
	GLfloat aspect = 1.0f;

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGTH);

	//glDisable(GL_LIGHTING);
	//glEnable(GL_TEXTURE_2D);

	glColor3f(1.0, 1.0, 1.0); // Green (text color)

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, aspect, 0.01f, 10.0f);

	//***** Manobras da nave 
	//***** (definir valores máximos) (zerar quando botão não estiver mais pressionado)
	//model->rotate(space_ship_rot_x, 1.0f, 0.0f, 0.0f);
	//model->rotate(space_ship_rot_y, 0.0f, 1.0f, 0.0f);
	//model->rotate(space_ship_rot_z, 0.0f, 0.0f, 1.0f);
	//model->translate(space_ship_transl_x, space_ship_transl_y, space_ship_transl_z);

	//model->render();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
}

int main(int argc, char **argv)
{
	if (!model->importFrmFile("spider.obj")) {
		printErr("");
		return 1;
	}
/*	
	if (model->loadTexture() < 0) {
		printErr("");
		return 1;
	}
*/
	model->scale(0.5, 0.5, 0.5);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGTH);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(".: Asteroids 3D :.");

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);

	GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_position[] = { -100.0f, 100.0f, 100.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);

	//GLfloat material_ambient[]   = { 0.2f, 0.1f, 0.1f, 1.0f};
	//GLfloat material_diffuse[]   = { 1.0f, 0.1f, 0.1f, 1.0f};
	//GLfloat material_specular[]  = { 1.0f, 1.0f, 1.0f, 1.0f};

	GLfloat material_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat material_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat material_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
	glMaterialf(GL_FRONT, GL_SHININESS, 128.0f);

	glShadeModel(GL_SMOOTH);

	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutMouseFunc(mouse);
	//glutMotionFunc(motion);
	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(special);

	//atexit(free_mem);

	glutMainLoop();

    return 0;
}

