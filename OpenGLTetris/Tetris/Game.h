#pragma once
#include <iostream>
#include <array>
#include <vector>
#include "Tetromino.h"
#include "../Util.h"
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#define FieldArray std::array<std::array<int,20>,10>
#define TetroArray std::array<std::array<int, 4>, 4>

extern float aspect;
struct Point3f {
	float x, y, z;
};


//UTF-8 Коммент


constexpr std::array<ColorRGBf, 7> colors = {	0.760784, 0.211765, 0.0862745,	//red
												0.266667, 0.741176, 0.196078,	//green
												0, 0.592157, 0.901961,			//blue
												0.882353, 0.694118, 0.172549,	//yellow
												0.54902, 0.478431, 0.901961,	//purple
												0.184314, 0.211765, 0.25098,	//black
												0.0980392, 0.164706, 0.337255}; //void

constexpr float frameTime = 16;
constexpr float timeOfFrame = 1 / 16.0f;

class Game 
{
public:
	Game(bool* PorgramStatus);
	~Game();
	

	void Init();
	int Run();
	void HandleInput(WPARAM	key);
	void Render();


private:
	bool* IsRunning = nullptr;



	//стстистика установленныъ блоков
	std::array<Tetromino*, 7> aScoreTetromino = { 0 };
	std::array<int, 7> tetroCount = { 0 };
	void UpdateTetroCount();
	void DrawTetroCount();
	void renderMino(const float x, const float y, const float z, const char& color);
	void renderTetromino(
		const Tetromino* Target,
		const char& state,
		float x, float y, const float z, const float scale );
	static void DrawCube(ColorRGBf color, Point3f offset = {0,0,0}, Point3f angles = {0,0,0}, float scale = 1);

protected:

	static int FieldXPos;
	static int FieldYPos;


	int xPos = 3;
	int yPos = 0;
	
	void DrawField(int FieldX, int FieldY, FieldArray Field);

	//void HandleEvents(SDL_Event& E);

	void DrawTetromino(float xPos, float yPso, Tetromino* Target, Point3f angles = { 0,0,0 });
	void UpdateScore(int LinesCount);
	virtual void DrawScore();
	virtual void DrawNextBlock();

	virtual void GameOver();

	int Speed = 100;
	int SpeedCount = 0;
	bool bForceDown = false;
	int pieceCount = 0;

	std::vector<int> vLines;

	
	bool IsGameRunning = true;

	int GameScore = 0;

	FieldArray Field;
	Tetromino* CurBlock = nullptr;
	Tetromino* NextBlock = nullptr;


	static std::array<std::array<float, 3>, 24> cubeData;
	std::array<std::array<float, 2>, 4> textureCoords;

	float showAngle = 0.0;

	int moveDown = 0;
	bool forceMoveDown = false;
	int moveCounts = 0;
	SYSTEMTIME beginTime;


	class flyingCube {
	public:
		Point3f Pos;
		Point3f Angle;
		Point3f Rotation;
		Point3f Speed;
		int Color;
		flyingCube(float x, float y, int color) {
			Color = color;
			Pos.x = x + FieldXPos;
			Pos.y = y + FieldYPos;
			Pos.z = 0;
			Rotation.x = rand() % 40 + 5;
			Rotation.y = rand() % 40 + 5;
			Rotation.z = rand() % 40 + 5;
			Speed.x = (rand() % 10) / 100.;
			Speed.y = (rand() % 10) / 100.;
			Speed.z = (rand() % 10) / 100.;
			Angle.x = 0;
			Angle.y = -(y + FieldYPos);
			Angle.z = 0;
		}
		void Move() {
			
			Pos.x += Speed.x * timeOfFrame;
			Pos.y += Speed.y * timeOfFrame;
			Pos.z += Speed.z * timeOfFrame;

			//Angle.x += Rotation.x * timeOfFrame;
			//Angle.y += Rotation.y * timeOfFrame;
			//Angle.z += Rotation.z * timeOfFrame; 
		}
		void Render() {
			glPushMatrix();
			//glTranslatef(-Pos.x, -Pos.y, -Pos.z);
			DrawCube(colors[Color - 1], {Pos.x, -Pos.y, Pos.z }, Angle, 10);
			glPopMatrix();
		}
	};
	std::vector<flyingCube> flies;

};



