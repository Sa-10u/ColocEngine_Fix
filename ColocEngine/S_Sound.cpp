#include "S_Sound.h"
#include<queue>

namespace S_Sound
{
	IXAudio2* audio_ = {};
	IXAudio2MasteringVoice* master_ = {};
	IXAudio2SourceVoice* SEs_[SE_Amount] = {};
	IXAudio2SourceVoice* BGMs_[BGM_Amount] = {};

	std::queue<IXAudio2SourceVoice*> useSE = {};
	std::queue<IXAudio2SourceVoice*> idleSE = {};
	
	std::queue<IXAudio2SourceVoice*> useBGM = {};
	std::queue<IXAudio2SourceVoice*> idleBGM = {};

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

		for (auto i = 0u; i < SE_Amount; ++i) {
			idleSE.push(SEs_[i]);
		}
		for (auto i = 0u; i < BGM_Amount; ++i) {
			idleBGM.push(BGMs_[i]);
		}
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
		ad->pBuf_.clear();
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

		//auto changeVecLen = [](std::vector<unsigned long>& vec, int size)->int {vec.resize(size); return 0; };

		unsigned long pos = {};
		
		{
			{
				IMFSample* sample = nullptr;
				uint32_t flag = {};
				u_long cnt_buf = {};

				srcreader_->ReadSample
				(
					MF_SOURCE_READER_FIRST_AUDIO_STREAM,
					NULL,
					nullptr,
					reinterpret_cast<unsigned long*>(&flag),
					nullptr,
					&sample
				);
				if (flag & MF_SOURCE_READERF_ENDOFSTREAM)	return false;;
				if (sample == nullptr || FAILED(sample->GetBufferCount(&cnt_buf)) || cnt_buf <= 0)	return false;

				IMFMediaBuffer* buf_media = nullptr;
				res = sample->ConvertToContiguousBuffer(&buf_media);
				if (FAILED(res))return false;

				byte* lbuf = nullptr;
				unsigned long curLen = {};
				unsigned long curMax = {};

				res = buf_media->Lock(&lbuf, &curMax, &curLen);
				if (FAILED(res))	return false;
				buf_media->Unlock();
				buf_media->Release();
				sample->Release();

				ad->pBuf_.resize(curMax);
				memcpy(ad->pBuf_.data(), lbuf, curLen);

				pos += curLen;
			}
			while (true) {

				IMFSample* sample = nullptr;
				uint32_t flag = {};
				u_long cnt_buf = {};

				srcreader_->ReadSample
				(
					MF_SOURCE_READER_FIRST_AUDIO_STREAM,
					NULL,
					nullptr,
					reinterpret_cast<unsigned long*>(&flag),
					nullptr,
					&sample
				);
				if (flag & MF_SOURCE_READERF_ENDOFSTREAM)	break;;
				if (sample == nullptr || FAILED(sample->GetBufferCount(&cnt_buf)) || cnt_buf <= 0)	return false;

				IMFMediaBuffer* buf_media = nullptr;
				res = sample->ConvertToContiguousBuffer(&buf_media);
				if (FAILED(res))return false;

				byte* lbuf = nullptr;
				unsigned long curLen = {};

				res = buf_media->Lock(&lbuf, nullptr, &curLen);
				if (FAILED(res))	return false;
				buf_media->Unlock();
				buf_media->Release();
				sample->Release();

				memcpy(ad->pBuf_.data() + pos, lbuf, curLen);
				pos += curLen;

				//static int a = (0, changeVecLen(Data, maxLen));//unuse
			}
		}

		ad->name_ = *str;
	}
	

}

AudioData::~AudioData()
{
	//free(pBuf_);
	pBuf_.clear();
}
