#include "Object3D.h"

Object3D::Object3D() {
	this->models	 = NULL;
	this->audios	 = NULL;
	this->num_audios = 0;
	this->num_models = 0;
}

//***********************************************************************************************************************

Object3D::~Object3D() {
	delete[] this->models;
	delete[] this->audios;
}

//***********************************************************************************************************************

Model3D * Object3D::model(unsigned int index) {
	return &this->models[index];
}

//***********************************************************************************************************************

Audio * Object3D::audio(unsigned int index) {
	return &this->audios[index];
}

//***********************************************************************************************************************

bool Object3D::load(std::string * models_list, unsigned int num_models, std::string * audiopath, unsigned int num_audios) {
	this->num_audios = num_audios;
	this->num_models = num_models;
	
	// Load Model
	if (models_list != NULL && num_models > 0) {
		this->models = new Model3D[num_models];

		for (int i = 0; i < num_models; i++) {
			// Load 3D Model
			if (!this->models[i].loadModel(models_list[i]))
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
