#include "Game.h"
//UTF-8 Коммент

Game::Game(bool* PorgramStatus)
{
	IsRunning = PorgramStatus;
	
	/*TetroBlocks.loadFromFile(mRenderer, "Assets/Images/Tetro.png");
	scoreFont = TTF_OpenFont("Assets/Fonts/Codename.ttf", 32);
	if (scoreFont == nullptr)
		SDL_Log("Unable to load font Assets/Fonts/Codename.ttf\n");
	
	for (int i = 0; i < 7; i++)
		BlocksClip[i] = { i * 32,0,32,32 };

	NextBlockText.loadFromRenderedText(mRenderer, scoreFont, "Следующий блок", scoreColor);*/


	cubeData = { {
	{0, 1, -1}, {1, 1, -1}, {1, 0, -1}, {0, 0, -1},
	{0, 1, 0}, {0, 1, -1}, {0, 0, -1}, {0, 0, 0},
	{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
	{1, 1, -1}, {1, 1, 0}, {1, 0, 0}, {1, 0, -1},
	{1, 1, -1}, {0, 1, -1}, {0, 1, 0}, {1, 1, 0},
	{0, 0, -1}, {1, 0, -1}, {1, 0, 0}, {0, 0, 0}
		} 
	};

	textureCoords = { {{0.25, 0}, {0, 0}, {0, 0.25}, {0.25, 0.25}} };
}

Game::~Game()
{
	/*TetroBlocks.free();
	for (int i = 0; i < 7; i++) {
		delete aScoreTetromino[i];
		countText[i].free();
	}
	TTF_CloseFont(scoreFont);
	NextBlockText.free();*/
}

void Game::Init()
{
	//поле
	
	for (int i = 0; i < 10; i++)
		Field.at(i) = { 0 };
	
	//счет
	for (int i = 0; i < 7; i++) {
		aScoreTetromino[i] = new Tetromino(i + 1);
		//std::string Score = "x" + std::to_string(tetroCount[i]);
		//countText[i].loadFromRenderedText(mRenderer, scoreFont, Score, scoreColor);
	}

	//statsText.loadFromRenderedText(mRenderer, scoreFont, "Статистика", scoreColor);

	UpdateScore(0);


	aScoreTetromino[0]->ForceRotate();
	aScoreTetromino[1]->ForceRotate();
	aScoreTetromino[3]->ForceRotate();
	aScoreTetromino[4]->ForceRotate();
	aScoreTetromino[5]->ForceRotate();
	aScoreTetromino[6]->ForceRotate();
	

	CurBlock = new Tetromino;
	NextBlock = new Tetromino;
	CurBlock->StartMoving(Field);
	CurBlock->Rotate(xPos, yPos);
}

int Game::Run()
{
	
	
	if (IsGameRunning && *IsRunning)
	{
		Sleep(10);//тик
		SpeedCount++;
		bForceDown = (SpeedCount == Speed);
		//каждый Speed тиков блок опускается на еденицу
		
		//обработка ввода
		/*while (SDL_PollEvent(&E)) {
			HandleEvents(E);
		}
		*/
		if (bForceDown) {
			SpeedCount = 0;
			pieceCount++;
			if (pieceCount % 50 == 0)
				if (Speed >= 10) Speed-=5;
			//каждые 50 спучков повышаем сложность

			// Проверяем, можно ли сдвинуть вниз
			if (CurBlock->DoesItFit(xPos, yPos + 1)) {
				//если да - двигаем
				yPos++;
			}//если нет, значит блок достиг дна
			else {
				//закрепляем его на поле
				TetroArray CurFigure = CurBlock->GetFigure();
				for(int i = 0; i < 4; i++)
					for (int j = 0; j < 4; j++) {
						if (CurFigure[j][i])
							Field[xPos + i][yPos + j] = CurFigure[j][i];
					}

				++tetroCount[CurBlock->GetType() - 1];
				UpdateTetroCount();
				//вызываем новый блок
				delete CurBlock;
				CurBlock = NextBlock;
				NextBlock = new Tetromino;
				CurBlock->StartMoving(Field);
				xPos = 3;
				yPos = 0;

				if (!CurBlock->DoesItFit(xPos, yPos)) {
					GameOver();
					IsGameRunning = false;
				}

				//проверяем линии
				for (int y = 0; y < 20; y++) {
					bool bLine = true;
					for (int x = 0; x < 10; x++) {
						bLine = Field[x][y] && bLine;
					}
					if (bLine) {
						vLines.push_back(y);
						//сюда можно вставить анимацию
					}
				}

				if (!vLines.empty())
					UpdateScore(vLines.size());

			}
		}
		
		if (!vLines.empty()) {
			
			for (auto& Line : vLines) {
				for (int x = 0; x < 10; x++) {
					for (int y = Line; y > 0; y--) {
						Field[x][y] = Field[x][y - 1];
					}
					Field[x][0] = 0;
				}
			}
			
			CurBlock->StartMoving(Field);
			vLines.clear();
		}
		
		
	}
	return 0;
}
int moveDown = 0;
int moveCounts = 0;
int xCoord = -1;
int yCoord = 2;
Tetromino toDraw(6);
void Game::Render() {
	//SDL_SetRenderDrawColor(mRenderer, 245, 246, 250, 255);
	//SDL_RenderClear(mRenderer);

	glClearColor(0.9607843137254902, 0.9647058823529412, 0.9803921568627451, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);
	glDepthFunc(GL_LESS);
	

	if (moveDown > 0) {
		DrawTetromino(xCoord, yCoord - (moveCounts * (1/8)), &toDraw);
		moveCounts++;
		if (moveCounts == 7) {
			moveCounts = 0;
			moveDown--;
			yCoord--;
		}
	}else
		DrawTetromino(xCoord, yCoord, &toDraw);
	

	






	DrawField(FieldXPos, FieldYPos, Field);
	DrawTetromino(FieldXPos + xPos, FieldYPos + yPos, CurBlock);
	DrawTetroCount();
	DrawScore();
	DrawNextBlock();
}

void Game::DrawCube(ColorRGBf color, Point3f offset, Point3f angles, float scale)
{
	glPushMatrix();
	glOrtho(-aspect, aspect, -1, 1, -1, 1);
	glRotatef(angles.x, 1.0, 0.0, 0.0);		//поворот относи-тельно оси X
	glRotatef(angles.y, 0.0, 1.0, 0.0);
	glRotatef(angles.z, 0.0, 0.0, 1.0);

	glBegin(GL_POLYGON);
	glColor3f(color);
	for (int i = 0; i < 24; i++)
		glVertex3f((cubeData[i][0] - 0.5 + offset.x) / scale, (cubeData[i][1] - 0.5 + offset.y) / scale, (cubeData[i][2] - 0.5 + offset.z) / scale);
	glEnd();
	glLineWidth(2);
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 24; i++) {
		if (i % 4 == 0) {
			glEnd();
			glBegin(GL_LINE_STRIP);
		}
		glVertex3f((cubeData[i][0] - 0.5 + offset.x) / scale, (cubeData[i][1] - 0.5 + offset.y) / scale, (cubeData[i][2] - 0.5 + offset.z) / scale);

	}
	glEnd();
	glPopMatrix();
	glLineWidth(1);

}


void Game::DrawField(int FieldX, int FieldY, FieldArray DrawableField)
{

	for(int x = 0; x < 10; x++)
		for (int y = 0; y < 20; y++) {
			if (DrawableField[x][y]) {
				float angle = -(y + FieldY);
				DrawCube(colors[DrawableField[x][y] - 1], { (float)(x + FieldX),-(float)(y + FieldY),0 }, { angle,0,0 }, 10);
			}
		}
	return;
}

//void Game::HandleEvents(SDL_Event& E)
//{
//	switch (E.type)
//	{
//	case SDL_QUIT:
//		*IsRunning = false;
//		break;
//	case SDL_KEYUP:
//		switch (E.key.keysym.sym)
//		{
//		case SDLK_UP:
//			CurBlock->Rotate(xPos,yPos);
//			break;
//
//		case SDLK_DOWN:
//			yPos += (CurBlock->DoesItFit(xPos, yPos + 1)) ? 1 : 0;
//			break;
//		
//		case SDLK_LEFT:
//			xPos -= (CurBlock->DoesItFit(xPos - 1, yPos)) ? 1 : 0;
//			break;
//
//		case SDLK_RIGHT:
//			xPos += (CurBlock->DoesItFit(xPos + 1, yPos)) ? 1 : 0;
//			break;
//		}
//		break;
//	}
//}

void Game::DrawTetromino(float xPos, float yPos, Tetromino* Target)
{
	if (Target == nullptr)
		return;
	float angle = -(yPos);
	std::array<std::array<int, 4>, 4> Figure = Target->GetFigure();
	for(int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) 
			if (Figure[y][x])
				DrawCube(colors[Target->GetType()-1], { (float)(x + xPos),-(float)(y + yPos),0}, { angle,0,0}, 10);
}

void Game::renderMino(
	const float x, const float y, const float z, const char& color
) {

	const float
		textureX = (color & 0b11) * 0.25,
		textureY = (color >> 2) * 0.25;
	for (int i = 0; i != 24; i++) {
		auto vertex = cubeData[i];
		auto textureCoord = textureCoords[i & 0b11];
		glTexCoord2f(textureX + textureCoord[0], textureY + textureCoord[1]);
		glVertex3f(vertex[0] + x, vertex[1] + y, vertex[2] + z);
	}
}

void Game::renderTetromino(
	const Tetromino* Target,
	const char& state,
	float x, float y, const float z, const float scale
) {
	glPushMatrix();
	glTranslatef(x, y, z);
	glScalef(scale, scale, scale);
	glBegin(GL_QUADS);
	const auto color = Target->GetType() - 1;
	//for (const auto& offset : tetrimino->getMinoes(state))
		//renderMino(offset.x, -offset.y, 0, color);
	glEnd();
	glPopMatrix();
}

void Game::HandleInput(WPARAM key)
{
	switch (key)
	{

	case VK_UP:
		yCoord++;
		CurBlock->Rotate(xPos, yPos);
		break;

	case VK_DOWN:
		moveDown++;
		yPos += (CurBlock->DoesItFit(xPos, yPos + 1)) ? 1 : 0;
		break;

	case VK_LEFT:
		xPos -= (CurBlock->DoesItFit(xPos - 1, yPos)) ? 1 : 0;
		break;

	case VK_RIGHT:
		xPos += (CurBlock->DoesItFit(xPos + 1, yPos)) ? 1 : 0;
		break;
	}
}

void Game::UpdateTetroCount()
{
	/*for (int i = 0; i < 7; i++) {
		std::string Score = "x" + std::to_string(tetroCount[i]);
		countText[i].loadFromRenderedText(mRenderer, scoreFont, Score, scoreColor);
	}*/
}

void Game::DrawTetroCount()
{
	int xCoords = 0;
	int yCoords = 15;
	//for (int i = 0; i < 7; i++) {
		//DrawTetromino(xCoords, yCoords + 96 * i, aScoreTetromino[i]);
		//countText[i].render(xCoords + 128, 64 +  yCoords + 96 * i);
	//}

	//statsText.render(xCoords, yCoords);

}

void Game::UpdateScore(int LinesCount)
{
	switch (LinesCount)
	{
	case 0:
		GameScore = 0;
		break;
	case 1:
		GameScore += 100;
		break;
	case 2:
		GameScore += 300;
		break;
	case 3:
		GameScore += 700;
		break;
	case 4:
		GameScore += 1500;
		break;
	}

	//std::string Score = "Текущий счет: " + std::to_string(GameScore);
	//ScoreImage.loadFromRenderedText(mRenderer, scoreFont, Score, scoreColor);
}

void Game::DrawScore()
{
	//ScoreImage.render(FieldXPos, 10);
}

void Game::DrawNextBlock()
{
	int x = FieldXPos + 32 * 10 + 100;
	//NextBlockText.render(x, 100);
	DrawTetromino(x, 150, NextBlock);
}

void Game::GameOver()
{
	/*SDL_SetRenderDrawColor(mRenderer, 245, 246, 250, 255);
	SDL_RenderClear(mRenderer);
	
	TTF_Font* Font = TTF_OpenFont("Assets/Fonts/Codename.ttf", 64);
	LTexture GameOver;
	GameOver.loadFromRenderedText(mRenderer, Font, "Игра окончена", scoreColor);
	TTF_CloseFont(Font);

	GameOver.render(1280 / 2 - GameOver.getWidth() / 2, 150);

	std::string Score = "Итоговый счет: " + std::to_string(GameScore);
	ScoreImage.loadFromRenderedText(mRenderer, scoreFont, Score, scoreColor);

	ScoreImage.render(1280 / 2 - ScoreImage.getWidth() / 2, 350);

	SDL_RenderPresent(mRenderer);

	bool Wait = true;

	while (Wait && *IsRunning) {
		while (SDL_PollEvent(&E)) {
			if (E.type == SDL_QUIT)
				*IsRunning = false;
			if (E.type == SDL_KEYDOWN)
				Wait = false;
		}
	}
	GameOver.free();*/

}
