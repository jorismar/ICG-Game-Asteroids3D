#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <thread>
#include <cstdlib>

#include "err_handler.h"
#include "Model3D.h"
#include "Audio.h"
#include "Spaceship.h"
#include "Laser.h"
#include "Background.h"
#include "Shader.h"

//************************ CONFIG ************************

#define WINDOW_WIDTH			1360		// Window width resolution
#define	WINDOW_HEIGTH			768			// Window heigth resolution

#define LASER_MOVE_SPEED		10.0f		// Lasers movement speed 10
#define SCENE_ROTATE_SPEED		0.5f		// Scene rotate speed
#define SPACESHIP_ROTATE		8.0f		// Spaceship rotate speed
#define SPACESHIP_MOVE_SPEED	1.0f		// Spaceship movement speed

#define LIMITE_FLY_ZONE			100			// Flight zone limit in each direction
#define SPACESHIP_Z_POSITION	700.0f		// Spaceship start Z position
#define MAX_LASERS_SHOTS		1000		// Max lasers shots (restored after reaching the distance limit - Z = 0)
#define MAX_ASTEROIDS_FLYING	200

#define ASTEROID_BASE_HARDNESS	10
#define ASTEROID_BASE_HEALTH	10
#define ASTEROID_BASE_SPEED		2.0f
#define SCORE_BASE_POINTS		10


Model3D * crosshair = new Model3D();	// Crosshair
Model3D * timesep	= new Model3D();
Model3D * numbers	= new Model3D[10];
Model3D * specials	= new Model3D();
Model3D * life		= new Model3D();
Model3D * asteroid1 = new Model3D();

//***********************************************************************************************************************

Laser*		laser = new Laser();			// Laser object
Spaceship*	spaceship = new Spaceship();	// Spaceship object
Background* background = new Background();	// Environment object

typedef struct {
	bool		alive;
	Angle		rot;
	Position	pos;
} Scene, Shot;

struct Asteroid {
	Angle		rot;
	Position	pos;
	double		size;
	double		speed;
	double		health;
	double		hardness;
	double		rot_animation;
};

// Lasers controls
Shot shots_list[MAX_LASERS_SHOTS];	// Fired shots list
bool cannon1_reloading = false;		// Cannon1 charge condition
bool cannon2_reloading = false;		// Cannon2 charge condition
bool side = true;					// Cannon selector: true = cannon 1, false = cannon 2
unsigned int shots_first_pos = 0;			// Last Laser Shot still alive
unsigned int shots_last_pos = 0;				// Shots count

// Asteroids controls
Asteroid astrd_list[MAX_ASTEROIDS_FLYING];
unsigned int astrd_first_pos = 0;
unsigned int astrd_last_pos = 0;

// Game controller
Scene scene;// = { {0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f } };						// Scene attributes
bool started = false;				// Game started flag
unsigned int special_count = 0;		// Specials count
unsigned int life_count = 3;		// Life count
unsigned int score = 0;				// Score points

double scene_z = 0.0f;
//***********************************************************************************************************************

void sleep(int time) {
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

//***********************************************************************************************************************
double adapter = static_cast <double> (RAND_MAX / (LIMITE_FLY_ZONE * 2));		// Adjusting the operation to get the positive and negative results.
double angle_adapter = RAND_MAX / 360.0f;
double size_adapter = RAND_MAX / 6.0f;
double difficulty = 5;
bool asteroid_launched = false;

void launcher() {
	astrd_list[astrd_last_pos].pos.x = LIMITE_FLY_ZONE - static_cast <double> (rand()) / adapter;	// Random value of range -400 ~ 400
	astrd_list[astrd_last_pos].pos.y = LIMITE_FLY_ZONE - static_cast <double> (rand()) / adapter;	// Random value of range -400 ~ 400
	astrd_list[astrd_last_pos].pos.z = 0.0f;
	astrd_list[astrd_last_pos].rot.z = scene_z;
	astrd_list[astrd_last_pos].rot_animation = static_cast <double> (rand()) / angle_adapter;		// Random value of range 0 ~ 360
	astrd_list[astrd_last_pos].size = (1.0f + static_cast <double> (rand()) / size_adapter) / 2.0f;	// Random value of range 1 ~ 3
	astrd_list[astrd_last_pos].hardness = ASTEROID_BASE_HARDNESS * astrd_list[astrd_last_pos].size;
	astrd_list[astrd_last_pos].health = ASTEROID_BASE_HEALTH * astrd_list[astrd_last_pos].size;
	astrd_list[astrd_last_pos].speed = ASTEROID_BASE_SPEED * difficulty;

	// Update the next position
	astrd_last_pos = (astrd_last_pos + 1) % MAX_ASTEROIDS_FLYING;

	asteroid_launched = true;
}

void renderAsteroids() {
	if (!asteroid_launched)
		return;

	for (int i = astrd_first_pos; i != astrd_last_pos; i = (i + 1) % MAX_ASTEROIDS_FLYING) {
		// Render the asteroid model
		glPushMatrix(); {
			glRotatef(360.0f - astrd_list[i].rot.z, 0.0f, 0.0f, 1.0f);
			glTranslatef(astrd_list[i].pos.x, astrd_list[i].pos.y, astrd_list[i].pos.z);
			glRotatef(astrd_list[i].rot_animation, 1.0f, 1.0f, 1.0f);
			asteroid1->scale(astrd_list[i].size, astrd_list[i].size, astrd_list[i].size);
			astrd_list[i].rot_animation = astrd_list[i].rot_animation < 360.0f ? astrd_list[i].rot_animation + 0.25f : 0.0f;
			asteroid1->render();
		} glPopMatrix();

		// Update the next position on Z axis of this shot
		astrd_list[i].pos.z += astrd_list[i].speed;

		// Check if the laser reaches the limit of the scene
		if (/*collisionTest() ||*/ astrd_list[i].pos.z > SPACESHIP_Z_POSITION + 10.0f) {
			astrd_first_pos = (astrd_first_pos + 1) % MAX_LASERS_SHOTS;
			
			if (astrd_first_pos == astrd_last_pos)
				asteroid_launched = false;
		}
	}
}

//***********************************************************************************************************************

void shooter() {
	// Check if the current cannon are reloading
	if ((cannon1_reloading && side) || (cannon2_reloading && !side))
		return;

	// Set the laser attributes
	shots_list[shots_last_pos].pos.x = side ? -(scene.pos.x - 3.9f) : -(scene.pos.x + 3.9f);
	shots_list[shots_last_pos].pos.y = -scene.pos.y;
	shots_list[shots_last_pos].pos.z = spaceship->getPosition().z - 3.0f;
	shots_list[shots_last_pos].rot.z = scene_z;

	// Update the next position
	shots_last_pos = (shots_last_pos + 1) % MAX_LASERS_SHOTS;
	
	// Recharging the left laser cannon.
	if (side) {			
		cannon1_reloading = side;
		std::thread cannon1([=]() {sleep(200); cannon1_reloading = false;});
		cannon1.detach();
	}
	// Recharging the right laser cannon.
	else {				
		cannon2_reloading = !side;
		std::thread cannon2([=]() {sleep(200); cannon2_reloading = false;});
		cannon2.detach();
	}

	side = !side;		// Change cannon
	laser->event(0);	// Call the laser event.
}

//***********************************************************************************************************************

void shotsManager() {
	for (int i = shots_first_pos; i != shots_last_pos; i = (i + 1) % MAX_LASERS_SHOTS) {
		// Render the laser model
		laser->dynamicRender(shots_list[i].pos, { 0.0f, 0.0f, 360.0f - shots_list[i].rot.z });

		// Update the next position on Z axis of this shot
		shots_list[i].pos.z -= laser->getSpeed();			
		
		// Check if the laser reaches the limit of the scene
		if(shots_list[i].pos.z <= 0.0f)
			shots_first_pos = (shots_first_pos + 1) % MAX_LASERS_SHOTS;
	}
}

//***********************************************************************************************************************

unsigned int timeboard[6];

void timer() {
	for (timeboard[5] = 0; timeboard[5] < 6; timeboard[5]++)	// Minutes
		for (timeboard[4] = 0; timeboard[4] < 10; timeboard[4]++)	// Minutes
			for (timeboard[3] = 0; timeboard[3] < 6; timeboard[3]++)	// Secounds
				for (timeboard[2] = 0; timeboard[2] < 10; timeboard[2]++)	// Secounds
					for (timeboard[1] = 0; timeboard[1] < 10; timeboard[1]++)	// Milissecounds
						sleep(100);
}

//***********************************************************************************************************************

void renderTimer() {
	double pos = 13.2f;

	for (unsigned int i = 0; i < 6; i++) {
		glPushMatrix(); {
			if (i == 2 || i == 4) {
				pos += 0.4f;
				glPushMatrix(); {
					glTranslatef(pos, 16.9f, 0.0f);
					timesep->render();
				} glPopMatrix();
				pos -= 0.7f;
			}

			glTranslatef(pos, 16.9f, 0.0f);
			numbers[timeboard[i]].render();
			pos -= 0.7f;
		} glPopMatrix();
	}
}

//***********************************************************************************************************************

unsigned int life_multiplier = 10;
unsigned int scoreboard[7];

void scorer(int sum) {
	score += sum;

	if (score >= life_multiplier && life_count < 5) {
		life_count++;
		life_multiplier *= 10;
	}

	int aux = score;

	for(unsigned int i = 0; i < 7; i++) {
		scoreboard[i] = aux % 10;
		aux /= 10;
	}
}

//***********************************************************************************************************************

void renderScore() {
	double pos = -13.7f;

	for (int i = 0; i < 7; i++) {
		glPushMatrix(); {
			glTranslatef(pos, 16.9f, 0.0f);
			numbers[scoreboard[6 - i]].render();
			pos += 0.7f;
		} glPopMatrix();
	}
}

//***********************************************************************************************************************
double spaceship_speed = SPACESHIP_MOVE_SPEED;

void keyPressed(unsigned char key, int x, int y) {
	if (key == 'w') {
		spaceship->event(0);
		scene.pos.y += scene.pos.y < LIMITE_FLY_ZONE ? spaceship_speed : 0.0f;
	}
	
	if (key == 's') {
		spaceship->event(1);
		scene.pos.y += scene.pos.y > -LIMITE_FLY_ZONE ? -spaceship_speed : 0.0f;
	}

	if (key == 'a') {
		spaceship->event(2);
		scene_z = scene_z > 0.0f ? scene_z - spaceship_speed : 360.0f + scene_z - spaceship_speed;
	}
	
	if (key == 'd') {
		spaceship->event(3);
		scene_z = scene_z < 360.0f ? scene_z + spaceship_speed : scene_z - 360.0f + spaceship_speed;
	}
	
	if (key == 'q') {
		spaceship->event(4);
		scene.pos.x += scene.pos.x < LIMITE_FLY_ZONE ? spaceship_speed : 0.0f;
	}
	
	if (key == 'e') {
		spaceship->event(5);
		scene.pos.x += scene.pos.x > -LIMITE_FLY_ZONE ? -spaceship_speed : 0.0f;
	}
	
	if (key == 'j') {
		shooter();
		scorer(1);
	}

	if (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'q' || key == 'e')
		spaceship_speed += spaceship_speed < 10.0f ? SPACESHIP_MOVE_SPEED : 0.0f;

	glutPostRedisplay();
}

//***********************************************************************************************************************

void keyRelease(unsigned char key, int x, int y) {
	if (key == 'w' || key == 'a' || key == 's' || key == 'd' || key == 'q' || key == 'e') {
		spaceship->event(6);
		spaceship_speed = SPACESHIP_MOVE_SPEED;
	}

	glutPostRedisplay();
}

//***********************************************************************************************************************

void OnMouseClick(int button, int state, int x, int y) {
	if (button == 0) {
		shooter();
		scorer(1);
	}

	glutPostRedisplay();
}

//***********************************************************************************************************************/

void renderCrosshair() {
	glPushMatrix(); {
		glTranslatef(0.0f, 9.6f, 0.0f);
		crosshair->render();
	} glPopMatrix();

}

//***********************************************************************************************************************

void sceneTransformations() {
	glTranslatef(scene.pos.x, scene.pos.y, 0.0f);

	if (scene_z != 0.0f)
		glRotatef(scene_z, 0.0f, 0.0f, 1.0f);
}

//***********************************************************************************************************************

void renderLifeboard() {
	double pos = -13.4f;

	for (int i = 0; i < life_count; i++) {
		glPushMatrix(); {
			glTranslatef(pos, 16.2f, 0.0f);
			life->render();
			pos += 0.9f;
		} glPopMatrix();
	}
}

//***********************************************************************************************************************

void renderSpecialsBoard() {
	double pos = 13.3f;

	for (int i = 0; i < special_count; i++) {
		glPushMatrix(); {
			glTranslatef(pos, 16.1f, 0.0f);
			//glTranslatef(pos, 16.1f, SPACESHIP_Z_POSITION + 20);
			specials->render();
			pos -= 0.9f;
		} glPopMatrix();
	}
}

void renderGUI() {
	glPushMatrix(); {
		glTranslatef(0.0f, 0.0f, SPACESHIP_Z_POSITION + 20);

		renderCrosshair();
		renderScore();
		renderTimer();
		renderLifeboard();
		renderSpecialsBoard();

	} glPopMatrix();
}

//***********************************************************************************************************************

bool collisionTest(Object * obj1, Object * obj2) {
	//obj1->getBoundingBox(0).higher.z < obj2->getBoundingBox(0).
	return true;
}

//*******************************************************************************************************************************

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset MV Matrix
	
	gluLookAt(0.0f, 10.0f, SPACESHIP_Z_POSITION + 40.0f,	// posição da câmera
		0.0f, 0.0f, 0.0f,									// ponto para o qual a camera está olhando
		0.0f, 1.0f, 0.0f									// vetor "up"
	);

	renderGUI();

	spaceship->render();

	glPushMatrix(); {
		sceneTransformations();

		renderAsteroids();
	
		shotsManager();
		background->render();
	} glPopMatrix();

	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);				// Depth Buffer Setup

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

	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 0.5f };
	GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[] = { 10.0f, 30.0f, 0.0f, 0.0f };

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);

	if (!crosshair->importFrmFile("resource/models/crosshair.obj")) { printErr("Model Import"); return 1; }

	if (!numbers[0].importFrmFile("resource/models/0.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[1].importFrmFile("resource/models/1.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[2].importFrmFile("resource/models/2.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[3].importFrmFile("resource/models/3.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[4].importFrmFile("resource/models/4.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[5].importFrmFile("resource/models/5.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[6].importFrmFile("resource/models/6.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[7].importFrmFile("resource/models/7.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[8].importFrmFile("resource/models/8.obj")) { printErr("Model Import"); return 1; }
	if (!numbers[9].importFrmFile("resource/models/9.obj")) { printErr("Model Import"); return 1; }
	if (!timesep->importFrmFile("resource/models/timesep.obj")) { printErr("Model Import"); return 1; }

	if (!specials->importFrmFile("resource/models/star.obj")) { printErr("Model Import"); return 1; }
	if (!life->importFrmFile("resource/models/heart.obj")) { printErr("Model Import"); return 1; }

	if (!crosshair->loadTexture()) { printDbgErr("Load texture failed."); return 1; }

	if (!numbers[0].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[1].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[2].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[3].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[4].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[5].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[6].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[7].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[8].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!numbers[9].loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!timesep->loadTexture()) { printDbgErr("Load texture failed."); return 1; }

	if (!specials->loadTexture()) { printDbgErr("Load texture failed."); return 1; }
	if (!life->loadTexture()) { printDbgErr("Load texture failed."); return 1; }

	crosshair->scale(0.55f, 0.55f, 0.55f);
	life->scale(0.21f, 0.21f, 0.21f);
	specials->scale(0.25f, 0.25f, 0.25f);

	//********************** FILES ***********************

	std::string models_folder = "resource/models/";
	std::string sounds_folder = "resource/sound/sfx/";

	std::string spaceship_obj_filename	= models_folder + "spaceship3.obj";
	std::string laser_obj_filename		= models_folder + "laser.obj";
	std::string bg_obj_filename			= models_folder + "bg.obj";
	std::string asteroid1_obj_filename	= models_folder + "asteroids.obj";

	std::string spaceship_sfx_filename	= sounds_folder + "game/spaceship/gas_stove_fire_loop.wav";
	std::string laser_sfx_filename		= sounds_folder + "game/spaceship/laser_blast_01.wav";
	std::string bg_sfx_filename			= sounds_folder + "game/environment/target_balls_cloud_fat_loop.wav";

	//*********************** LOAD ***********************

	laser	   -> load( &laser_obj_filename,	 1, &laser_sfx_filename,	 1 );	// Laser
	if (!spaceship  -> load( &spaceship_obj_filename, 1, &spaceship_sfx_filename, 1 )) { printErr("Spaceship import"); return 1; };	// Spaceship
	background -> load( &bg_obj_filename,		 1, &bg_sfx_filename,		 1 );	// Background and Enviroment
	if (!asteroid1->importFrmFile(asteroid1_obj_filename)) { printErr("Asteroid1 import"); return 1; }
	if (!asteroid1->loadTexture()) { printDbgErr("Asteroid1 texture load failed"); return 1; }

	//********************** CONFIG **********************

	// Spaceship
	spaceship->setPosition(0.0f, 0.0f, SPACESHIP_Z_POSITION);
	spaceship->setScale(0.65f);
	spaceship->setRotation(12.0f, true, false, false);
	spaceship->setAnimationRotateInc(8.0f);

	// Laser
	laser->setScale(1.3f);
	laser->setSpeed(10.0f);

	// Backgroud
	background->setScale(1.8f);

	// Asteroid 1
	asteroid1->scale(0.5f, 0.5f, 0.5f);

	//********************** START ***********************

	spaceship->start();
	background->start();

	laser	  ->getAudio(0)->setVolume(25);
	background->getAudio(0)->setVolume(15);
	spaceship ->getAudio(0)->setVolume(75);

	std::thread launchAsteroid(
		[=]() {
			while (true) {
				launcher();
				sleep(3500 / difficulty);
			}
		}
	);

	launchAsteroid.detach();

	std::thread tm(timer);
	tm.detach();

	//****************************************************

	glutMainLoop();

    return 0;
}
