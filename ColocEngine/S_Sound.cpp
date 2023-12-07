#include "S_Sound.h"
#include"D3D.h"

namespace S_Sound
{
	IDirectSound8* ds;
	IDirectSoundBuffer* buff;
	IDirectSoundBuffer8* buff8;

//--------------------


	bool Init()
	{
		HRESULT res = DirectSoundCreate8(nullptr, &ds, nullptr);
		if (FAILED(res))	return false;

		res =ds->SetCooperativeLevel(*PTR_WND::ptr, DSSCL_NORMAL);
		if (FAILED(res))	return false;

		DSBUFFERDESC desc = {};
		{
			
		}
		res = ds->CreateSoundBuffer(&desc, &buff, nullptr);
		if (FAILED(res))	return false;

		res = buff->QueryInterface(IID_IDirectSoundBuffer8, reinterpret_cast<void**>(&buff8));
		if (FAILED(res))	return false;

	}

	void Run()
	{
	}
}