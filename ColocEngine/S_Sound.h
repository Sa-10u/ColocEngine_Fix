#pragma once
#include <xaudio2.h>
#include<cstdint>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")

#include"FileLoader.h"

struct WaveData
{
	WAVEFORMATEX format_;
	void* pBuf_;
	uint32_t size_;

	~WaveData();
};

namespace S_Sound
{
	constexpr uint8_t SE_Amount = 16u;
	constexpr uint8_t BGM_Amount = 4;

	bool Init();
	void Run();
	void Term();

	bool LoadWave_wav(std::wstring* str , WaveData* wd);
};

