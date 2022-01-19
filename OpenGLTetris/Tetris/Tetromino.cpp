#include "Tetromino.h"
//UTF-8 Коммент

Tetromino::Tetromino()
{
    int X = rand() % 7 + 1;
	iType = X;

	switch (iType)
	{
	case 1:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,0,X,0 };
		aTetro.at(2) = { 0,0,X,0 };
		aTetro.at(3) = { 0,0,X,0 };
		break;
	case 2:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,0,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 3:
		aTetro.at(0) = { 0,0,0,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,X,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 4:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,X,0,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 5:
		aTetro.at(0) = { 0,X,0,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,0,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 6:
		aTetro.at(0) = { 0,X,0,0 };
		aTetro.at(1) = { 0,X,0,0 };
		aTetro.at(2) = { 0,X,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 7:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,0,X,0 };
		aTetro.at(2) = { 0,X,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	}
}

Tetromino::Tetromino(int X)
{
repeat:
	iType = X;

	switch (iType)
	{
	case 1:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,0,X,0 };
		aTetro.at(2) = { 0,0,X,0 };
		aTetro.at(3) = { 0,0,X,0 };
		break;
	case 2:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,0,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 3:
		aTetro.at(0) = { 0,0,0,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,X,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 4:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,X,0,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 5:
		aTetro.at(0) = { 0,X,0,0 };
		aTetro.at(1) = { 0,X,X,0 };
		aTetro.at(2) = { 0,0,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 6:
		aTetro.at(0) = { 0,X,0,0 };
		aTetro.at(1) = { 0,X,0,0 };
		aTetro.at(2) = { 0,X,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	case 7:
		aTetro.at(0) = { 0,0,X,0 };
		aTetro.at(1) = { 0,0,X,0 };
		aTetro.at(2) = { 0,X,X,0 };
		aTetro.at(3) = { 0,0,0,0 };
		break;
	default:
		X = rand() % 7 + 1;
		goto repeat;
		break;
	}
}

Tetromino::~Tetromino()
{

}

void Tetromino::Rotate(int xPos, int yPos)
{
	TetroArray Rotated;

	for(int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			Rotated[y][4-x-1] = aTetro[x][y];
		}

	if (DoesItFitRotated(xPos,yPos, Rotated))
		aTetro = Rotated;
}

void Tetromino::ForceRotate()
{
	TetroArray Rotated;

	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			Rotated[y][4 - x - 1] = aTetro[x][y];
		}
	aTetro = Rotated;
}

void Tetromino::StartMoving(FieldArray& Field)
{
	CurrField = Field;
}


TetroArray Tetromino::GetFigure()
{
	return aTetro;
}


bool Tetromino::DoesItFit(int x, int y)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (x+i < 0 || x+i > 9 || y+j > 19) {
				if (aTetro[j][i])
					return false;
			}
			else if (CurrField[x+i][y+j] && aTetro[j][i])
				return false;
		}
	
	return true;
}

bool Tetromino::DoesItFitRotated(int xPos, int yPos, TetroArray& Rotated)
{
	//Ограничения поля

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++) {
			if (xPos + i < 0 || xPos + i > 9 || yPos + j > 17) {
				if (Rotated[j][i])
					return false;
			}
			else if (CurrField[xPos + i][yPos + j] && Rotated[j][i])
				return false;
		}
	return true;
}

