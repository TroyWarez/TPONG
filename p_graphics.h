#pragma once
class Graphics
{
public:

	Graphics();
	~Graphics();
	bool IsGameActive, IsGamePaused, lastResult;

	float r, g, b;

	unsigned int color, pause;

	__int64 StartTime, CurrentTime, EndTime, Freq;
	float msec;


	void RenderGame();

	void PauseGame();

	void ScoreRender();

	bool InitGraphics();

private:
	GLfloat Black[3] = { 0.0f, 0.0f, 0.0f };
	GLfloat White[3] = { 1.0f, 1.0f, 1.0f };

	GLfloat Red[3] = { 0.5f, 0.0f, 0.0f };
	GLfloat Green[3] = { 0.0f, 0.5f, 0.0f };
	GLfloat Blue[3] = { 0.0f, 0.0f, 0.5f };

	GLfloat Purple[3] = { 0.5058823529411765f, 0.3058823529411765f, 0.8235294117647059f };
	GLfloat DimGray[3] = { 0.4627450980392157f, 0.4627450980392157f, 0.4627450980392157f };
	void BackgroundColorSwitch();

	void PauseChecker();

	void NetRender();
	bool CreateGLContext();

	void Rect(int x1, int y1, int x2, int y2, GLfloat* Color);
	void Pixel(GLint x, GLint y, GLfloat* Color);

	bool InitDeviceResources();
private:
	HDC hDeviceContext;
	int pixelFormat;
	HGLRC renderingContext;
	PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { 0 };
	GLfloat* CurrentShadeBrush;
	GLfloat* CurrentColorBrush;
};