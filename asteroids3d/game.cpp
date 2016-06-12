#include "game.h"

// ***********************************************************************************************************************
//	GLOBAL VARIABLES
// ***********************************************************************************************************************

// -----------------------------------------------------------------------------------------------------------------------
//	GAME CONTROLs
// -----------------------------------------------------------------------------------------------------------------------

// Game
bool		 show_bounding_boxes	= false;						// Show models bounding boxes
unsigned int current_difficulty		= DIFFICULTY_LEVEL_VERY_EASY;	// Current Difficulty Level
unsigned int needed_to_next_health	= POINTS_NEEDED_TO_HEALTH;		// Controller lives by points
unsigned int health_count			= 3;							// HP Level count
unsigned int spcial_count			= 5;							// Specials level count
unsigned int current_score			= 0;							// Current score 
unsigned int timevalue[] = { 0, 0, 0, 0, 0, 0 };					// Digits timer controller

// Laser control attributes
bool cannon_selector				= false;	// Cannon selector to be fired
bool left_cannon_charging			= false;	// Flag to indicate whether the left cannon is charging
bool right_cannon_charging			= false;	// Flag to indicate whether the left cannon is charging
unsigned int laser_list_front		= 0;		// Pointer to the first laser shot.
unsigned int laser_list_back		= 0;		// Pointer to the last laser shot.

// Asteroid control attributes
unsigned int astrd_list_front		= 0;		// Pointer to the first asteroid created
unsigned int astrd_list_back		= 0;		// Pointer to the last asteroid created

// Spaceship control attributes
bool		 respawnDraw			= false;
unsigned int respawnTime			= 100;


// -----------------------------------------------------------------------------------------------------------------------
//	ENTITIES
// -----------------------------------------------------------------------------------------------------------------------

Entity lasers;
Entity asteroids;
Entity background;
Entity spaceship;
Entity score_board;
Entity timer_board;
Entity hp_board;
Entity special_board;
Entity crosshair;
Camera camera;

// -----------------------------------------------------------------------------------------------------------------------
//	PATHs
// -----------------------------------------------------------------------------------------------------------------------

// Path models
std::string models_folder	=	"resource/models/";
std::string obj_spaceship	=	models_folder + "spaceship3.obj";	// Spaceship model file
std::string obj_asteroid	=	models_folder + "asteroids.obj";	// Asteroid model file
std::string obj_background	=	models_folder + "bg.obj";			// Background model file
std::string obj_laser		=	models_folder + "laser.obj";		// Laser model file
std::string obj_heart		=	models_folder + "heart.obj";		// Heart model file
std::string obj_star		=	models_folder + "star.obj";			// Star model file
std::string obj_crosshair	=	models_folder + "crosshair.obj";	// Crosshair model file
std::string obj_numbers[]	= {										// Objects to form score and timer boards
								models_folder + "0.obj",			
								models_folder + "1.obj",
								models_folder + "2.obj",
								models_folder + "3.obj",
								models_folder + "4.obj",
								models_folder + "5.obj",
								models_folder + "6.obj",
								models_folder + "7.obj",
								models_folder + "8.obj",
								models_folder + "9.obj",
								models_folder + "separator.obj"
							  };

// Path audios
std::string sounds_folder			=	"resource/sound/sfx/";
std::string sfx_laser				=	sounds_folder + "game/spaceship/laser_blast_01.wav";					// Laser shot SFX
std::string sfx_background			=	sounds_folder + "game/environment/target_balls_cloud_fat_loop.wav";		// Background SFX
std::string sfx_asteroid_explosion[] = {
										sounds_folder + "game/asteroid/explosion_generic_large_distant_01.wav",	// Asteroid hit SFX
										sounds_folder + "game/spaceship/explosion_vehicle_big_distant_01.wav"	// Spaceship explosion SFX
									  };
std::string sfx_spaceship[]			= {
										sounds_folder + "game/spaceship/gas_stove_fire_loop.wav",				// Spaceship rockets SFX
										sounds_folder + "game/spaceship/explosion_vehicle_big_distant_01.wav"	// Spaceship explosion SFX
									  };
// ***********************************************************************************************************************
//	FUNCTIONS UTILS
// ***********************************************************************************************************************

void initEntity(Entity * entity, size_t num_entities) {
	// Initiate entity fields
	entity->num_entities	= num_entities;
	entity->status			= new Status[num_entities];
	entity->object			= NULL;

	// Initiate all fields of all status 
	for (size_t i = 0; i < num_entities; i++)
		resetStatus(&entity->status[i]);
}

// -----------------------------------------------------------------------------------------------------------------------

void resetStatus(Status * status) {
	status->alive			= false;
	status->hardness		= 0.0f;
	status->health			= 0.0f;
	status->inclination.x	= 0.0f;
	status->inclination.y	= 0.0f;
	status->inclination.z	= 0.0f;
	status->position.x		= 0.0f;
	status->position.y		= 0.0f;
	status->position.z		= 0.0f;
	status->size			= 1.0f;
	status->speed			= 0.0f;
	status->rotate_animation.x = 0.0f;
	status->rotate_animation.y = 0.0f;
	status->rotate_animation.z = 0.0f;
}

// -----------------------------------------------------------------------------------------------------------------------

void draw(Entity entity, unsigned int index, unsigned int model_index) {
	glPushMatrix(); {
		// Draw the bounding box, if the mode is activated
		if (show_bounding_boxes && entity.object->model(model_index)->hasBoundingBox())
			renderBoundingBox(
				translateBoundingBox(
					entity.object->model(model_index)->getBoundingBox(),
					entity.status[index].position.x,
					entity.status[index].position.y,
					entity.status[index].position.z
				)
			);

		// Apply translation, if necessary
		if (entity.status[index].position.x != 0.0f || entity.status[index].position.y != 0.0f || entity.status[index].position.z != 0.0f)
			glTranslatef(entity.status[index].position.x, entity.status[index].position.y, entity.status[index].position.z);

		// Apply rotation on X, if necessary
		if(entity.status[index].inclination.x + entity.status[index].rotate_animation.x != 0.0f)
			glRotatef(entity.status[index].inclination.x + entity.status[index].rotate_animation.x, 1.0f, 0.0f, 0.0f);

		// Apply rotation on Y, if necessary
		if (entity.status[index].inclination.y + entity.status[index].rotate_animation.y != 0.0f)
			glRotatef(entity.status[index].inclination.y + entity.status[index].rotate_animation.y, 0.0f, 1.0f, 0.0f);

		// Apply rotation on Z, if necessary
		if (entity.status[index].inclination.z + entity.status[index].rotate_animation.z != 0.0f)
			glRotatef(entity.status[index].inclination.z + entity.status[index].rotate_animation.z, 0.0f, 0.0f, 1.0f);

		// Draw the model
		entity.object->model(model_index)->render();
	} glPopMatrix();
}

// -----------------------------------------------------------------------------------------------------------------------

bool checkCollision(BoundingBox obj1, BoundingBox obj2) {
	if ((obj1.higher.x - obj1.less.x) < (obj2.higher.x - obj2.less.x)) {
		double xl = obj2.less.x, xh = obj2.higher.x;
		obj2.higher.x = obj1.higher.x;
		obj2.less.x = obj1.less.x;
		obj1.higher.x = xh;
		obj1.less.x = xl;
	}

	if ((obj1.higher.y - obj1.less.y) < (obj2.higher.y - obj2.less.y)) {
		double yl = obj2.less.y, yh = obj2.higher.y;
		obj2.higher.y = obj1.higher.y;
		obj2.less.y = obj1.less.y;
		obj1.higher.y = yh;
		obj1.less.y = yl;
	}

	if (obj1.higher.z >= obj2.less.z)
		if ((obj1.higher.y >= obj2.higher.y && obj1.less.y <= obj2.higher.y) || (obj1.higher.y >= obj2.less.y && obj1.less.y <= obj2.less.y))
			if ((obj1.higher.x >= obj2.higher.x && obj1.less.x <= obj2.higher.x) || (obj1.higher.x >= obj2.less.x && obj1.less.x <= obj2.less.x))
				return true;

	return false;
}

// ***********************************************************************************************************************
//	GENERATORS
// ***********************************************************************************************************************

void shootLaser() {
	// Check for any cannon available
	if (left_cannon_charging && right_cannon_charging)
		return;

	// Sets the position on X of the new laser
	lasers.status[laser_list_back].position.x = !cannon_selector ?
		(spaceship.status[0].position.x - 3.9f) * cos(spaceship.status[0].inclination.z) - (spaceship.status[0].position.y * sin(spaceship.status[0].inclination.z)) :
		(spaceship.status[0].position.x + 3.9f) * cos(spaceship.status[0].inclination.z) - (spaceship.status[0].position.y * sin(spaceship.status[0].inclination.z));
		
	// Sets the position on Y of the new laser
	lasers.status[laser_list_back].position.y = !cannon_selector ?
		(spaceship.status[0].position.x - 3.9f) * sin(spaceship.status[0].inclination.z) + (spaceship.status[0].position.y * cos(spaceship.status[0].inclination.z)) :
		(spaceship.status[0].position.x + 3.9f) * sin(spaceship.status[0].inclination.z) + (spaceship.status[0].position.y * cos(spaceship.status[0].inclination.z));

	// Sets the position on Z of the new laser
	lasers.status[laser_list_back].position.z = spaceship.status[0].position.z - 3.0f;

	// Give life to the laser
	lasers.status[laser_list_back].alive = true;

	// Update back of list
	laser_list_back++;
	laser_list_back = laser_list_back < lasers.num_entities ? laser_list_back : 0;

	// Disable the left cannon while recharging.
	if (!left_cannon_charging && !cannon_selector) {
		// Passing the baton
		cannon_selector = true;

		// Disabling the cannon
		left_cannon_charging = true;

		// Recharging
		std::thread left_cannon(
			[=]() {
				sleep(300); 
				left_cannon_charging = false;
			}
		);

		left_cannon.detach();
	}
	// Disable the right cannon while recharging.
	else if (!right_cannon_charging && cannon_selector) {
		// Passing the baton
		cannon_selector = false;

		// Disabling the cannon
		right_cannon_charging = true;

		// Recharging
		std::thread right_cannon(
			[=]() {
				sleep(300); 
				right_cannon_charging = false;
			}
		);

		right_cannon.detach();
	}

	// Play shot sound
	lasers.object->audio(0)->play();
}

// -----------------------------------------------------------------------------------------------------------------------

void createAsteroid() {
	// Sets random position
	asteroids.status[astrd_list_back].position.x = random(0.0f, SCENE_LIMITE_RIGHT * 2) + SCENE_LIMITE_LEFT;	// Position range [-150, 150]
	asteroids.status[astrd_list_back].position.y = random(0.0f, SCENE_LIMITE_UP * 2) + SCENE_LIMITE_DOWN;		// Position range [-150, 150]
	asteroids.status[astrd_list_back].position.z = 0.0f;

	// Sets rotation for animation
	double astrd_rotate = random(0.0f, 360.0f);

	asteroids.status[astrd_list_back].inclination.x = astrd_rotate;
	asteroids.status[astrd_list_back].inclination.y = astrd_rotate;
	asteroids.status[astrd_list_back].inclination.z = astrd_rotate;

	// Size range [1, 3] - Division by 2 to generate values 1.0, 1.5, 2.0, 2.5, 3.0
	asteroids.status[astrd_list_back].size = random(1.0f, 6.0f) / 2.0f;

	// Sets the resistence by the size
	asteroids.status[astrd_list_back].hardness = ASTEROID_BASE_HARDNESS * asteroids.status[astrd_list_back].size;

	// Sets the HP by the size
	asteroids.status[astrd_list_back].health = ASTEROID_BASE_HEALTH * asteroids.status[astrd_list_back].size;

	// Sets the speed by the current difficulty level
	asteroids.status[astrd_list_back].speed = ASTEROID_BASE_SPEED * current_difficulty;

	// Give life to the asteroid
	asteroids.status[astrd_list_back].alive = true;

	// Update back of list
	astrd_list_back++;
	astrd_list_back = astrd_list_back < asteroids.num_entities ? astrd_list_back : 0;
}

// ***********************************************************************************************************************
//	MANAGERS
// ***********************************************************************************************************************

void laserManager() {
	unsigned int i = laser_list_front;

	while (i != laser_list_back) {
		// Check if this laser is alive and render it
		if (lasers.status[i].alive)
			draw(lasers, i, 0);

		// Update the next position on Z
		lasers.status[i].position.z -= LASER_BASE_SPEED;

		// Check if the laser reaches the limit of the scene
		if (lasers.status[i].position.z <= 0.0f) {
			lasers.status[i].alive = false;
			laser_list_front += 1;
			laser_list_front = laser_list_front < lasers.num_entities ? laser_list_front : 0;
		}

		// Next laser
		i++;
		i = i < lasers.num_entities ? i : 0;
	}
}

// -----------------------------------------------------------------------------------------------------------------------

void asteroidManager() {
	unsigned int a = astrd_list_front;

	while (a != astrd_list_back) {
		// Check if the asteroid is alive
		if (asteroids.status[a].alive) {
			// Check if any laser collided with this asteroid
			{
				unsigned int l = laser_list_front;

				// Scale
				asteroids.object->model(0)->scale(asteroids.status[a].size);

				while (l != laser_list_back) {
					// Check if the laser is alive
					if (lasers.status[l].alive) {
						// Check for collision
						if (
							checkCollision(
								// Translate the asteroid bounding box to the current position
								translateBoundingBox(
									asteroids.object->model(0)->getBoundingBox(),
									asteroids.status[a].position.x,
									asteroids.status[a].position.y,
									asteroids.status[a].position.z
								),
								// Translate the laser bounding box to the current position
								translateBoundingBox(
									lasers.object->model(0)->getBoundingBox(),
									lasers.status[l].position.x,
									lasers.status[l].position.y,
									lasers.status[l].position.z
								)
							)
							)
						{
							// Subtract HP by damage -> HP = HP - (damage - hardness)
							asteroids.status[a].health -= (LASER_DAMAGE - asteroids.status[a].hardness);

							// Kill the asteroid if your life points are gone.
							if (asteroids.status[a].health <= 0.0f) {
								asteroids.status[a].alive = false;
								asteroids.object->audio(1)->play();
							}

							// Kill the laser
							lasers.status[l].alive = false;

							// Counting points
							if(asteroids.status[a].alive)
								current_score += SCORE_BASE_POINTS * current_difficulty;
							else
								current_score += SCORE_BASE_POINTS * asteroids.status[a].hardness * current_difficulty;

							// Play explosion sound
							asteroids.object->audio(0)->play();

							// Explosion animation


							// If collided, get out of the collision check
							break;
						}
					}

					// Next laser
					l++;
					l = l < lasers.num_entities ? l : 0;
				}
			}
			// Check if this asteroid collided with the spaceship
			{
				if (
					checkCollision(
						// Translate the asteroid bounding box to the current position
						translateBoundingBox(
							asteroids.object->model(0)->getBoundingBox(),
							asteroids.status[a].position.x,
							asteroids.status[a].position.y,
							asteroids.status[a].position.z
						),
						// Translate the spaceship bounding box to the current position
						translateBoundingBox(
							spaceship.object->model(0)->getBoundingBox(),
							spaceship.status[0].position.x,
							spaceship.status[0].position.y,
							spaceship.status[0].position.z
						)
					) && spaceship.status[0].alive
					) {
					// Kill the Spaceship
					spaceship.status[0].alive = false;

					// Play explosion sound
					spaceship.object->audio(1)->play();

					// Pause Spaceship audio
					spaceship.object->audio(0)->pause();

					// Decress HP level
					health_count--;

					// Respawn animation
					if (health_count > 0) {
						// Reset animation
						spaceship.status->rotate_animation.x = 0.0f;
						spaceship.status->rotate_animation.y = 0.0f;
						spaceship.status->rotate_animation.z = 0.0f;

						// Respawn
						spaceshipRespawn();
					}
					
					// Game over
					else {
						// GAME OVER EVENT NOT IMPLEMENTED YET
					}

					// Explosion animation

				}
			}

			// Draw asteroid
			if (asteroids.status[a].alive)
				draw(asteroids, a, 0);

			// Update gravity animation
			double animation = asteroids.status[a].inclination.x < 360.0f ? asteroids.status[a].inclination.x + 0.5f : 0.0f;

			asteroids.status[a].inclination.x = animation;
			asteroids.status[a].inclination.y = animation;
			asteroids.status[a].inclination.z = animation;
		}

		// Update position
		asteroids.status[a].position.z += asteroids.status[a].speed;

		// Check if the asteroid reaches the limit of the scene
		if (asteroids.status[a].position.z >= SCENE_LIMITE_BACK - 10.0f) {
			astrd_list_front++;
			astrd_list_front = astrd_list_front < asteroids.num_entities ? astrd_list_front : 0;
		}

		// Next asteroid
		a++;
		a = a < asteroids.num_entities ? a : 0;
	}
}

// -----------------------------------------------------------------------------------------------------------------------

void scoreManager() {
	int aux = current_score;

	// Split score into digits and use it as an index of the list of numbers.
	for (int i = score_board.num_entities - 1; i >= 0; i--) { // Countdown because the numbers grow from right to left
		draw(score_board, i, aux % 10);
		aux /= 10;
	}
}

// -----------------------------------------------------------------------------------------------------------------------

void timerManager() {
	for (unsigned int i = 0, l = 0; i < 8 && l < 6; i++, l++) {
		// Draw separator
		if (i == 2 || i == 5) {
			draw(timer_board, i, 10);
			i++;
		}

		// Draw digits
		draw(timer_board, i, timevalue[l]);
	}
}

// -----------------------------------------------------------------------------------------------------------------------

void healthManager() {
	// Verify that the game ended.
	if (health_count == 0)
		gameOver();

	// Check if the points are enough for a new life.
	else if (current_score >= needed_to_next_health) {
		health_count += health_count < 5 ? 1 : 0;
		needed_to_next_health += POINTS_NEEDED_TO_HEALTH;
	}

	// Draw hearts
	for (unsigned int i = 0; i < health_count; i++) {
		draw(hp_board, i, 0);
	}
}

// -----------------------------------------------------------------------------------------------------------------------

void specialManager() {
	// Draw stars
	for (unsigned int i = 0; i < spcial_count; i++) {
		draw(special_board, i, 0);
	}
}

// -----------------------------------------------------------------------------------------------------------------------

void cameraManager() {
	// Update position
	camera.position.x = spaceship.status->position.x;
	camera.position.y = spaceship.status->position.y + 10.0f;
	camera.position.z = spaceship.status->position.z + 40.0f;

	// Update look at
	camera.lookat.x = spaceship.status->position.x;
	camera.lookat.y = spaceship.status->position.y;
	camera.lookat.z = 0.0f;

	// Camera setup
	gluLookAt(
		camera.position.x,	camera.position.y,	camera.position.z,		// Camera position
		camera.lookat.x,	camera.lookat.y,	camera.lookat.z,		// Camera look at
		camera.up.x,		camera.up.y,		camera.up.z				// Camera vector up
	);
}

// ***********************************************************************************************************************
//	INTERACTION CALLBACKS
// ***********************************************************************************************************************

void spaceshipMoveUp() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	spaceship.status->rotate_animation.x = SPACESHIP_ANIMATION;		// Apply animation rotate around X
	spaceship.status->position.y += spaceship.status->position.y < SCENE_LIMITE_UP ? spaceship.status->speed : 0.0f;	// Move spaceship on the positive Y axis
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipMoveDown() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	spaceship.status->rotate_animation.x = 360.0f - SPACESHIP_ANIMATION;	// Apply animation rotate around X (reverse)
	spaceship.status->position.y -= spaceship.status->position.y > SCENE_LIMITE_DOWN ? spaceship.status->speed : 0.0f;	// Move spaceship on the negative Y axis
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipMoveLeft() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//spaceship.status->rotate_animation.y = SPACESHIP_ANIMATION;		// Apply animation rotate around Y
	spaceship.status->rotate_animation.z = SPACESHIP_ANIMATION;		// Apply animation rotate around Y
	spaceship.status->position.x -= spaceship.status->position.x > SCENE_LIMITE_LEFT ? spaceship.status->speed : 0.0f;	// Move spaceship on the negative X axis
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipMoveRight() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//spaceship.status->rotate_animation.y = 360.0f - SPACESHIP_ANIMATION;		// Apply animation rotate around Y (reverse)
	spaceship.status->rotate_animation.z = 360.0f - SPACESHIP_ANIMATION;		// Apply animation rotate around Y (reverse)
	spaceship.status->position.x += spaceship.status->position.x < SCENE_LIMITE_RIGHT ? spaceship.status->speed : 0.0f;	// Move spaceship on the positive X axis
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipTurnLeft() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//glm::mat4 matrix(1);

	//spaceship.status->rotate_animation.z = SPACESHIP_ANIMATION + 1.0f;		// Apply animation rotate around Z
	//spaceship.status->inclination.z += spaceship.status->inclination.z < 360.0f ? 0.5f : spaceship.status->inclination.z - 360.0f + 0.5f;	// Rotate around Z

	// Update vector up
	//camera.inclination.z += 1.0f;
	//matrix = glm::rotate(matrix, (float)camera.inclination.z, glm::vec3(0.0, 0.0, 1.0));
	//camera.up = glm::vec3(matrix * glm::vec4(camera.up, 1.0f));
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipTurnRight() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//glm::mat4 matrix(1);

	//spaceship.status->rotate_animation.z = 360.0f - SPACESHIP_ANIMATION + 1.0f;		// Apply animation rotate around Z (reverse)
	//spaceship.status->inclination.z -= spaceship.status->inclination.z > 0.0f ? 0.5f : 360.0f + spaceship.status->inclination.z - 0.5f;	// Rotate around Z
	
	//camera.inclination.z -= 1.0f;// Update vector up
	//matrix = glm::rotate(matrix, (float)camera.inclination.z, glm::vec3(0.0, 0.0, 1.0));
	//camera.up = glm::vec3(matrix * glm::vec4(camera.up, 1.0f));
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipReleaseMoveUp() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	spaceship.status->rotate_animation.x = 0.0f;		// Apply animation rotate around X
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipReleaseMoveDown() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	spaceship.status->rotate_animation.x = 0.0f;	// Apply animation rotate around X (reverse)
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipReleaseMoveLeft() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//spaceship.status->rotate_animation.y = 0.0f;		// Apply animation rotate around Y
	spaceship.status->rotate_animation.z = 0.0f;		// Apply animation rotate around Y
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipReleaseMoveRight() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//spaceship.status->rotate_animation.y = 0.0f;		// Apply animation rotate around Y (reverse)
	spaceship.status->rotate_animation.z = 0.0f;		// Apply animation rotate around Y (reverse)
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipReleaseTurnLeft() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//glm::mat4 matrix(1);

	//spaceship.status->rotate_animation.z = SPACESHIP_ANIMATION + 1.0f;		// Apply animation rotate around Z
	//spaceship.status->inclination.z += spaceship.status->inclination.z < 360.0f ? 0.5f : spaceship.status->inclination.z - 360.0f + 0.5f;	// Rotate around Z

	// Update vector up
	//camera.inclination.z += 1.0f;
	//matrix = glm::rotate(matrix, (float)camera.inclination.z, glm::vec3(0.0, 0.0, 1.0));
	//camera.up = glm::vec3(matrix * glm::vec4(camera.up, 1.0f));
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipReleaseTurnRight() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	//glm::mat4 matrix(1);

	//spaceship.status->rotate_animation.z = 360.0f - SPACESHIP_ANIMATION + 1.0f;		// Apply animation rotate around Z (reverse)
	//spaceship.status->inclination.z -= spaceship.status->inclination.z > 0.0f ? 0.5f : 360.0f + spaceship.status->inclination.z - 0.5f;	// Rotate around Z

	//camera.inclination.z -= 1.0f;// Update vector up
	//matrix = glm::rotate(matrix, (float)camera.inclination.z, glm::vec3(0.0, 0.0, 1.0));
	//camera.up = glm::vec3(matrix * glm::vec4(camera.up, 1.0f));
}

// -----------------------------------------------------------------------------------------------------------------------

void spaceshipShootLasers() {
	// If the ship is not alive, disable the interaction events.
	if (!spaceship.status->alive)
		return;

	// Shoot laser
	shootLaser();
}

// -----------------------------------------------------------------------------------------------------------------------

void pauseGame() {
	// NOT IMPLEMENTED YET
}

// ***********************************************************************************************************************
//	GAME
// ***********************************************************************************************************************

bool loadGame() {
	double pos;
	double gui_z_pos = SCENE_LIMITE_BACK - 20;

	// Init Audio
	Audio::init();

	// --------------------------------------------------------------
	// INIT ENTITIES
	// --------------------------------------------------------------
	
	// Func		Entity			  Num
	initEntity(	&asteroids,		MAX_AF);	// Init asteroids entities
	initEntity(	&background,		1 );	// Init background entity
	initEntity(	&crosshair,			1 );	// Init crosshair entity
	initEntity(	&hp_board,		MAX_HP);	// Init hp_board entity
	initEntity(	&lasers,		MAX_LF);	// Init lasers entities
	initEntity(	&score_board,	MAX_NL);	// Init score_board entity
	initEntity(	&spaceship,			1 );	// Init spaceship entity
	initEntity(	&special_board, MAX_SP);	// Init special_board entity
	initEntity(	&timer_board,		8 );	// Init timer_board entity

	// Instantiating 3D objects
	asteroids.object	 = new Object3D();	// Asteroid
	background.object	 = new Object3D();	// Background
	crosshair.object	 = new Object3D();	// Crosshair
	hp_board.object		 = new Object3D();	// Healthboard
	lasers.object		 = new Object3D();	// Laser
	score_board.object	 = new Object3D();	// Scoreboard
	spaceship.object	 = new Object3D();	// Spaceship
	special_board.object = new Object3D();	// Specialboard
	timer_board.object	 = NULL;			// Timerboard

	// --------------------------------------------------------------
	// LOAD MODELS AND AUDIOS
	// --------------------------------------------------------------

	//  Object					Method		Model_Path		Num		Audio_Path				Num		ErrHandler		Msg				  Return
	if (!lasers.object		->	load	(	&obj_laser,		 1,		&sfx_laser,				 1	))	PRINT_ERR("Load laser model",	  false);	// Loads the laser model
	if (!hp_board.object	->	load	(	&obj_heart,		 1,		NULL,					 0	))	PRINT_ERR("Load heart model",	  false);	// Loads the heart model
	if (!crosshair.object	->	load	(	&obj_crosshair,  1,		NULL,					 0	))	PRINT_ERR("Load crosshair model", false);	// Loads the crosshair model
	if (!spaceship.object	->	load	(	&obj_spaceship,  1,		sfx_spaceship,			 2	))	PRINT_ERR("Load spaceship model", false);	// Loads the spaceship model
	if (!asteroids.object	->	load	(	&obj_asteroid,	 1,		sfx_asteroid_explosion,  2	))	PRINT_ERR("Load asteroid model",  false);	// Loads the asteroids model
	if (!background.object	->	load	(	&obj_background, 1,		&sfx_background,		 1	))	PRINT_ERR("Load bg model",		  false);	// Loads the background model
	if (!special_board.object->	load	(	&obj_star,		 1,		NULL,					 0	))	PRINT_ERR("Load star model",	  false);	// Loads the star model
	if (!score_board.object	->	load	(	obj_numbers,	11,		NULL,					 0	))	PRINT_ERR("Load numbers model",	  false);	// Loads the number model

	timer_board.object = score_board.object;		// Sets the pointer to same numbers models

	// --------------------------------------------------------------
	// CONFIGURING OBJECTS
	// --------------------------------------------------------------

	// Configuring background
	background.object->model(0)->scale(1.8f);		// Set scale factor

	// Configuring Spaceship
	spaceship.status->alive			= true;							// Set alive
	spaceship.status->speed			= SPACESHIP_SPEED;				// Set speed
	spaceship.status->inclination.x = 12.0f;						// Set visual inclination
	spaceship.status->position.z	= SCENE_LIMITE_BACK - 40.0f;	// Set Z position
	spaceship.object->model(0)->scale(0.65f);						// Set the model scale factor
	spaceship.object->model(0)->genBoundingBox(BoundingBoxRange::FULL);		// Generates the Bounding box

	// Configuring laser
	lasers.object->model(0)->scale(1.3f);									// Set scale factor
	lasers.object->model(0)->genBoundingBox(BoundingBoxRange::FULL);		// Generates the Bounding box

	// Configuring asteroid
	asteroids.object->model(0)->genBoundingBox(BoundingBoxRange::CUBE);		// Generates the Bounding box
//	asteroids.object->audio(0)->setVolume(50);

	// Configuring crosshair
	crosshair.object->model(0)->scale(0.55f);		// Set scale factor

	// Configuring healthboard
	hp_board.object->model(0)->scale(0.21f);		// Set scale factor

	// Configuring specialsboard
	special_board.object->model(0)->scale(0.25f);	// Set scale factor

	// Configuring camera
	camera.position.x	= spaceship.status->position.x;
	camera.position.y	= spaceship.status->position.y + 10.0f;
	camera.position.z	= spaceship.status->position.z + 40.0f;
	camera.lookat.x		= spaceship.status->position.x;
	camera.lookat.y		= spaceship.status->position.y;
	camera.lookat.z		= 0.0f;
	camera.up.x			= 0.0f;
	camera.up.y			= 1.0f;
	camera.up.z			= 0.0f;

	// --------------------------------------------------------------
	// POSITIONING GUI
	// --------------------------------------------------------------

	// Positioning the crosshair
	crosshair.status->position.y = 9.6f;
	crosshair.status->position.z = gui_z_pos;

	double aux;
	unsigned int i;

	// Positioning the healthboard
	for (i = 0, aux = -13.4f; i < hp_board.num_entities; i++) {
		hp_board.status[i].position.x = aux;
		hp_board.status[i].position.y = 16.2f;
		hp_board.status[i].position.z = gui_z_pos;

		aux += 0.9f;
	}

	// Positioning the specialsboard
	for (i = 0, aux = 13.3f; i < special_board.num_entities; i++) {
		special_board.status[i].position.x = aux;
		special_board.status[i].position.y = 16.1f;
		special_board.status[i].position.z = gui_z_pos;
		
		aux -= 0.9f;
	}

	//  Positioning the scoreboard
	for (i = 0, aux = -13.7f; i < score_board.num_entities; i++) {
		score_board.status[i].position.x = aux;
		score_board.status[i].position.y = 16.9f;
		score_board.status[i].position.z = gui_z_pos;

		aux += 0.7f;
	}

	//  Positioning the timerboard
	for (i = 0, aux = 13.2f; i < timer_board.num_entities; i++) {
		// Add separator
		if (i == 2 || i == 5) {
			aux += 0.4f;
			timer_board.status[i].position.x = aux;
			timer_board.status[i].position.y = 16.9f;
			timer_board.status[i].position.z = gui_z_pos;
			aux -= 0.7f;
			i++;
		}

		// New digite
		timer_board.status[i].position.x = aux;
		timer_board.status[i].position.y = 16.9f;
		timer_board.status[i].position.z = gui_z_pos;
		aux -= 0.7f;
	}

	// --------------------------------------------------------------
	// CONTROL THREADS
	// --------------------------------------------------------------

	// Periodically creates a new asteroid
	std::thread asteroidLauncher(
		[=]() {
			// Start session delay
			sleep(5000);

			while (true) {
				sleep(1500 / current_difficulty);	// milliseconds / difficulty
				createAsteroid();
			}
		}
	);

	// Time counter for timerboard
	std::thread timeCounter(
		[=]() {
			for (timevalue[5] = 0; timevalue[5] < 6; timevalue[5]++)	// Minutes
				for (timevalue[4] = 0; timevalue[4] < 10; timevalue[4]++)	// Minutes
					for (timevalue[3] = 0; timevalue[3] < 6; timevalue[3]++)	// Secounds
						for (timevalue[2] = 0; timevalue[2] < 10; timevalue[2]++)	// Secounds
							for (timevalue[1] = 0; timevalue[1] < 10; timevalue[1]++)	// Milissecounds
								do {	// While the spaceship will not revive, time stands still
									sleep(100);
								} while (!spaceship.status->alive);
		}
	);

	// --------------------------------------------------------------
	// PLAY LOOPS AUDIOS
	// --------------------------------------------------------------

	spaceship.object->audio(0)->playLoop();		// Start spaceship rockets sound
	background.object->audio(0)->playLoop();	// Start space sound

	// --------------------------------------------------------------
	// SET AUDIO VOLUME
	// --------------------------------------------------------------

	spaceship.object->audio(0)->setVolume(75);		// Set sound volume
	background.object->audio(0)->setVolume(20);		// Set sound volume

	// --------------------------------------------------------------
	// START BACKGROUND THREADS
	// --------------------------------------------------------------

	timeCounter.detach();		// Start timer
	asteroidLauncher.detach();	// Start asteroids generator

	return true;
}

// -----------------------------------------------------------------------------------------------------------------------

void runGame() {
	cameraManager();

	glPushMatrix(); {
		glTranslatef(spaceship.status->position.x, spaceship.status->position.y, 0.0f);
		draw(crosshair, 0, 0);	// Draw Crosshair
		timerManager();			// Draw Timer
		scoreManager();			// Draw Score
		healthManager();		// Draw HP
		specialManager();		// Draw Specials
	} glPopMatrix();

	if(spaceship.status[0].alive || respawnDraw)
		draw(spaceship, 0, 0);		// Draw Spaceship

	laserManager();			// Draw Lasers
	asteroidManager();		// Draw Asteroids

	draw(background, 0, 0);	// Draw Background
}

// -----------------------------------------------------------------------------------------------------------------------

void gameOver() {
	// NOT IMPLEMENTED YET
}

void spaceshipRespawn() {
	std::thread respawnCountdown(
		[=]() {
			int i = 0;

			sleep(1500);

			while (i++ < 15) {
				respawnDraw = true;
				sleep(respawnTime);
				respawnDraw = false;
				sleep(respawnTime);
			}

			spaceship.status[0].alive = true;
			spaceship.object->audio(0)->resume();
			respawnDraw = false;
		}
	);

	respawnCountdown.detach();
}
