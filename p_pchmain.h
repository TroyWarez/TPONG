#pragma once

#include "targetver.h"

#ifdef WINDOWS

#define WIN32_LEAN_AND_MEAN
// Windows Header Files:
#include <windows.h>
#include <WindowsX.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <Xinput.h>
#include <mmsystem.h>

extern HWND hWnd;

#endif // WINDOWS

#include <stdlib.h>
#include <time.h>
#include <stdio.h>