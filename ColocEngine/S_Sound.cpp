#include "S_Sound.h"

namespace S_Sound
{
	IXAudio2* audio_ = {};
	IXAudio2MasteringVoice* master_ = {};
	IXAudio2SourceVoice* SEs_[SE_Amount] = {};
	IXAudio2SourceVoice* BGMs_[SE_Amount] = {};
	
	IMFMediaType* type_ = {};

	bool Init()
	{
		auto&& res = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(res))	return false;

		res = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
		if (FAILED(res))	return false;

		res = XAudio2Create(&audio_, 0);
		if (FAILED(res))	return false;

		return false;

		res = audio_->CreateMasteringVoice(&master_);
		if (FAILED(res))	return false;

		MFCreateMediaType(&type_);
		type_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	}

	void Run()
	{
	}

	void Term()
	{
		master_->DestroyVoice();
		audio_->Release();

		MFShutdown();

		CoUninitialize();
	}

	bool LoadWave_wav(std::wstring* str, AudioData* ad)
	{
		std::wstring file = {};
		if (!isResourceFile(str->c_str(), &file))	return false;

		if (ad == nullptr)	return false;
		delete[] ad->pBuf_;
		/*
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
	*/
		type_->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);

		IMFSourceReader* srcreader_ = {};
		auto&& res = MFCreateSourceReaderFromURL(file.c_str(), NULL, &srcreader_);
		if (FAILED(res))	return false;

		res = srcreader_->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, type_);
		if (FAILED(res))	return false;

		{
			IMFMediaType* defType = {};

			res = srcreader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &defType);
			if (FAILED(res))	return false;

			WAVEFORMATEXTENSIBLE* temp_ = {};

			res = MFCreateWaveFormatExFromMFMediaType(
				defType,
				reinterpret_cast<WAVEFORMATEX**>(&temp_),
				nullptr,
				MFWaveFormatExConvertFlag_ForceExtensible
			);
			if (FAILED(res))	return false;

			ad->format_ = *temp_;
			CoTaskMemFree(temp_);

			defType->Release();
		}

		while (true) {
			IMFSample* sample = nullptr;
			uint32_t flag = {};
			uint32_t cnt_buf = {};

			srcreader_->ReadSample
			(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				NULL,
				nullptr,
				reinterpret_cast<unsigned long*>(&flag),
				nullptr,
				&sample
			);
			if (flag & MF_SOURCE_READERF_ENDOFSTREAM)break;;

			IMFMediaBuffer* buf_media = nullptr;

			sample->ConvertToContiguousBuffer(&buf_media);

			u_char* buf = nullptr;
			uint32_t length = {};
			
			buf_media->Lock(&buf, nullptr, &length);
		}
	}	
}

AudioData::~AudioData()
{
	//free(pBuf_);
	delete[] pBuf_;
}
