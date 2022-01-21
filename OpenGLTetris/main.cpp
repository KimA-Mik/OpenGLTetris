#include "pch.h"
#include "Tetris/Game.h"

constexpr int width = 1600;
constexpr int height = 900;

float aspect = (float)width / (float)height;


int curFigure = 1;
bool transfom = false;
float size = 1;
float x_cobe = 30.0;
float y_cobe = 70.0;
bool isRunning = true;
Game tetroGL(&isRunning);


static HGLRC hRC;
// ���������� �������� ����������
static HDC hDC;
// ��������� �������� ���������� GDI
BOOL keys[256];
// ������ ��� ��������� ��������� ����������

GLvoid InitGL(GLsizei Width, GLsizei Height);
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height);
GLvoid DrawGLScene(GLvoid);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	MSG		msg;	// ��������� ��������� Windows
	WNDCLASS	wc; 	// ��������� ������ Windows 
				// ��� ��������� ���� ����
	HWND	hWnd;	// ���������� ����������� ����

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = nullptr;
	wc.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = L"OpenGL WinClass";


	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"������ ����������� ������ ����.",
			L"������", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	hWnd = CreateWindow(
		L"OpenGL WinClass",
		L"Tetro 3D",
		// ��������� ������ ����
		//WS_POPUP |
		WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS,
		0, 0,		// ������� ���� �� ������
		width, height,	// ������ � ������ ����
		nullptr,
		nullptr,
		hInstance,
		nullptr);


	if (!hWnd)
	{
		MessageBox(0, L"������ �������� ����.",
			L"������", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	DEVMODE dmScreenSettings;	// ����� ������
	memset(&dmScreenSettings, 0, sizeof(DEVMODE));
	// ������� ��� �������� ���������
	dmScreenSettings.dmSize = sizeof(DEVMODE);
	// ������ ��������� Devmode
	dmScreenSettings.dmPelsWidth = width;
	// ������ ������
	dmScreenSettings.dmPelsHeight = height;
	// ������ ������
	dmScreenSettings.dmFields = DM_PELSWIDTH |
		DM_PELSHEIGHT;
	// ����� �������
	//ChangeDisplaySettings(&dmScreenSettings, CDS_RESET);
	// ������������ � ������ �����

	srand(time(nullptr));

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	while (1)
	{
		// ��������� ���� ���������
		while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}
		tetroGL.Run();
		tetroGL.Render();
		SwapBuffers(hDC);	// ����������� ����� ������

		if (keys[VK_ESCAPE])
		{
			isRunning = false;
			SendMessage(hWnd, WM_CLOSE, 0, 0);// ���� ESC - �����
		}
		
		


	}
	return 0;
}

GLvoid InitGL(GLsizei Width, GLsizei Height)	//������� ����� �������� ���� GL
{
	gladLoadGL();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// ������� ������ � ������ ����
}

GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
	if (Height == 0)
		// �������������� ������� �� ����, 
		//���� ���� ������� ����
		Height = 1;
	glViewport(0, 0, Width, Height);
	// ����� ������� ������� ������ �
	//������������� ��������������
}



LRESULT CALLBACK WndProc(HWND	hWnd,
	UINT	message,
	WPARAM	wParam,
	LPARAM	lParam)
{
	RECT	Screen;
	// ������������ ������� ��� �������� ����
	GLuint	PixelFormat;
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		// ������ ���� ���������
		1,			// ����� ������ 
		PFD_DRAW_TO_WINDOW |	// ������ ��� ����
		PFD_SUPPORT_OPENGL |	// ������ ��� OpenGL
		PFD_DOUBLEBUFFER,	// ������ ��� �������� ������
		PFD_TYPE_RGBA,		// ��������� RGBA ������
		16,			// ����� 16 ��� ������� �����
		0, 0, 0, 0, 0, 0,   // ������������� �������� ����� 
		0,			// ��� ������ ������������
		0,			// ��������� ��� ������������ 
		0,			// ��� ������ �����������
		0, 0, 0, 0,	// ���� ����������� ������������ 
		16,		// 16 ������ Z-����� (����� �������)  
		0,			// ��� ������ ���������
		0,		// ��� ��������������� ������� 
		PFD_MAIN_PLANE,		// ������� ���� ���������
		0,			// ������ 
		0, 0, 0			// ����� ���� ������������ 
	};

	switch (message)	// ��� ���������
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		// �������� �������� ���������� ��� ����
		PixelFormat = ChoosePixelFormat(hDC, &pfd);
		// ����� ��������� ���������� ��� ������� ��������

		if (!PixelFormat)
		{
			MessageBox(0, L"�� ������ ���������� ������ �������.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			// ��� ��������� �������, ���
			// ��������� ������ �����������
			break;		// �������������� ������� ����
		}
		if (!SetPixelFormat(hDC, PixelFormat, &pfd))
		{
			MessageBox(0, L"������ ������� �� ����������.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}
		hRC = wglCreateContext(hDC);
		if (!hRC)
		{
			MessageBox(0, L"�������� ��������������� �� ������.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}
		if (!wglMakeCurrent(hDC, hRC))
		{
			MessageBox(0, L"���������� �������������� GLRC.",
				L"������", MB_OK | MB_ICONERROR);
			PostQuitMessage(0);
			break;
		}
		GetClientRect(hWnd, &Screen);
		InitGL(Screen.right, Screen.bottom);
		tetroGL.Init();
		break;

	case WM_DESTROY:
	case WM_CLOSE:
		ChangeDisplaySettings(NULL, 0);
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		keys[wParam] = TRUE;
		break;

	case WM_KEYUP:
		tetroGL.HandleInput(wParam);
		keys[wParam] = FALSE;
		break;

	case WM_SIZE:
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;

	default:
		return (DefWindowProc(hWnd, message,
			wParam, lParam));
	}

	return (0);
}