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
	bool CreateSE(const AudioData* data,bool isLoop);
	bool CreateBGM(const AudioData* data);
	bool Starts(bool isSE ,bool isBGM);
	bool Stops(bool isSE, bool isBGM);
	bool Destroys(bool isSE,bool isBGM);

	bool StartSE(IXAudio2SourceVoice* ptr);
	bool StartBGM(IXAudio2SourceVoice* ptr);
	bool StopSE(IXAudio2SourceVoice* ptr);
	bool StopBGM(IXAudio2SourceVoice* ptr);
	bool DestroySE(IXAudio2SourceVoice* ptr);
	bool DestroyBGM(IXAudio2SourceVoice* ptr);


	bool LoadWave_wav(std::wstring* str , AudioData* ad);
};

