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
	//std::array<LTexture, 7> countText;
	//LTexture statsText;
	void UpdateTetroCount();
	void DrawTetroCount();
	void renderMino(const float x, const float y, const float z, const char& color);
	void renderTetromino(
		const Tetromino* Target,
		const char& state,
		float x, float y, const float z, const float scale );
	void DrawCube(ColorRGBf color, Point3f offset = {0,0,0}, Point3f angles = {0,0,0}, float scale = 1);

protected:

	int FieldXPos = -5;
	int FieldYPos = -10;


	int xPos = 3;
	int yPos = 0;
	
	void DrawField(int FieldX, int FieldY, FieldArray Field);

	//void HandleEvents(SDL_Event& E);

	void DrawTetromino(float xPos, float yPso, Tetromino* Target);
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

	//TTF_Font* scoreFont = nullptr;
	//SDL_Color scoreColor = { 12, 36, 97,255 };
	int GameScore = 0;
	//LTexture ScoreImage;

	//std::array<SDL_Rect, 7> BlocksClip;
	FieldArray Field;
	Tetromino* CurBlock = nullptr;
	Tetromino* NextBlock = nullptr;
	
	//LTexture TetroBlocks;

	std::array<std::array<float, 3>, 24> cubeData;
	std::array<std::array<float, 2>, 4> textureCoords;

};



