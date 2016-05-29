#ifndef JB_SPACESHIP_H
#define JB_SPACESHIP_H

#include "Object.h"

class Spaceship : public Object {
private:
	double	animation_angle_x;		// Animation rotation around the X axis
	double	animation_angle_y;		// Animation rotation around the Y axis
	double	animation_angle_z;		// Animation rotation around the Z axis
	double	animation_angle_inc;	// Increment of the animation rotation

	double	speed;					// Moviment speed (NOT IMPLEMENTED YET)

public:
	/**
	 * \brief Constructor 
	 */
	Spaceship();

	/**
	* \brief Destructor
	*/
	virtual ~Spaceship();

	/**
	* \brief Sets the angle increment to rotate animations.
	*
	* \param	angle	Angle of rotation.
	*/
	void setAnimationRotateInc(double angle);

	/**
	* \brief Start the attributes out of the scene.
	*
	* \return	Returns true if successful and false if not.
	*/
	virtual bool start();

	/**
	* \brief Renders the model according to their own rules.
	*/
	virtual void render();

	/**
	* \brief Events handler.
	* 
	* \param	id		Event id.
	*/
	virtual void event(int id);
};

#endif // !JB_SPACESHIP_H
