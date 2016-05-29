#include "Spaceship.h"

Spaceship::Spaceship() {
	this->animation_angle_x		= 0.0f;
	this->animation_angle_y		= 0.0f;
	this->animation_angle_z		= 0.0f;
	this->animation_angle_inc	= 0.0f;
	this->speed					= 0.0f;
}

//***********************************************************************************************************************

Spaceship::~Spaceship() {
	// NO IMPLEMENTATION NEEDED.
}

//***********************************************************************************************************************

void Spaceship::setAnimationRotateInc(double angle) {
	this->animation_angle_inc = angle;
}

//***********************************************************************************************************************

bool Spaceship::start() {
	return this->audios[0].playLoop();
}

//***********************************************************************************************************************

void Spaceship::render() {
	glPushMatrix(); {
		// Move to selected position
		glTranslatef(this->pos.x, this->pos.y, this->pos.z);

		// Rotate and Animations
		glRotatef(this->angle.x + this->animation_angle_x, 1.0f, 0.0f, 0.0f);
		glRotated(this->angle.y + this->animation_angle_y, 0.0f, 1.0f, 0.0f);
		glRotated(this->angle.z + this->animation_angle_z, 0.0f, 0.0f, 1.0f);

		// Render model
		this->models[0].render();
	} glPopMatrix();
}

//***********************************************************************************************************************

void Spaceship::event(int id) {
	// Commands interation events
	if (id == 0) this->animation_angle_x = 360.0f - this->animation_angle_inc;				// Turn Down
	else if (id == 1) this->animation_angle_x = this->animation_angle_inc;					// Turn Up
	else if (id == 2) this->animation_angle_z = this->animation_angle_inc + 1.0f;			// Spin Left
	else if (id == 3) this->animation_angle_z = 360.0f - this->animation_angle_inc + 1.0f;	// Spit Right
	else if (id == 4) this->animation_angle_y = this->animation_angle_inc;					// Turn Right
	else if (id == 5) this->animation_angle_y = 360.0f - this->animation_angle_inc;			// Turn Left
	else if (id == 6) {																		// Reset
		this->animation_angle_x = 0.0f;
		this->animation_angle_y = 0.0f;
		this->animation_angle_z = 0.0f;
	}
}
