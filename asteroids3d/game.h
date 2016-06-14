#ifndef JB_ASTEROIDS3D_GAME_H
#define JB_ASTEROIDS3D_GAME_H

#ifdef WIN32
	#include <windows.h>
#endif

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <string>

#include "utils.h"
#include "Object3D.h"
#include "err_handler.h"

// ***********************************************************************************************************************
//	GAME CONFIGs
// ***********************************************************************************************************************

// Limite flying zone of the scene
#define SCENE_LIMITE_FRONT			   0.0f	// Minimum Z translate
#define SCENE_LIMITE_BACK			 740.0f	// Maximum Z translate
#define SCENE_LIMITE_LEFT			-150.0f	// Minimum X translate
#define SCENE_LIMITE_RIGHT			 150.0f	// Maximum X translate
#define	SCENE_LIMITE_DOWN			-150.0f	// Minimum Y translate
#define SCENE_LIMITE_UP				 150.0f	// Maximum Y translate

// Difficulty Levels
#define DIFFICULTY_LEVEL_VERY_EASY	0x001	// Very Easy
#define DIFFICULTY_LEVEL_EASY		0x002	// Easy
#define DIFFICULTY_LEVEL_NORMAL		0x003	// Normal
#define DIFFICULTY_LEVEL_HARD		0x004	// Hard
#define DIFFICULTY_LEVEL_VERY_HARD	0x005	// Very Hard

// Maximum entities in the scene by type.
#define MAX_LF						0x3E8	// Max lasers flying
#define MAX_AF						0x3E8	// Max asteroids flying
#define MAX_HP						0x005	// Max level of health points
#define MAX_SP						0x005	// Max level of specials
#define MAX_NL						0x007	// Max level of score numerical order

// Base Multipliers
#define SCORE_BASE_POINTS			0x002	// Multiplier base score
#define LASER_BASE_SPEED			10.0f	// Multiplier base of laser speed
#define ASTEROID_BASE_HARDNESS		2.0f	// Multiplier base of asteroid hardness
#define ASTEROID_BASE_HEALTH		10.0f	// Multiplier base of asteroid health
#define ASTEROID_BASE_SPEED			0.8f	// Multiplier base of asteroid speed
#define SPACESHIP_TURBO_SPEED		2.0f	// Spaceship turbo speed

// Others
#define POINTS_NEEDED_TO_HEALTH		0x3E8	// Points needed to earn a new HP level
#define SPACESHIP_ANIMATION			8.0f	// Spaceship animation rotate increment
#define LASER_DAMAGE				8.0f	// Laser damage
#define SPACESHIP_SPEED				3.0f	// Spaceship speed

// ***********************************************************************************************************************
//	STRUCT - ENTITY
// ***********************************************************************************************************************

struct Entity {
	size_t num_entities;
	Status * status;
	Object3D * object;
};

struct Camera {
	Position position;
	Position lookat;
	Position up;
};
// ***********************************************************************************************************************
//	FUNCTIONS
// ***********************************************************************************************************************

/**
 * \brief	Initiates a entity
 *
 */
void initEntity(Entity * entity, size_t num_entities);

/**
* \brief
*
*/
void resetStatus(Status * status);

/**
* \brief
*
*/
void draw(Entity entity, unsigned int index, unsigned int model_index);

/**
* \brief
*
*/
bool checkCollision(BoundingBox astrd, BoundingBox obj);

/**
* \brief
*
*/
void shootLaser();

/**
* \brief
*
*/
void createAsteroid();

/**
* \brief
*
*/
void laserManager();

/**
* \brief
*
*/
void asteroidManager();

/**
* \brief
*
*/
void scoreManager();

/**
* \brief
*
*/
void timerManager();

/**
* \brief
*
*/
void healthManager();

/**
* \brief
*
*/
void specialManager();

/**
* \brief
*
*/
void cameraManager();

/**
* \brief
*
*/
void spaceshipMoveUp();

/**
* \brief
*
*/
void spaceshipMoveDown();

/**
* \brief
*
*/
void spaceshipMoveLeft();

/**
* \brief
*
*/
void spaceshipMoveRight();

/**
* \brief
*
*/
void spaceshipTurnLeft();

/**
* \brief
*
*/
void spaceshipTurnRight();

/**
* \brief
*
*/
void spaceshipReleaseMoveUp();

/**
* \brief
*
*/
void spaceshipReleaseMoveDown();

/**
* \brief
*
*/
void spaceshipReleaseMoveLeft();

/**
* \brief
*
*/
void spaceshipReleaseMoveRight();

/**
* \brief
*
*/
void spaceshipReleaseTurnLeft();

/**
* \brief
*
*/
void spaceshipReleaseTurnRight();

/**
* \brief
*
*/
void spaceshipShootLasers();

/**
* \brief
*
*/
void pauseGame();

/**
* \brief
*
*/
bool loadGame();

/**
* \brief
*
*/
void runGame();

/**
* \brief
*
*/
void gameOver();

/**
* \brief
*
*/
void spaceshipRespawn();

void missonComplete();

// ***********************************************************************************************************************

#endif // !JB_ASTEROIDS3D_GAME_H
