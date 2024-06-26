#pragma once
#include <xaudio2.h>
#include<cstdint>
#include<mmsystem.h>
#include<mfapi.h>
#include<mfidl.h>
#include<mfreadwrite.h>
#include<vector>
#include<string>
#include"MACRO.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#include"FileLoader.h"

struct AudioData
{
	WAVEFORMATEX format_;
	std::vector<byte> pBuf_;
	std::wstring name_;

	void Release();

	~AudioData();
};

class Sounder
{
public:

public:
	void SetVolume(float v);
	float GetVolume();
	
	Sounder(IXAudio2SourceVoice* ad ,bool isSE_);

	~Sounder();

	IXAudio2SourceVoice* GetPointer();

private:
	IXAudio2SourceVoice* src_;
	float vol_;
	float3 pos_;
	bool isSE_;

};

namespace S_Sound
{
	constexpr uint8_t SE_Amount = 16u;
	constexpr uint8_t BGM_Amount = 4;

	bool Init();
	void Run();
	void Term();
	bool CreateSE(const AudioData* data,bool isLoop,Sounder** s);//sounder
	bool CreateBGM(const AudioData* data,Sounder** s);
	bool Starts(bool isSE ,bool isBGM);
	bool Stops(bool isSE, bool isBGM);
	bool Destroys(bool isSE,bool isBGM);

	bool StartSE(Sounder* ptr);
	bool StartBGM(Sounder* ptr);
	bool StopSE(Sounder* ptr);
	bool StopBGM(Sounder* ptr);
	bool DestroySE(Sounder* ptr);
	bool DestroyBGM(Sounder* ptr);

	size_t GetAudioFileData(std::wstring file ,IMFMediaType* t);

	bool LoadWave_wav(std::wstring str , AudioData* ad);

	void SetVol_BGM(float v);
	void SetVol_SE(float v);
	void SetVol_Master(float v);
};

