#pragma once
#include <Windows.h>	
// Заголовочный файл для Windows
#include <gl\gl.h>		
	// Заголовочный файл для библиотеки OpenGL32 
#include <gl\glu.h>		
	// Заголовочный файл для библиотеки GLu32 
#include <gl\glaux.h>		
	// Заголовочный файл для библиотеки GLaux
#include <cstdint>



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
void glColor3f(ColorRGBf col);