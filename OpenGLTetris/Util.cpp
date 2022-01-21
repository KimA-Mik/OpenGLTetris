#include "Util.h"

void glClearColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	glClearColor(r / 255., g / 255., b / 255., a / 255.);
}

void glColor3dRGB(uint8_t r, uint8_t g, uint8_t b)
{
	glColor3d(r / 255., g / 255., b / 255.);
}

void glColor3dRGB(ColorRGB col)
{
	glColor3d(col.r / 255., col.g / 255., col.b / 255.);
}

void myGLColor3f(ColorRGBf col)
{
	glColor3f(col.r, col.g, col.b);
}
