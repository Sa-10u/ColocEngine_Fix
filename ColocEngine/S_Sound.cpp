#include "S_Sound.h"
#include<list>

namespace S_Sound
{
	IXAudio2* audio_ = {};
	IXAudio2MasteringVoice* master_ = {};
	IXAudio2SourceVoice* SEs_[SE_Amount] = {};
	IXAudio2SourceVoice* BGMs_[BGM_Amount] = {};

	std::list<IXAudio2SourceVoice**> useSE = {};
	std::list<IXAudio2SourceVoice**> idleSE = {};
	std::list<IXAudio2SourceVoice**> standbySE = {};
	
	std::list<IXAudio2SourceVoice**> useBGM = {};
	std::list<IXAudio2SourceVoice**> idleBGM = {};
	std::list<IXAudio2SourceVoice**> standbyBGM = {};

	float BaseVol_SE = 1.0f;
	float BaseVol_BGM = 1.0f;
	float BaseVol_Master = 1.0f;

	IMFMediaType* type_ = {};

	bool Init()
	{
		auto&& res = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(res))	return false;

		res = MFStartup(MF_VERSION,0);
		if (FAILED(res))	return false;

		res = XAudio2Create(&audio_, 0);
		if (FAILED(res))	return false;

		res = audio_->CreateMasteringVoice(&master_);
		if (FAILED(res))	return false;

		MFCreateMediaType(&type_);
		type_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);

		for (auto i = 0u; i < SE_Amount; ++i) {
			idleSE.push_back(&SEs_[i]);
		}
		for (auto i = 0u; i < BGM_Amount; ++i) {
			idleBGM.push_back(&BGMs_[i]);
		}
	}

	void Run()
	{
	}

	void Term()
	{
		Stops(true,true);
		Destroys(true, true);

		master_->DestroyVoice();
		audio_->Release();

		MFShutdown();

		CoUninitialize();
	}

	bool CreateBGM(const AudioData* data, Sounder** s)
	{
		return false;
	}

	bool CreateSE(const AudioData* data ,bool isLoop, Sounder** s)
	{
		auto buf = idleSE.front();
		idleSE.pop_front();

		auto&& res = audio_->CreateSourceVoice
		(
			buf,
			&data->format_
		);
		if (FAILED(res))	return false;

		{
			XAUDIO2_BUFFER temp = {};
			temp.pAudioData = data->pBuf_.data();
			temp.Flags = XAUDIO2_END_OF_STREAM;
			temp.AudioBytes = data->pBuf_.size() * sizeof(data->pBuf_[0]);
			temp.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : false;
			
			res = (*buf)->SubmitSourceBuffer(&temp);
			if (FAILED(res))	return false;
		}
		standbySE.push_back(buf);
		
		if(s != nullptr)	*s = new Sounder(*buf, true);
		return true;

	}

	bool Starts(bool isSE,bool isBGM)//when stop or end,call back of self push to list
	{

		auto itr = []
		(auto& sli ,auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				(**itr)->Start();
				rli.push_back(*itr);

				itr = sli.erase(itr);
			}
		};

		if(isSE)	itr(standbySE, useSE );
		if(isBGM)	itr(standbyBGM, useBGM);
		
		return true;
	}

	bool Stops(bool isSE, bool isBGM)
	{
		auto itr = []
		(auto& sli, auto& rli)
			{
				for (auto itr = sli.begin(); itr != sli.end();) {

					(**itr)->Stop();
					rli.push_back(*itr);

					itr = sli.erase(itr);
				}
			};

		if (isSE)	itr(useSE, standbySE);
		if (isBGM)	itr(useBGM, standbyBGM);

		return true;
	}

	bool Destroys(bool isSE, bool isBGM)
	{
		auto itr = []
		(auto& sli, auto& rli)
			{
				for (auto itr = sli.begin(); itr != sli.end();) {

					(**itr)->DestroyVoice();
					rli.push_back(*itr);

					itr = sli.erase(itr);
				}
			};

		if (isSE)	itr(standbySE, idleSE);
		if (isBGM)	itr(standbyBGM, idleBGM);

		return true;
	}
	//--------------------

	auto l_start = [](IXAudio2SourceVoice* ptr,auto& sli,auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				if ((**itr) != ptr)	itr++; continue;


				(**itr)->Start();
				rli.push_back(*itr);
				sli.erase(itr);

				return true;
			}

			return false;
		};

	auto l_stop = [](IXAudio2SourceVoice* ptr, auto& sli, auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				if ((**itr) != ptr)	itr++; continue;


				(**itr)->Stop();
				rli.push_back(*itr);
				sli.erase(itr);

				return true;
			}

			return false;
		};

	auto l_destroy = [](IXAudio2SourceVoice* ptr, auto& sli, auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				if ((**itr) != ptr)	itr++; continue;

				(**itr)->DestroyVoice();
				rli.push_back(*itr);
				sli.erase(itr);

				return true;
			}

			return false;
		};

	bool StartSE(Sounder* ptr)
	{
		return l_start((ptr->GetPointer()), standbySE, useSE);
	}

	bool StartBGM(Sounder* ptr)
	{
		return  l_start((ptr->GetPointer()), standbyBGM, useBGM);
	}

	bool StopSE(Sounder* ptr)
	{
		return l_stop((ptr->GetPointer()), useSE, standbySE);
	}

	bool StopBGM(Sounder* ptr)
	{
		return l_stop((ptr->GetPointer()), useBGM, standbyBGM);
	}

	bool DestroySE(Sounder* ptr)
	{
		return l_destroy((ptr->GetPointer()), standbySE, idleSE);
	}

	bool DestroyBGM(Sounder* ptr)
	{
		return l_destroy((ptr->GetPointer()), standbyBGM, idleBGM);
	}

	size_t GetAudioFileData(std::wstring file, IMFMediaType* t)
	{
		size_t size = NULL;

		IMFSourceReader* rd = {};

		auto&& res = MFCreateSourceReaderFromURL(file.c_str(), nullptr, &rd);
		if (FAILED(res))	return NULL;

		res = rd->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, t);
		if (FAILED(res))	return NULL;

		while (true) {

			IMFSample* sample = nullptr;
			uint32_t flag = {};
			u_long cnt_buf = {};

			rd->ReadSample
			(
				MF_SOURCE_READER_FIRST_AUDIO_STREAM,
				NULL,
				nullptr,
				reinterpret_cast<unsigned long*>(&flag),
				nullptr,
				&sample
			);
			if (flag & MF_SOURCE_READERF_ENDOFSTREAM)	break;
			if (sample == nullptr || FAILED(sample->GetBufferCount(&cnt_buf)) || cnt_buf < 0)	return NULL;

			IMFMediaBuffer* buf_media = nullptr;
			res = sample->ConvertToContiguousBuffer(&buf_media);
			if (FAILED(res))return NULL;

			u_long s = NULL;

			u_char *empty = nullptr;

			res = buf_media->Lock(&empty, nullptr, &s);
			if (FAILED(res))	return NULL;

			size += s;
			
			buf_media->Unlock();
			buf_media->Release();
			sample->Release();
		}

		return size;
	}

	bool LoadWave_wav(std::wstring str, AudioData* ad)
	{
		std::wstring file = {};
		if (!isResourceFile(str.c_str(), &file))	return false;

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
		auto&& res = MFCreateSourceReaderFromURL(file.c_str(), nullptr, &srcreader_);
		if (FAILED(res))	return false;

		res = srcreader_->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, type_);
		if (FAILED(res))	return false;

		{
			IMFMediaType* defType = {};

			res = srcreader_->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &defType);
			if (FAILED(res))	return false;

			WAVEFORMATEX* temp_ = {};

			res = MFCreateWaveFormatExFromMFMediaType(
				defType,
				reinterpret_cast<WAVEFORMATEX**>(&temp_),
				nullptr,
				MFWaveFormatExConvertFlag_Normal
			);
			if (FAILED(res))	return false;

			ad->format_ = *temp_;
			CoTaskMemFree(temp_);

			defType->Release();
		}

		//auto changeVecLen = [](std::vector<unsigned long>& vec, int size)->int {vec.resize(size); return 0; };
		size_t s = GetAudioFileData(file.c_str(), type_);
		ad->pBuf_.resize(s);

		auto size = 0u;

		{
			unsigned long curLen = {};
			
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
				if (flag & MF_SOURCE_READERF_ENDOFSTREAM)	break;
				if (sample == nullptr || FAILED(sample->GetBufferCount(&cnt_buf)) || cnt_buf < 0)	return false;

				IMFMediaBuffer* buf_media = nullptr;
				res = sample->ConvertToContiguousBuffer(&buf_media);
				if (FAILED(res))return false;

				byte* lbuf = nullptr;

				res = buf_media->Lock(&lbuf,nullptr, &curLen);
				if (FAILED(res))	return false;

				memcpy(ad->pBuf_.data() + size , lbuf, curLen);
				size += curLen;

				buf_media->Unlock();
				buf_media->Release();
				sample->Release();

				//static int a = (0, changeVecLen(Data, maxLen));//unuse
			}

		}
		ad->name_ = str;
		return true;
	}

	void SetVol_BGM(float v)
	{
		BaseVol_BGM = v;
	}

	void SetVol_SE(float v)
	{
		BaseVol_SE = v;
	}

	void SetVol_Master(float v)
	{
		BaseVol_Master = v;
	}
	
	
}
//----------------------------------
void AudioData::Release()
{
	this->pBuf_.clear();
	this->name_.clear();
	this->format_ = {};
}

AudioData::~AudioData()
{
	//free(pBuf_);
	Release();
	pBuf_.clear();
}
//------------------------------------
void Sounder::SetVolume(float v)
{
	vol_ = v;
	(src_)->SetVolume(vol_ * S_Sound::BaseVol_Master * (isSE_ ? S_Sound::BaseVol_SE : S_Sound::BaseVol_BGM));
}

float Sounder::GetVolume()
{
	return vol_;
}


Sounder::Sounder(IXAudio2SourceVoice* ad,bool isSE) :pos_{}, isSE_(isSE), vol_(1.0f), src_(ad)
{
}

Sounder::~Sounder()
{
}

IXAudio2SourceVoice* Sounder::GetPointer()
{
	return src_;
}
