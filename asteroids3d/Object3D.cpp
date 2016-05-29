#include "Object.h"

Object::Object() {
	this->models	= NULL;
	this->audios	= NULL;
	this->pos.x		= 0.0f;
	this->pos.y		= 0.0f;
	this->pos.z		= 0.0f;
	this->angle.x	= 0.0f;
	this->angle.y	= 0.0f;
	this->angle.z	= 0.0f;
	this->factor.x	= 1.0f;		// Scale not apllied (Normal size)
	this->factor.y	= 1.0f;		// Scale not apllied (Normal size)
	this->factor.z	= 1.0f;		// Scale not apllied (Normal size)
}

//***********************************************************************************************************************

Object::~Object() {
	delete[] this->models;
	delete[] this->audios;
}

//***********************************************************************************************************************

void Object::setPosition(double x, double y, double z) {
	this->pos.x = x;
	this->pos.y = y;
	this->pos.z = z;
}

//***********************************************************************************************************************

void Object::setRotation(double angle, bool x, bool y, bool z) {
	this->angle.x = x ? angle : this->angle.x;
	this->angle.y = y ? angle : this->angle.y;
	this->angle.z = z ? angle : this->angle.z;
}

//***********************************************************************************************************************

void Object::setScale(double factor) {
	this->factor.x = factor;
	this->factor.y = factor;
	this->factor.z = factor;

	for(unsigned int i = 0; i < this->num_models; i++)
		this->models[i].scale(this->factor.x, this->factor.y, this->factor.z);
}

//***********************************************************************************************************************

void Object::setScale(double x, double y, double z) {
	this->factor.x = x;
	this->factor.y = y;
	this->factor.z = z;

	for (unsigned int i = 0; i < this->num_models; i++)
		this->models[i].scale(this->factor.x, this->factor.y, this->factor.z);
}

//***********************************************************************************************************************

Model3D * Object::getModel(unsigned int index) {
	return &this->models[index];
}

//***********************************************************************************************************************

Audio * Object::getAudio(unsigned int index) {
	return &this->audios[index];
}

//***********************************************************************************************************************

Position Object::getPosition() {
	return this->pos;
}

//***********************************************************************************************************************

Angle Object::getRotateAngle() {
	return this->angle;
}

//***********************************************************************************************************************

Factor Object::getScaleFactor() {
	return this->factor;
}

//***********************************************************************************************************************

bool Object::load(std::string * models_list, unsigned int num_models, std::string * audiopath, unsigned int num_audios) {
	this->num_audios = num_audios;
	this->num_models = num_models;
	
	// Load Model
	if (models_list != NULL && num_models > 0) {
		this->models = new Model3D[num_models];

		for (int i = 0; i < num_models; i++) {
			// Load 3D Model
			if (!this->models[i].importFrmFile(models_list[i]))
				return false;

			// Load Material and Texture
			if (!this->models[i].loadTexture())
				return false;
		}
	}

	// Load Sound
	if (audiopath != NULL && num_audios > 0) {
		this->audios = new Audio[num_audios];

		for (int i = 0; i < num_audios; i++) {
			if (!this->audios[i].readFile(audiopath[i]))
				return false;
		}
	}

	return true;
}

//***********************************************************************************************************************

BoundingBox Object::getBoundingBox(unsigned int index) {
	return this->models[index].getBoundingBox();
}
/*
bool Object::start() {
	if(this->num_audios == 1)
		this->audios[0].playLoop();
	else
		for (unsigned int i = 0; i < this->num_audios; i++)
			this->audios[i].play();
}

//***********************************************************************************************************************

void Object::render() {
	for(unsigned int i = 0; i < this->num_models; i++)
		this->models[i].render();
}

//***********************************************************************************************************************

void Object::event(int id) {
	// NO IMPLEMENTATION NEEDED.
}
*/