#include "p_pchmain.h"
#include "p_logic.h"
#include "p_graphics.h"

GameLogic Logic;

Graphics::Graphics()
{
	hDeviceContext = nullptr;
	renderingContext = nullptr;
	pixelFormat = 0;
	CurrentShadeBrush = nullptr;
	CurrentColorBrush = nullptr;
	IsGameActive = true;
	IsGamePaused = true;

	msec = 0;

	color = pause = 0;
	r = g = b = 0;
	Freq = StartTime = EndTime = CurrentTime = 0;
	pixelFormatDescriptor =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
		32,                   // Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                   // Number of bits for the depthbuffer
		8,                    // Number of bits for the stencilbuffer
		0,                    // Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
}
Graphics::~Graphics()
{
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(renderingContext);
	ReleaseDC(hWnd, hDeviceContext);
}
bool Graphics::CreateGLContext()
{
#ifdef WIN32
	hDeviceContext = GetDC(hWnd);
#endif // WIN32

#ifdef WEB

#endif // WEB

	pixelFormat = ChoosePixelFormat(hDeviceContext, &pixelFormatDescriptor);
	lastResult = SetPixelFormat(hDeviceContext, pixelFormat, &pixelFormatDescriptor);
	if (lastResult == false)
	{
		return false;
	}
	renderingContext = wglCreateContext(hDeviceContext);
	lastResult = (bool)wglMakeCurrent(hDeviceContext, renderingContext);
	if (lastResult == false)
	{
		return false;
	}

	return true;
}
void Graphics::Rect(int x1, int y1, int x2, int y2, GLfloat* Color)
{
	if (Color == nullptr)
	{
		printf("Rect Error: Color is equal to nullptr\n");
#ifdef WINDOWS
		OutputDebugString(L"Rect Error: Color is equal to nullptr\n");
#endif // WINDOWS
		return;
	}
	int x, y;
	y = y1;
	for (x = x1; x < x2; x++)
	{
		Pixel(x, y, Color);
		for (y = y1; y < y2; y++)
		{
			Pixel(x, y, Color);
		}
	}
}
void Graphics::Pixel(GLint x, GLint y, GLfloat* Color)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, 1, 1);
	glClearColor(Color[0], Color[1], Color[2], Color[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}
bool Graphics::InitDeviceResources()
{
#ifdef WIN32
	RECT rect = { 0 };
	lastResult = GetClientRect(hWnd, &rect);
#endif // WIN32

	hDeviceContext = GetDC(hWnd);
	CurrentShadeBrush = DimGray;

	return lastResult;
}
bool Graphics::InitGraphics()
{
	bool lastResult = false;
	lastResult = InitDeviceResources();

	if (lastResult == true) {
		lastResult = CreateGLContext();
	}

#ifdef WINDOWS
	QueryPerformanceFrequency((LARGE_INTEGER*)&Freq);
#endif // WINDOWS
	float msec = 1000 / (float)Freq;

	return lastResult;
}
void Graphics::RenderGame()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, (float)800 / (float)600, 0.1f, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	PauseChecker();
	BackgroundColorSwitch();
	NetRender();
	//ScoreRender();

	//float cpuPaddle[4] = { Logic.CPUPaddle.y, Logic.CPUPaddle.x, Logic.CPUPaddle.y2, Logic.CPUPaddle.x2 };
	//Rect(cpuPaddle[0], cpuPaddle[1], cpuPaddle[2], cpuPaddle[3], CurrentShadeBrush);

	//float playerPaddle[4] = { PlayerPaddleY, Logic.PlayerPaddleX, PlayerPaddleY2, Logic.PlayerPaddleX2 };
	//Rect(playerPaddle[0], playerPaddle[1], playerPaddle[2], playerPaddle[3], CurrentShadeBrush);

	//float ball[4] = { Logic.Ball.y, Logic.Ball.x, Logic.Ball.y2, Logic.Ball.x2 };
	//Rect(ball[0], ball[1], ball[2], ball[3], CurrentShadeBrush);

	if (IsGameActive || IsGamePaused)
	{
		Logic.UpdateState();
	}

	SwapBuffers(hDeviceContext);
}
void Graphics::PauseChecker()
{

	if (!IsGameActive || IsGamePaused)
	{
	}
	else if (IsGameActive || IsGamePaused)
	{
		IsGameActive = true;
		IsGamePaused = true;
	}
}
void Graphics::PauseGame()
{
	 BackgroundColorSwitch();
	 NetRender();
	 ScoreRender();

	 float cpuPaddle[4] = { Logic.CPUPaddle.y, Logic.CPUPaddle.x, Logic.CPUPaddle.y2, Logic.CPUPaddle.x2 };
	 Rect(cpuPaddle[0], cpuPaddle[1], cpuPaddle[2], cpuPaddle[3], CurrentShadeBrush);

	 float playerPaddle[4] = { PlayerPaddleY, Logic.PlayerPaddleX, PlayerPaddleY2, Logic.PlayerPaddleX2 };
	 Rect(playerPaddle[0], playerPaddle[1], playerPaddle[2], playerPaddle[3], CurrentShadeBrush);

	 float ball[4] = { Logic.Ball.y, Logic.Ball.x, Logic.Ball.y2, Logic.Ball.x2 };
	 Rect(ball[0], ball[1], ball[2], ball[3], CurrentShadeBrush);
}
void Graphics::BackgroundColorSwitch()
{
	switch (color) {
	case 0:
	{
		color = 1;
		break;
	}
	case 1:
	{
		CurrentShadeBrush = DimGray;
		CurrentColorBrush = Black;
		break;
	}
	case 2: {
		CurrentShadeBrush = White;
		CurrentColorBrush = Green;
		break;
	}
	case 3: {
		CurrentShadeBrush = White;
		CurrentColorBrush = Red;
		break;
	}
	case 4: {
		CurrentShadeBrush = White;
		CurrentColorBrush = Blue;
		break;
	}
	case 5: {
		CurrentShadeBrush = Black;
		CurrentColorBrush = White;
		break;
	}
	case 6: {
		CurrentShadeBrush = White;
		CurrentColorBrush = Purple;
		break;
	}
	case 7:
	{
		color = 6;
		break;
	}
	default: {
		color = 6;
		break;
	}
	}
}
void Graphics::NetRender()
{
	const int y1 = 400, y2 = 410;
	int x1 = 0, x2 = 10;
	for (int i = 0; i < 30; i++)
	{
		Rect(x1, y1, x2, y2, CurrentShadeBrush);
		x1 += 20, x2 += 20;
	}
}
void Graphics::ScoreRender()
{
	switch (Logic.PlayerScore) {
	case 0:
	{
		Rect(211, 36, 247, 119, CurrentShadeBrush);

		Rect(220, 45, 239, 110, CurrentColorBrush);
		break;
	}
	case 1:
	{
		Rect(234, 39, 244, 121, CurrentShadeBrush);

		break;
	}
	case 2:
	{
		Rect(212, 35, 245, 119, CurrentShadeBrush);

		Rect(210, 45, 236, 70, CurrentColorBrush);

		Rect(220, 80, 245, 110, CurrentColorBrush);

		break;

	}
	case 3:
	{
		Rect(210, 40, 245, 125, CurrentShadeBrush);

		Rect(210, 50, 233, 70, CurrentColorBrush);

		Rect(210, 82, 233, 114, CurrentColorBrush);

		break;
	}

	case 4:
	{
		Rect(210, 36, 245, 118, CurrentShadeBrush);

		Rect(220, 17, 234, 68, CurrentColorBrush);

		Rect(210, 82, 235, 118, CurrentColorBrush);
 
		break;
	}
	case 5:
	{
		Rect(210, 39, 244, 121, CurrentShadeBrush);

		Rect(219, 50, 245, 70, CurrentColorBrush);

		Rect(210, 80, 235, 111, CurrentColorBrush);

		break;
	}
	case 6:
	{
		Rect(210, 39, 245, 122, CurrentShadeBrush);

		Rect(219, 39, 250, 70, CurrentColorBrush);

		Rect(219, 79, 237, 111, CurrentColorBrush);

		break;
	}
	case 7:
	{
		Rect(210, 39, 245, 122, CurrentShadeBrush);

		Rect(210, 51, 235, 122, CurrentColorBrush);

		break;
	}
	case 8:
	{
		Rect(210, 39, 245, 122, CurrentShadeBrush);

		Rect(218, 48, 237, 72, CurrentColorBrush);

		Rect(218, 82, 237, 112, CurrentColorBrush);

		break;
	}
	case 9:
	{
		Rect(210, 39, 245, 122, CurrentShadeBrush);

		Rect(218, 48, 237, 72, CurrentColorBrush);

		Rect(210, 82, 237, 122, CurrentColorBrush);
 
		break;
	}
	case 10:
	{
		Rect(211, 36, 247, 119, CurrentShadeBrush);

		Rect(220, 45, 239, 110, CurrentColorBrush);

		Rect(169, 36, 179, 119, CurrentShadeBrush);
		break;
	}
	case 11:
	{
		Rect(169, 36, 179, 119, CurrentShadeBrush);

		Rect(225, 36, 235, 119, CurrentShadeBrush);
		break;
	}
	default:
	{
		Logic.PlayerScore = 0;
		break;
	}
	}
	switch (Logic.CPUScore) {
	case 0:
	{
		Rect(611, 36, 647, 119, CurrentShadeBrush);

		Rect(620, 45, 639, 110, CurrentColorBrush);

		break;
	}
	case 1:
	{
		Rect(634, 39, 644, 121, CurrentShadeBrush);

		break;
	}
	case 2:
	{
		Rect(612, 35, 645, 119, CurrentShadeBrush);

		Rect(610, 45, 636, 70, CurrentColorBrush);

		Rect(620, 80, 645, 110, CurrentColorBrush);

		break;

	}
	case 3:
	{
		Rect(610, 40, 645, 125, CurrentShadeBrush);

		Rect(610, 50, 633, 70, CurrentColorBrush);

		Rect(610, 82, 633, 114, CurrentColorBrush);

		break;
	}

	case 4:
	{
		Rect(610, 36, 645, 118, CurrentShadeBrush);

		Rect(620, 17, 634, 68, CurrentColorBrush);

		Rect(610, 82, 635, 118, CurrentColorBrush);
 
		break;
	}
	case 5:
	{
		Rect(610, 39, 644, 121, CurrentShadeBrush);

		Rect(619, 50, 645, 70, CurrentColorBrush);

		Rect(610, 80, 635, 111, CurrentColorBrush);

		break;
	}
	case 6:
	{
		Rect(610, 39, 645, 122, CurrentShadeBrush);

		Rect(619, 39, 650, 70, CurrentColorBrush);

		Rect(619, 79, 637, 111, CurrentColorBrush);

		break;
	}
	case 7:
	{
		Rect(610, 39, 645, 122, CurrentShadeBrush);

		Rect(610, 51, 635, 122, CurrentColorBrush);

		break;
	}
	case 8:
	{
		Rect(610, 39, 645, 122, CurrentShadeBrush);

		Rect(618, 48, 637, 72, CurrentColorBrush);

		Rect(618, 82, 637, 112, CurrentColorBrush);

		break;
	}
	case 9:
	{
		Rect(610, 39, 645, 122, CurrentShadeBrush);

		Rect(618, 48, 637, 72, CurrentColorBrush);

		Rect(610, 82, 637, 122, CurrentColorBrush);

		break;
	}
	case 10:
	{
		Rect(611, 36, 647, 119, CurrentShadeBrush);

		Rect(620, 45, 639, 110, CurrentColorBrush);

		Rect(569, 36, 579, 119, CurrentShadeBrush);
		break;
	}
	case 11:
	{
		Rect(569, 36, 579, 119, CurrentShadeBrush);

		Rect(625, 36, 635, 119, CurrentShadeBrush);
		break;
	}
	default: {
		Logic.CPUScore = 0;
		break;
	}
	}
}