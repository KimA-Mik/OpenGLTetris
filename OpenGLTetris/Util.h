#pragma once
#include "pch.h"


void glClearColorRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

void glColor3dRGB(uint8_t r, uint8_t g, uint8_t b);

//цвет
struct ColorRGB {
	uint8_t r, g, b;
};

struct ColorRGBf{
	GLfloat r, g, b;
};

void glColor3dRGB(ColorRGB col);
void myGLColor3f(ColorRGBf col);