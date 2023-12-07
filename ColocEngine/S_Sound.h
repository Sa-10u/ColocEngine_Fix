#pragma once
#include<dsound.h>

#pragma comment(lib,"dsound.lib")

namespace S_Sound
{
	bool Init();
	void Run();

	extern IDirectSound8* ds;
	extern IDirectSoundBuffer* buff;
	extern IDirectSoundBuffer8* buff8;
};

