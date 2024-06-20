#pragma once
#include <xaudio2.h>
#include<cstdint>
#include<mmsystem.h>
#include<mfapi.h>
#include<mfidl.h>
#include<mfreadwrite.h>
#include<vector>
#include<string>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib,"mfplat.lib")

#include"FileLoader.h"

struct AudioData
{
	WAVEFORMATEXTENSIBLE format_;
	std::vector<byte> pBuf_;
	std::wstring name_;

	~AudioData();
};

namespace S_Sound
{
	constexpr uint8_t SE_Amount = 16u;
	constexpr uint8_t BGM_Amount = 4;

	bool Init();
	void Run();
	void Term();

	bool LoadWave_wav(std::wstring* str , AudioData* ad);
};

