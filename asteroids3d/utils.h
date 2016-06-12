#ifndef JB_GLUTILS_H
#define JB_GLUTILS_H

#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <thread>
#include <cstdlib>

#define PUSH_TO_ARRAY4(ar, v1, v2, v3, v4) {	\
	ar[0] = v1;									\
	ar[1] = v2;									\
	ar[2] = v3;									\
	ar[3] = v4;									\
}

typedef struct {
	double x;
	double y;
	double z;
} Position, Angle, Factor;

typedef struct BoundingBox {
	Position less;
	Position higher;
};

typedef enum BoundingBoxRange {
	FULL = 0,	// Full bounding
	CUBE = 1	// Center bounding cube
};

typedef struct Status {
	bool		alive;
	double		size;
	double		speed;
	double		health;
	double		hardness;
	Position	position;
	Angle		inclination;
	Angle		rotate_animation;
};

typedef struct KeyboardEvent {
	bool is_pressed;
	void (*pressed)();
	void (*released)();
};

typedef struct MouseEvent {
	int button;
	void *(*pressed)();
};

BoundingBox translateBoundingBox(BoundingBox box, double pos_x, double pos_y, double pos_z);
void renderBoundingBox(BoundingBox box);
void sleep(int time);
void threadDetach(void(*callback)());
void threadJoin(void(*callback)());
double random(double start, double end);

#endif // !JB_GLUTILS_H