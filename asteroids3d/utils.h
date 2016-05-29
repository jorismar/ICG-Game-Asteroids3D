#ifndef JB_GLUTILS_H
#define JB_GLUTILS_H

typedef struct {
	double x;
	double y;
	double z;
} Position, Angle, Factor;

struct BoundingBox {
	Position higher;
	Position less;
};

#endif // !JB_GLUTILS_H