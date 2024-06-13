#include "S_Sound.h"

namespace S_Sound
{
	IXAudio2* audio_ = {};
	IXAudio2MasteringVoice* master_ = {};
	IXAudio2SourceVoice* SEs_[SE_Amount] = {};
	IXAudio2SourceVoice* BGMs_[SE_Amount] = {};

	bool Init()
	{
		auto&& res = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(res))	return false;

		res = XAudio2Create(&audio_, 0);
		if (FAILED(res))	return false;

		return false;

		res = audio_->CreateMasteringVoice(&master_);
		if (FAILED(res))	return false;
	}

	void Run()
	{
	}

	void Term()
	{
		master_->DestroyVoice();
		audio_->Release();

		CoUninitialize();
	}

	bool LoadWave_wav(std::wstring* str, WaveData* wd)
	{
		std::wstring file = {};
		if (!isResourceFile(str->c_str(), &file))	return false;

		if (wd == nullptr)	return false;
		delete[] wd->pBuf_;

		HMMIO h_mmio = {};
		MMCKINFO info_mmchunk = {};
		MMCKINFO riff_chunk;

		h_mmio = mmioOpen
		(
			file.data(),
			nullptr,
			MMIO_READ
		);
		if (h_mmio == nullptr)	return false;

		riff_chunk.fccType = mmioFOURCC('W','A','V','E');
	}
}

WaveData::~WaveData()
{
	//free(pBuf_);
	delete[] pBuf_;
}
