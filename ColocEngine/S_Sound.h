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

namespace S_Sound
{
	enum class FLAG
	{
		Loop = 0,
		AutoRelease,
		ManualRelease,
	};

	class CallBack_Voice : public IXAudio2VoiceCallback
	{
	public:
		bool isEnd;
		CallBack_Voice();
	};
}

class Conductor
{
public:
	class Sounder
	{
	public:
		void SetVolume(float v);

		Sounder(IXAudio2SourceVoice** ppsv, bool isSE_,AudioData* ad , S_Sound::CallBack_Voice* cb);

		~Sounder();

		IXAudio2SourceVoice* GetPointer();
		bool isSE();
		AudioData* GetAudioData();

	public :

		enum class PLACE
		{
			useSE,
			standbySE,
			idleSE,
			useBGM,
			standbyBGM,
			idleBGM
		};

		bool ShifTo(PLACE p);

	private:
		IXAudio2SourceVoice** psrc_;
		bool isSE_;
		AudioData* mother_;
		S_Sound::CallBack_Voice* cb_;
	};

	Sounder* GetSounder();
	Sounder** GetPPS();
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

	bool Init();
	void Run();
	void Term();
	bool CreateSE(AudioData* data,FLAG flag,Conductor::Sounder** s);//sounder
	bool CreateBGM(AudioData* data,Conductor::Sounder** s);
	bool Starts(bool isSE ,bool isBGM);
	bool Stops(bool isSE, bool isBGM);
	bool Destroys(bool isSE,bool isBGM);

	bool StartSE( Conductor::Sounder* ptr);
	bool StartBGM( Conductor::Sounder* ptr);
	bool StopSE( Conductor::Sounder* ptr);
	bool StopBGM( Conductor::Sounder* ptr);
	bool DestroySE( Conductor::Sounder* ptr);
	bool DestroyBGM( Conductor::Sounder* ptr);
	bool ReSetSE(Conductor* ptr);
	bool ReSetBGM(Conductor* ptr);

	size_t GetAudioFileData(std::wstring file ,IMFMediaType* t);

	bool LoadWave_wav(std::wstring str , AudioData* ad);

	void SetVol_BGM(float v);
	void SetVol_SE(float v);
	void SetVol_Master(float v);
};

