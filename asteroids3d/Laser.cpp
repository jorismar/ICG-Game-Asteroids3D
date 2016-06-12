#include "Laser.h"

Laser::Laser() {
	this->speed	= 0.0f;
	this->alive = true;
}

//***********************************************************************************************************************

Laser::~Laser() {
	// NO IMPLEMENTATION NEEDED.
}

//***********************************************************************************************************************

void Laser::setSpeed(double speed) {
	this->speed = speed;
}

//***********************************************************************************************************************

double Laser::getSpeed() {
	return this->speed;
}

//***********************************************************************************************************************

bool Laser::start() {
	this->models[0].scale(this->factor.x, this->factor.y, this->factor.z);
	return true;
}

//***********************************************************************************************************************

void Laser::dynamicRender(Position pos, Angle angle) {
	glPushMatrix(); {
		glRotatef(angle.x + angle.y + angle.z, angle.x, angle.y, angle.z);

		glTranslatef(pos.x, pos.y, pos.z);

		pos.z -= this->speed;

		//this->models[0].renderBoundingBox();
		this->models[0].render();
	} glPopMatrix();
}

//***********************************************************************************************************************

void Laser::render() {
	glPushMatrix(); {
		glRotatef(this->angle.x + this->angle.y + this->angle.z, this->angle.x, this->angle.y, this->angle.z);

		glTranslatef(this->pos.x, this->pos.y, this->pos.z);

		this->pos.z -= this->speed;

		this->models[0].render();
	} glPopMatrix();
}

//***********************************************************************************************************************

void Laser::event(int id) {
	if(id == 0)
		this->audios[0].play();

	// posição da cena
	// rotação da cena
	// posição da nave
	// lado do canhão disparado
}

void Laser::kill() {
	this->alive = false;
}

bool Laser::isAlive() {
	return this->alive;
}