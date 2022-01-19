#pragma once
#include <array>
#include <vector>
//поле 10 на 20
#define FieldArray std::array<std::array<int,20>,10>
#define TetroArray std::array<std::array<int, 4>, 4>

struct Coordinates {
	char x, y;
};

struct TetriminoStaticData {
	char color;
	std::array<float, 2> queueOffset;
	std::array<std::array<Coordinates, 4>, 4> stateData;
	std::array<std::vector<Coordinates>, 8> kickData;
	std::array<char, 4> baseData, topData, leftData, widthData;
	std::array<std::vector<std::array<char, 2>>, 4> hardDropDepthData;
};
//
//Сам тетромино
class Tetromino
{
public:
	Tetromino();
	Tetromino(int X);
	~Tetromino();

	void Rotate(int xPos, int yPos);
	void ForceRotate();

	void StartMoving(FieldArray& Field);

	TetroArray GetFigure();

	bool DoesItFit(int x, int y);
	
	int GetType() const { return iType; }

private:
	FieldArray CurrField; //массив на начало движения
	TetroArray aTetro ; // 4 на 4 тетромино
	int iType; // тип тетромино 1 - 7
	bool DoesItFitRotated(int xPos, int yPos, TetroArray& Rotated);
	TetriminoStaticData staticData;
	

};

