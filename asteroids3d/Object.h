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

class Object {
protected:
	Model3D*	models;	// Model object.
	Audio*		audios;	// Audio object.
	Position	pos;	// Model position
	Angle		angle;	// Model angle inclination
	Factor		factor;	// Scaling factor

	unsigned int num_models;
	unsigned int num_audios;

public:
	/**
	* \brief Constructor
	*/
	Object();

	/**
	* \brief Destructor
	*/
	virtual ~Object();

	/**
	* \brief Sets the model position on the scene.
	*
	* \param	x	coordinate X.
	* \param	y	coordinate Y.
	* \param	z	coordinate Z.
	*/
	void setPosition(double x, double y, double z);

	/**
	* \brief Apply the rotation transformation in the model.
	*
	* \param	angle	Angle of rotate.
	* \param	x		Flag to apply rotate around of X axis.
	* \param	y		Flag to apply rotate around of Y axis.
	* \param	z		Flag to apply rotate around of Z axis.
	*/
	void setRotation(double angle, bool x, bool y, bool z);

	/**
	* \brief Sets the value of the proportional scale.
	*
	* \param	factor	Scale factor.
	*/
	void setScale(double factor);

	/**
	* \brief Sets the scale on the individual directions.
	*
	* \param	x	Scale value on the X axis.
	* \param	y	Scale value on the Y axis.
	* \param	z	Scale value on the Z axis.
	*/
	void setScale(double x, double y, double z);

	/**
	* \brief Get the model instancied object.
	*
	* \return	Returns a pointer to the model object.
	*/
	Model3D * getModel(unsigned int index);

	/**
	* \brief Get the audio instancied object.
	*
	* \return	Returns a pointer to the audio object.
	*/
	Audio   * getAudio(unsigned int index);

	/**
	* \brief Get the model position.
	*
	* \return	Returns a struct of the model position.
	*/
	Position getPosition();

	/**
	* \brief Get the model inclination.
	*
	* \return	Returns a struct of the model angle inclination.
	*/
	Angle getRotateAngle();

	/**
	* \brief Get the model scaling factor.
	*
	* \return	Returns a struct of the model scaling factor.
	*/
	Factor getScaleFactor();

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

	BoundingBox getBoundingBox(unsigned int index);
	//***************************** CLASS INTERFACE METHODS *****************************

	/**
	* \brief Start the attributes out of the scene.
	* 
	* \return	Returns true if successful and false if not.
	*/
	virtual bool start() = 0;

	/**
	* \brief Renders the model according to their own rules.
	*/
	virtual void render() = 0;

	/**
	* \brief Events handler.
	*
	* \param	id		Event id.
	*/
	virtual void event(int) = 0;
};

#endif // !JB_OBJECT3D_H
