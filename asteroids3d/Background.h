#ifndef JB_BACKGROUND_H
#define JB_BACKGROUND_H

#include "Object.h"

class Background : public Object {
private:
	double	speed;			// Moviment speed

public:
	/**
	* \brief Constructor
	*/
	Background();

	/**
	* \brief Destructor
	*/
	virtual ~Background();

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

#endif // !JB_BACKGROUND_H
