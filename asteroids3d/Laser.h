#ifndef JB_LASER_H
#define JB_LASER_H

#include "Object.h"

class Laser : public Object {
private:
	bool	alive;
	double	speed;			// Moviment speed

public:
	/**
	* \brief Constructor
	*/
	Laser();

	/**
	* \brief Destructor
	*/
	virtual ~Laser();

	/**
	* \brief Sets the moviment speed of laser.
	*
	* \param	speed	Moviment speed in positions.
	*/
	void setSpeed(double speed);

	/**
	* \brief Gets the moviment speed of laser.
	*
	* \return	Returns the moviment speed in positions.
	*/
	double getSpeed();

	/**
	* \brief Renders the model according to their own rules.
	*
	* \param	pos		Position of object at this moment.
	* \param	angle	Angle of object inclination at this moment.
	*/
	void dynamicRender(Position pos, Angle angle);

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
 
	* \param	id		Event id.
	*/
	virtual void event(int id);

	void kill();
	bool isAlive();
};

#endif // !JB_LASER_H
