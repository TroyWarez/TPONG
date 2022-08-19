#include "p_pchmain.h"
#include "p_Input.h"

void Input::Initialize(ID2D1Factory *pFactory)
{
	FLOAT dpiX, dpiY;
	pFactory->GetDesktopDpi(&dpiX, &dpiY);
	scaleX = dpiX / 96.0f;
	scaleY = dpiY / 96.0f;
}