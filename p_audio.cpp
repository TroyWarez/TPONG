#include "p_pchmain.h"
#include "p_audio.h"
#include "p_resource.h"
extern HINSTANCE hInst;
Audio::Audio()
{
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE3), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
	//hResWAVE0 = hResWAVE1 = hResWAVE0 = nullptr;
	LoadSound(hResWAVE0, IDR_WAVE1);
	LoadSound(hResWAVE1, IDR_WAVE2);
	LoadSound(hResWAVE0, IDR_WAVE3);
}
Audio::~Audio()
{
	FreeResource(hResWAVE0);
	FreeResource(hResWAVE1);
	FreeResource(hResWAVE2);
}

BOOL Audio::LoadSound(HANDLE hRes, WORD iRESOURCE)//Messy
{
	BOOL bRtn;
	LPWSTR lpRes;
	HRSRC hResInfo;

	// Find the WAVE resource. 

	hResInfo = FindResource(hInst, MAKEINTRESOURCE(IDR_WAVE1), L"WAVE");
	if (hResInfo == NULL)
		return FALSE;

	// Load the WAVE resource. 

	hResWAVE0 = LoadResource(hInst, hResInfo);
	if (hResWAVE0 == NULL)
		return FALSE;

	hResInfo = FindResource(hInst, MAKEINTRESOURCE(IDR_WAVE2), L"WAVE");
	if (hResInfo == NULL)
		return FALSE;

	// Load the WAVE resource. 

	hResWAVE1 = LoadResource(hInst, hResInfo);
	if (hResWAVE1 == NULL)
		return FALSE;

	hResInfo = FindResource(hInst, MAKEINTRESOURCE(IDR_WAVE3), L"WAVE");
	if (hResInfo == NULL)
		return FALSE;

	// Load the WAVE resource. 

	hResWAVE2 = LoadResource(hInst, hResInfo);
	if (hResWAVE2 == NULL)
		return FALSE;

	return TRUE;
}
BOOL Audio::PlayResource(WORD Sound)
{
	BOOL bRtn;
	LPWSTR lpRes;
	HANDLE hResInfo, hRes;
	// Lock the WAVE resource and play it. 

	switch (Sound)
	{
	case 135:
	{
		hRes = hResWAVE0;
		break;
	}
	case 136:
	{
		hRes = hResWAVE1;
		break;
	}
	case 137:
	{
		hRes = hResWAVE2;
		break;
	}
	default:
	{
		return 1;
	}
	}

	lpRes = (LPWSTR)LockResource(hRes);
	if (lpRes != NULL) {
		bRtn = sndPlaySound(lpRes, SND_MEMORY | SND_ASYNC |
			SND_NODEFAULT);
		UnlockResource(hRes);
	}
	else
		bRtn = 0;

	// Free the WAVE resource and return success or failure. 
	if (!bRtn)
	{
		PlaySound(L"SystemExclamation", (HMODULE)1, 0);
	}
	return bRtn;
}