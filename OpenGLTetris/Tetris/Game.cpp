#include "Game.h"

int Game::FieldXPos = -5;
int Game::FieldYPos = -10;


std::array<std::array<float, 3>, 24> Game::cubeData = { {
	{0, 1, -1}, {1, 1, -1}, {1, 0, -1}, {0, 0, -1},
	{0, 1, 0}, {0, 1, -1}, {0, 0, -1}, {0, 0, 0},
	{1, 1, 0}, {0, 1, 0}, {0, 0, 0}, {1, 0, 0},
	{1, 1, -1}, {1, 1, 0}, {1, 0, 0}, {1, 0, -1},
	{1, 1, -1}, {0, 1, -1}, {0, 1, 0}, {1, 1, 0},
	{0, 0, -1}, {1, 0, -1}, {1, 0, 0}, {0, 0, 0}
		}
};


float vertex[] = { -1,-1,1, 1,-1,1, 1,1,1, -1,1,1 };
float normal[] = { -1,-1,1, 1,-1,1, 1,1,1, -1,1,1 };

Game::Game(bool* PorgramStatus)
{
	IsRunning = PorgramStatus;

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
	texture = _loadTexture("img/fon.bmp");

	for (int i = 0; i < 10; i++)
		Field.at(i) = { 0 };

	
	//счет
	for (int i = 0; i < 7; i++) {
		aScoreTetromino[i] = new Tetromino(i + 1);
		
	}




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
	IsGameRunning = true;
	float xCam = 0;
	float yCam = 0;
	lightning = false;
	flies.clear();
	loadTextureCube();
}

int Game::Run()
{
	
	
	if (IsGameRunning && *IsRunning)
	{
		Sleep(10);//тик
		SpeedCount++;
		bForceDown = (SpeedCount == Speed);
		//каждый Speed тиков блок опускается на еденицу

		if (bForceDown) {
			SpeedCount = 0;
			pieceCount++;
			if (pieceCount % 50 == 0)
				if (Speed >= 10) Speed-=5;
			//каждые 50 спучков повышаем сложность

			// Проверяем, можно ли сдвинуть вниз
			if (CurBlock->DoesItFit(xPos, yPos + 1)) {
				//если да - двигаем
				moveDown++;
				//yPos++;
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
				//вызываем новый блок
				Tetromino* temp = new Tetromino();
				//алгоритм рандомизации
				if (temp->GetType() == NextBlock->GetType()) {
					delete temp;
					temp = new Tetromino();
				}
				delete CurBlock;
				CurBlock = NextBlock;
				NextBlock = temp;
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

void Game::Render() {
	glClearColor(0.9607843137254902, 0.9647058823529412, 0.9803921568627451, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (lightning) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_NORMALIZE);
	}
	else {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_NORMALIZE);
	}


	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
	glDepthFunc(GL_LESS);
	//float position[] = { 0,1,3,0 };
	float position[] = { 10,5,10,0 };
	

	if (!IsGameRunning) {
		glPushMatrix();
		MoveCamera();
		DrawSkybox();
		for (auto& fly : flies) {
			fly.Move();
			fly.Render();
		}
		glPopMatrix();
		SYSTEMTIME endTime;
		GetSystemTime(&endTime);
		if (endTime.wMinute > beginTime.wMinute
			|| endTime.wSecond - beginTime.wSecond > 5)
			Init();
		return;
	}

	glPushMatrix();
	MoveCamera();
	DrawSkybox();
	DrawField(FieldXPos, FieldYPos, Field);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	if (moveDown > 0) {
		int speedControl = 10;
		if (forceMoveDown) speedControl /= 5;
		DrawTetromino(FieldXPos + xPos, FieldYPos + yPos + (moveCounts * (1/ (float)speedControl)), CurBlock);
		moveCounts++;
		if (moveCounts == speedControl - 1) {
			moveCounts = 0;
			moveDown--;
			if (CurBlock->DoesItFit(xPos, (int)yPos + 1) && CurBlock->DoesItFit(xPos, (int)yPos))
				yPos++;
			else
				forceMoveDown = false;
		}
	}else
		DrawTetromino(FieldXPos + xPos, FieldYPos + yPos, CurBlock);
	DrawNextBlock();

	glPopMatrix();
}

void Draw() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, vertex);
		glNormalPointer(GL_FLOAT, 0, normal);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_LINE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}

void Game::DrawCube(ColorRGBf color, Point3f offset, Point3f angles, float scale)
{
	glPushMatrix();
	glOrtho(-aspect, aspect, -1, 1, -1, 1);
	glRotatef(angles.x, 1.0, 0.0, 0.0);		//поворот относи-тельно оси X
	glRotatef(angles.y, 0.0, 1.0, 0.0);
	glRotatef(angles.z, 0.0, 0.0, 1.0);
	
	/*myGLColor3f(color);
	glScalef(0.09, 0.09, 0.09);
	glTranslatef(offset.x - 0.5, offset.y + 0.5 , offset.z);
	Draw();
	glRotatef(90, 1, 0, 0);
	Draw();
	glRotatef(90, 1, 0, 0);
	Draw();
	glRotatef(90, 1, 0, 0);
	Draw(); 
	glRotatef(90, 0, 1, 0);
	Draw();
	glRotatef(180, 0, 1, 0);
	Draw();*/
	//glMaterialf(GL_FRONT_AND_BACK, GL_SPECULAR,0);
	glBegin(GL_POLYGON);
	myGLColor3f(color);
	for (int i = 0; i < 24; i++) {
		glNormal3f(cubeData[i][0], cubeData[i][1], cubeData[i][2]);
		glVertex3f((cubeData[i][0] - 0.5 + offset.x) / scale, (cubeData[i][1] - 0.5 + offset.y) / scale, (cubeData[i][2] - 0.5 + offset.z) / scale);
	}
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

void Game::MoveCamera()
{
	
	glRotatef(xCam, 1.0, 0.0, 0.0);
	glRotatef(yCam, 0.0, 1.0, 0.0);


	
}

const float PI = 3.14159265358979323846;

void Game::DrawField(int FieldX, int FieldY, FieldArray DrawableField)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_POLYGON);
		glColor4f(1, 1, 1, 0.5);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-0.31, -0.965, 0.4);
		glNormal3f(0, 0, 1);
		glTexCoord2f(cos(xCam * 180 / PI), 0.0 );
		glVertex3f(0.255, -0.965, 0.4);
		glNormal3f(0, 0, 1);
		glTexCoord2f(cos(xCam * 180 / PI) - sinf(yCam * 180 / PI), sinf(yCam * 180 / PI) + cos(xCam * 180 / PI));
		glVertex3f(0.255, 0.965, 0.4);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, sinf(yCam * 180 / PI));
		glVertex3f(-0.31, 0.965, 0.4);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	auto res = glGetError();

	glPushMatrix();
	glColor3f(0, 1, 0);
	//glRotatef(90, 0, 1, 0);
	//glTranslatef(0, 11, 0);
	//glScalef(10, 10, 10);
	/*glBegin(GL_POLYGON);
	glVertex3f(-5, -10, -10);
	glVertex3f(-5, -10, 10);
	glVertex3f(-5, 10, -10);
	glVertex3f(-5, 10, 10);
	glEnd();*/

	glPopMatrix();


	for(int x = 0; x < 10; x++)
		for (int y = 0; y < 20; y++) {
			if (DrawableField[x][y]) {
				float angle = -(y + FieldY);
				DrawCube(colors[DrawableField[x][y] - 1], { (float)(x + FieldX),-(float)(y + FieldY),0 }, { angle,0,0 }, 10);
			}
		}
}

//float cube[] = { -3,-3,3, 3,-3,3, 3,3,3, -3,3,3, -3,-3,-3, 3,-3,-3, 3,3,-3, -3,3,-3 };


float cube[] = { -1,-1,1, 1,-1,1, 1,1,1, -1,1,1,
				- 1,-1,-1, 1,-1,-1, 1,1,-1, -1,1,-1 };


unsigned int cubeInd[] = { 0,1,2,2,3,0, 4,5,6,6,7,4,
							1,5,6,6,2,1, 0,4,7,7,3,0,
							3,2,6,6,7,3, 0,1,5,5,4,0 };

int cubeIntCnt = sizeof(cubeInd) / sizeof(cubeInd[0]);

const char* skybox[6] = {	"img/Right.bmp",
							"img/Left.bmp",
							"img/Top.bmp",
							"img/Bottom.bmp",
							"img/Front.bmp",
							"img/Back.bmp"
};
float normals[24];
void Game::loadTextureCube() {
	int width = 1024;
	int height = 1024;
	glGenTextures(1, &	skyboxTextures);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextures);
	for (int i = 0; i < 6; i++) {
		
		std::vector<unsigned char> data = _readFile(skybox[i]);

		for (int i = 0; i < width * height; ++i)
		{
			int index = i * 3;
			unsigned char B, R;
			B = data[index];
			R = data[index + 2];

			data[index] = R;
			data[index + 2] = B;
		}
		unsigned char* set = &data[0];

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
		data.clear();
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, set);
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	for (int i = 0; i < 24; i++)
		normals[i] = -cube[i];
}

void Game::DrawSkybox()
{
	glUseProgram(0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glColor3f(1, 1, 1);
	glPushMatrix();
	glScalef(1.2, 1.2, 1.2);
	glTranslatef(0, 0, -0.5);
	glRotatef(180, 1, 0, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextures);

	glVertexPointer(3, GL_FLOAT, 0, cube);
	glTexCoordPointer(3, GL_FLOAT, 0, cube);
	glNormalPointer(GL_FLOAT, 3, normals);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
		glDrawElements(GL_TRIANGLES, cubeIntCnt, GL_UNSIGNED_INT, cubeInd);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glPopMatrix();
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}




void Game::DrawTetromino(float xPos, float yPos, Tetromino* Target, Point3f angles)
{
	if (Target == nullptr)
		return;
	float angle = -(yPos);
	std::array<std::array<int, 4>, 4> Figure = Target->GetFigure();
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			if (Figure[y][x])
				DrawCube(colors[Target->GetType() - 1], { (float)(x + xPos),-(float)(y + yPos),0 }, { angle,angles.y,angles.z }, 10);
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
		CurBlock->Rotate(xPos, yPos);
		break;

	case VK_DOWN:
		moveDown += (CurBlock->DoesItFit(xPos, yPos + 1) && !moveDown) ? 1 : 0;
		break;

	case VK_LEFT:
		xPos -= (CurBlock->DoesItFit(xPos - 1, yPos ) && !moveDown) ? 1 : 0;
		break;

	case VK_RIGHT:
		xPos += (CurBlock->DoesItFit(xPos + 1, yPos) && !moveDown) ? 1 : 0;
		break;
	case VK_SPACE:
		while (CurBlock->DoesItFit(xPos, yPos +  moveDown))
			moveDown++;
		bForceDown = true;
		forceMoveDown = true;
		break;
	case 'W':
		xCam += 5.0f;
		if (xCam > 360.0f)
			xCam -= 360.0f;
		break;
	case 'S':
		xCam -= 5.0f;
		if (xCam < 0.0f)
			xCam += 360.0f;
		break;
	case 'A':
		yCam += 5.0f;
		if (yCam > 360.0f)
			yCam -= 360.0f;
		break;
	case 'D':
		yCam -= 5.0f;
		if (yCam < 0.0f)
			yCam += 360.0f;
		break;
	case 'R':
		xCam = 0.0f;
		yCam = 0.0f;
		break;
	case 'L':
		lightning = !lightning;
		break;
	}
}


void Game::DrawNextBlock()
{
	glPushMatrix();
	int x = FieldXPos + 10;
	glTranslatef(0.5, 0.5, 0);
	showAngle += 45 * timeOfFrame;
	DrawTetromino(-2, 0, NextBlock, { 0, showAngle ,0 });
	DrawCube({ 0.5,0.5,0.5 }, { 0,0,-2 }, { 0,0,0 }, 4);

	glPopMatrix();
}

void Game::GameOver()
{

	flies.reserve(200);
	for (int x = 0; x < 10; x++)
		for (int y = 0; y < 20; y++) {
			if (Field[x][y]) {
				flies.emplace_back(x, y, Field[x][y]);
			}
		}
	GetSystemTime(&beginTime);
}

std::vector<unsigned char> Game::_readFile(const char* path)
{
	std::ifstream file(path, std::ios::binary);

	std::streampos size;
	file.seekg(0, std::ios::end);
	size = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<unsigned char> data(size);
	file.read((char*)&data[0], size);

	return data;
}

GLuint Game::_loadTexture(const char* path)
{
	int width = 750;
	int height = 750;
	GLuint tempText;
	std::vector<unsigned char> data = _readFile(path);

	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;
	}
	unsigned char* set = &data[0];
	glGenTextures(1, &tempText); //генерация уникального имени
	auto res = glGetError();
	glBindTexture(GL_TEXTURE_2D, tempText); //первичное связывание
	//Режимы фильтрации
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, set);

	//Build2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, set); //связывание изображения с текстурой

	
	//Фильтрация со смешиванием и без
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); //смешивания Взаимодействие с поверхностью
	glBindTexture(GL_TEXTURE_2D, 0);
	data.clear();
	return tempText;
}
