#pragma once

class Audio
{
public:
	Audio();
   ~Audio();

   HANDLE hResWAVE0;
   HANDLE hResWAVE1;
   HANDLE hResWAVE2;

   BOOL PlayResource(WORD Sound);
private:
	BOOL LoadSound(HANDLE hRes, WORD iRESOURCE);
};