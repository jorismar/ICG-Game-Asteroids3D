#ifndef JB_OBJECT3D_H
#define JB_OBJECT3D_H
#ifdef WIN32
	#include <windows.h>
#endif

#include <string>
#include <GL/gl.h>

#include "Model3D.h"
#include "Audio.h"
#include "utils.h"

class Object3D {
protected:
	Model3D *	 models;		// Model object.
	Audio   *	 audios;		// Audio object.
	unsigned int num_models;	// Number of 3D models that object
	unsigned int num_audios;	// Number of audios that object

public:
	/**
	* \brief Constructor
	*/
	Object3D();

	/**
	* \brief Destructor
	*/
	virtual ~Object3D();

	/**
	* \brief Get the model instancied object.
	*
	* \return	Returns a pointer to the model object.
	*/
	Model3D * model(unsigned int index);

	/**
	* \brief Get the audio instancied object.
	*
	* \return	Returns a pointer to the audio object.
	*/
	Audio * audio(unsigned int index);

	/**
	* \brief Load model and audio files.
	*
	* \param	model		Model path.
	* \param	audio_list	List of audio files.
	* \param	num_audios	Size of the audio list.
	*
	* \return	Returns true if successful and false if not.
	*/
	bool load(std::string * models_list, unsigned int num_models, std::string* audio_list, unsigned int num_audios);
};

#endif // !JB_OBJECT3D_H
