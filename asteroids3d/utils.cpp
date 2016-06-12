#include "utils.h"

//***********************************************************************************************************************

BoundingBox translateBoundingBox(BoundingBox box, double pos_x, double pos_y, double pos_z) {
	box.less.x += pos_x;
	box.less.y += pos_y;
	box.less.z += pos_z;
	box.higher.x += pos_x;
	box.higher.y += pos_y;
	box.higher.z += pos_z;

	return box;
}

//***********************************************************************************************************************

void renderBoundingBox(BoundingBox box) {
	glBegin(GL_LINES);
	glVertex4d(box.higher.x, box.higher.y, box.higher.z, 1.0f);
	glVertex4d(box.higher.x, box.higher.y, box.less.z, 1.0f);

	glVertex4d(box.higher.x, box.higher.y, box.higher.z, 1.0f);
	glVertex4d(box.higher.x, box.less.y, box.higher.z, 1.0f);

	glVertex4d(box.higher.x, box.higher.y, box.higher.z, 1.0f);
	glVertex4d(box.less.x, box.higher.y, box.higher.z, 1.0f);

	glVertex4d(box.higher.x, box.higher.y, box.less.z, 1.0f);
	glVertex4d(box.higher.x, box.less.y, box.less.z, 1.0f);

	glVertex4d(box.higher.x, box.higher.y, box.less.z, 1.0f);
	glVertex4d(box.less.x, box.higher.y, box.less.z, 1.0f);

	glVertex4d(box.less.x, box.higher.y, box.less.z, 1.0f);
	glVertex4d(box.less.x, box.less.y, box.less.z, 1.0f);

	glVertex4d(box.less.x, box.higher.y, box.less.z, 1.0f);
	glVertex4d(box.less.x, box.higher.y, box.higher.z, 1.0f);

	glVertex4d(box.less.x, box.higher.y, box.higher.z, 1.0f);
	glVertex4d(box.less.x, box.less.y, box.higher.z, 1.0f);

	glVertex4d(box.less.x, box.less.y, box.higher.z, 1.0f);
	glVertex4d(box.less.x, box.less.y, box.less.z, 1.0f);

	glVertex4d(box.less.x, box.less.y, box.higher.z, 1.0f);
	glVertex4d(box.higher.x, box.less.y, box.higher.z, 1.0f);

	glVertex4d(box.higher.x, box.less.y, box.higher.z, 1.0f);
	glVertex4d(box.higher.x, box.less.y, box.less.z, 1.0f);

	glVertex4d(box.higher.x, box.less.y, box.less.z, 1.0f);
	glVertex4d(box.less.x, box.less.y, box.less.z, 1.0f);
	glEnd();
}

void sleep(int time) {
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

void threadDetach(void(*callback)()) {
	std::thread thr(callback);
	thr.detach();
}

void threadJoin(void(*callback)()) {
	std::thread thr(callback);
	thr.join();
}

double random(double start, double end) {
	return (start + static_cast <double> (rand()) / (RAND_MAX / end));
}

