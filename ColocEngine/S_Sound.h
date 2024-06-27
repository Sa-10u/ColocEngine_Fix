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

class Conductor
{
public:
	class Sounder
	{
	public:
		void SetVolume(float v);

		Sounder(IXAudio2SourceVoice* sv, bool isSE_,AudioData* ad);

		~Sounder();

		IXAudio2SourceVoice* GetPointer();
		bool isSE();
		AudioData* GetAudioData();

		void SetCallBack(IXAudio2VoiceCallback* cb);
		IXAudio2VoiceCallback* GetCallBack();

	private:
		IXAudio2SourceVoice* src_;
		bool isSE_;
		AudioData* mother_;
		IXAudio2VoiceCallback* cb_;

	};

	Sounder* GetSounder();
	void SetSounder(Sounder* ptr);
	void Release();

	Conductor();
	~Conductor();

	void SetVolume(float v);
	float GetVolume();

	void SetPos(float3 pos);
	float3 GetPos();

private:
	Sounder* sd_;
	float vol_;
	float3 pos_;

};

namespace S_Sound
{
	constexpr uint8_t SE_Amount = 16u;
	constexpr uint8_t BGM_Amount = 4;

	enum class FLAG
	{
		Loop = 0,
		AutoRelease,
		ManualRelease,
	};

	bool Init();
	void Run();
	void Term();
	bool CreateSE(const AudioData* data,FLAG flag,Conductor::Sounder** s);//sounder
	bool CreateBGM(const AudioData* data,Conductor::Sounder** s);
	bool Starts(bool isSE ,bool isBGM);
	bool Stops(bool isSE, bool isBGM);
	bool Destroys(bool isSE,bool isBGM);

	bool StartSE( Conductor::Sounder* ptr);
	bool StartBGM( Conductor::Sounder* ptr);
	bool StopSE( Conductor::Sounder* ptr);
	bool StopBGM( Conductor::Sounder* ptr);
	bool DestroySE( Conductor::Sounder* ptr);
	bool DestroyBGM( Conductor::Sounder* ptr);
	bool ReStartSE(Conductor* ptr);
	bool ReStartBGM(Conductor* ptr);

	size_t GetAudioFileData(std::wstring file ,IMFMediaType* t);

	bool LoadWave_wav(std::wstring str , AudioData* ad);

	void SetVol_BGM(float v);
	void SetVol_SE(float v);
	void SetVol_Master(float v);
};

