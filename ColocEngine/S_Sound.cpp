#include "S_Sound.h"
#include<list>
#include<queue>

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

	class CB_Release :public CallBack_Voice
	{
	private:

		Conductor::Sounder** ptr_;

	public:
		void OnStreamEnd() override
		{
			delete *ptr_;
		}

		CB_Release(Conductor::Sounder** s) :ptr_(s),CallBack_Voice() {}
		~CB_Release() {}

		void OnVoiceProcessingPassEnd() override
		{
		}

		void OnVoiceProcessingPassStart(UINT32 SamplesRequired) override {    }
		void OnBufferEnd(void* pBufferContext) override { }
		void OnBufferStart(void* pBufferContext) override {    }
		void OnLoopEnd(void* pBufferContext) override {    }
		void OnVoiceError(void* pBufferContext, HRESULT Error) override { }
	};

	class CB_Reset :public CallBack_Voice
	{
	private:

		Conductor::Sounder** ptr_;

	public:
		void OnStreamEnd() override
		{
			auto sv = (*ptr_)->GetPointer();
			auto data = (*ptr_)->GetAudioData();
			
			sv->FlushSourceBuffers();
			sv->Stop();
			{
				XAUDIO2_BUFFER temp = {};
				temp.pAudioData = data->pBuf_.data();
				temp.Flags = XAUDIO2_END_OF_STREAM;
				temp.AudioBytes = data->pBuf_.size() * sizeof(data->pBuf_[0]);
				temp.LoopCount = false;

				auto res = (sv)->SubmitSourceBuffer(&temp);
				if (FAILED(res))	return;
			}

			(*ptr_)->ShifTo(Conductor::Sounder::PLACE::standbySE);
			
		}

		CB_Reset(Conductor::Sounder** s) :ptr_(s),CallBack_Voice(){}
		~CB_Reset() {}

		void OnVoiceProcessingPassEnd() override 
		{
		}

		void OnVoiceProcessingPassStart(UINT32 SamplesRequired) override {    }
		void OnBufferEnd(void* pBufferContext) override { }
		void OnBufferStart(void* pBufferContext) override {    }
		void OnLoopEnd(void* pBufferContext) override {    }
		void OnVoiceError(void* pBufferContext, HRESULT Error) override { }
	};

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

	void Flush()
	{
		for (auto i = 0; i < SE_Amount; ++i) {
			if (!SEs_[i])continue;	SEs_[i]->Stop();	SEs_[i]->DestroyVoice();
		}
		for (auto i = 0; i < BGM_Amount; ++i) {
			if (!BGMs_[i])continue;	BGMs_[i]->Stop();	BGMs_[i]->DestroyVoice();
		}
		
		idleSE.clear(); useSE.clear(); standbySE.clear();
		idleBGM.clear(); useBGM.clear(); standbyBGM.clear();

		for (auto i = 0u; i < SE_Amount; ++i) {
			idleSE.push_back(&SEs_[i]);
		}
		for (auto i = 0u; i < BGM_Amount; ++i) {
			idleBGM.push_back(&BGMs_[i]);
		}
	}

	void Term()
	{
		Flush();

		master_->DestroyVoice();
		audio_->Release();
		MFShutdown();

		CoUninitialize();
	}

	bool CreateBGM(AudioData* data, Conductor* cd)
	{
		if (!idleBGM.size())	return false;
		
		auto buf = idleBGM.front();
		idleBGM.pop_front();

		S_Sound::CallBack_Voice* cb = nullptr;

		auto res = audio_->CreateSourceVoice
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
			temp.LoopCount = XAUDIO2_LOOP_INFINITE;

			res = (*buf)->SubmitSourceBuffer(&temp);
			if (FAILED(res))	return false;
		}
		standbySE.push_back(buf);

		cd->SetSounder(new Conductor::Sounder(buf, false, data));
		return true;
	}

	bool CreateSE(AudioData* data , FLAG flag, Conductor* cd)
	{
		if (!idleSE.size())	return false;

		auto buf = idleSE.front();
		idleSE.pop_front();

		HRESULT res = {};
		switch (flag)
		{
		case FLAG::AutoRelease:

			cd->SetCallBack(new CB_Release(cd->GetPPS()));

			res = audio_->CreateSourceVoice
			(
				buf,
				&data->format_,
				0,
				XAUDIO2_DEFAULT_FREQ_RATIO,
				cd->GetCallBack(),
				nullptr,
				nullptr
			);
			break;

		case FLAG::ManualRelease:

			cd->SetCallBack(new CB_Reset(cd->GetPPS()));

			res = audio_->CreateSourceVoice
			(
				buf,
				&data->format_,
				0,
				XAUDIO2_DEFAULT_FREQ_RATIO,
				cd->GetCallBack(),
				nullptr,
				nullptr
			);
			break;

		default:
			res = audio_->CreateSourceVoice
			(
				buf,
				&data->format_
			);
			break;
		}
		if (FAILED(res))	return false;

		{
			XAUDIO2_BUFFER temp = {};
			temp.pAudioData = data->pBuf_.data();
			temp.Flags = XAUDIO2_END_OF_STREAM;
			temp.AudioBytes = data->pBuf_.size() * sizeof(data->pBuf_[0]);
			temp.LoopCount = flag == FLAG::Loop ? XAUDIO2_LOOP_INFINITE : false;
			
			res = (*buf)->SubmitSourceBuffer(&temp);
			if (FAILED(res))	return false;
		}
		standbySE.push_back(buf);

		cd->SetSounder(new Conductor::Sounder(buf, true, data));
		return true;
	}

	bool Starts(bool isSE,bool isBGM)//when stop or end,call back of self push to list
	{

		auto itr = []
		(auto& sli ,auto& rli)
		{
			for (auto&& itr = sli.begin(); itr != sli.end();) {

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
				for (auto&& itr = sli.begin(); itr != sli.end();) {

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
				for (auto&& itr = sli.begin(); itr != sli.end();) {

					(**itr)->Stop();
					(**itr)->FlushSourceBuffers();
					(**itr)->DestroyVoice();
					rli.push_back(*itr);

					itr = sli.erase(itr);
				}
			};

		if (isSE) { itr(standbySE, idleSE); itr(useSE, idleSE);}
		if (isBGM) { itr(standbyBGM, idleBGM); itr(useBGM, idleBGM); }

		return true;
	}
	//--------------------

	auto l_start = [](IXAudio2SourceVoice* ptr,auto& sli,auto& rli)
		{
			for (auto&& itr = sli.begin(); itr != sli.end();) {

				if ((**itr) != ptr) { itr++; continue; }

				(**itr)->Start();

				auto temp = *itr;
				sli.erase(itr);
				rli.push_back(temp);

				return true;
			}

			return false;
		};

	auto l_stop = [](IXAudio2SourceVoice* ptr, auto& sli, auto& rli)
		{
			for (auto&& itr = sli.begin(); itr != sli.end();) {

				if ((**itr) != ptr) { itr++; continue; }

				(**itr)->Stop();

				auto temp = *itr;
				sli.erase(itr);
				rli.push_back(temp);


				return true;
			}

			return false;
		};

	auto l_destroy = [](IXAudio2SourceVoice* ptr, auto& sli, auto& rli)
		{
			for (auto&& itr = sli.begin(); itr != sli.end();) {

				if ((**itr) != ptr) { itr++; continue; }

				(**itr)->FlushSourceBuffers();
				(**itr)->DestroyVoice();
				
				auto temp = *itr;
				sli.erase(itr);
				rli.push_back(temp);


				return true;
			}

			return false;
		};

	auto l_restart = [](Conductor* ptr, IXAudio2SourceVoice** arr , uint16_t size, Conductor::Sounder::PLACE p)
		{
			for (auto&& i = 0u; i < size;i++) {

				if ((arr[i]) != ptr->GetSounder()->GetPointer()) continue;

				auto&& sv = ptr->GetSounder()->GetPointer();
				auto&& data = ptr->GetSounder()->GetAudioData();
				sv->Stop();
				sv->FlushSourceBuffers();
				{
					XAUDIO2_BUFFER temp = {};
					temp.pAudioData = data->pBuf_.data();
					temp.Flags = XAUDIO2_END_OF_STREAM;
					temp.AudioBytes = data->pBuf_.size() * sizeof(data->pBuf_[0]);
					temp.LoopCount = false;

					auto res = (sv)->SubmitSourceBuffer(&temp);
					if (FAILED(res))	return false;
				}

				ptr->GetSounder()->ShifTo(p);

				return true;
			}

			return false;
		};

	bool StartSE( Conductor::Sounder* ptr)
	{
		return l_start((ptr->GetPointer()), standbySE, useSE);
	}

	bool StartBGM( Conductor::Sounder* ptr)
	{
		return  l_start((ptr->GetPointer()), standbyBGM, useBGM);
	}

	bool StopSE( Conductor::Sounder* ptr)
	{
		return l_stop((ptr->GetPointer()), useSE, standbySE);
	}

	bool StopBGM( Conductor::Sounder* ptr)
	{
		return l_stop((ptr->GetPointer()), useBGM, standbyBGM);
	}

	bool DestroySE( Conductor::Sounder* ptr)
	{
		return l_destroy((ptr->GetPointer()), standbySE, idleSE);
	}

	bool DestroyBGM( Conductor::Sounder* ptr)
	{
		return l_destroy((ptr->GetPointer()), standbyBGM, idleBGM);
	}

	bool ReSetSE(Conductor* ptr)
	{
		return l_restart((ptr), SEs_, SE_Amount, Conductor::Sounder::PLACE::standbySE);
	}

	bool ReSetBGM(Conductor* ptr)
	{
		return l_restart((ptr), BGMs_, BGM_Amount, Conductor::Sounder::PLACE::standbyBGM);
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

	bool LoadWave(std::wstring str, AudioData* ad)
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
	
	
	CallBack_Voice::CallBack_Voice(){}
	CallBack_Voice::~CallBack_Voice(){}

}
//----------------------------------
void AudioData::Release()
{
	this->pBuf_.clear();
	this->format_ = {};
}

AudioData::AudioData()
{
	format_ = {};
	pBuf_ = {};
}

AudioData::~AudioData()
{
	//free(pBuf_);
	Release();
	pBuf_.clear();
}
//------------------------------------
void Conductor::Sounder::SetVolume(float v)
{
	(*psrc_)->SetVolume(v * S_Sound::BaseVol_Master * (isSE_ ? S_Sound::BaseVol_SE : S_Sound::BaseVol_BGM));
}

Conductor::Sounder::Sounder(IXAudio2SourceVoice** ppsv, bool isSE, AudioData* ad):isSE_(isSE), psrc_(ppsv), mother_(ad)
{
}

 Conductor::Sounder::~Sounder()
{
	 if (*psrc_)
	 {
		 (*psrc_)->Stop();
		 (*psrc_)->FlushSourceBuffers();
		 (*psrc_)->DestroyVoice();
		 (*psrc_) = nullptr;
	 }

	 mother_ = nullptr;
}

IXAudio2SourceVoice* Conductor::Sounder::GetPointer()
{
	return *psrc_;
}

bool Conductor::Sounder::isSE()
{
	return isSE_;
}

AudioData* Conductor::Sounder::GetAudioData()
{
	return mother_;
}

Conductor::Sounder* Conductor::GetSounder()
{
	return sd_;
}

S_Sound::CallBack_Voice* Conductor::GetCallBack()
{
	return cb_;
}

void Conductor::SetCallBack(S_Sound::CallBack_Voice* cb)
{
	delete cb_;
	cb_ = cb;
}

Conductor::Sounder** Conductor::GetPPS()
{
	return &sd_;
}

void Conductor::SetSounder(Sounder* ptr)
{
	delete sd_;
	sd_ = ptr;
}

void Conductor::Release()
{
	delete sd_;
	sd_ = nullptr;

	delete cb_;
	cb_ = nullptr;
}

Conductor::Conductor() :sd_(nullptr),pos_{},vol_(1.0f)
{
}

Conductor::~Conductor()
{
	Release();
}

void Conductor::SetVolume(float v)
{
	vol_ = v;
	this->sd_->SetVolume(vol_);
}

float Conductor::GetVolume()
{
	return vol_;
}

void Conductor::SetPos(float3 pos)
{
	pos_ = pos;
}

float3 Conductor::GetPos()
{
	return pos_;
}

bool Conductor::Sounder::ShifTo(PLACE p)
{
	using namespace S_Sound;

	std::list<IXAudio2SourceVoice**>* lists[] = {&useSE, &standbySE, &idleSE, &useBGM, &standbyBGM, &idleBGM};

	auto tgt = lists[static_cast<uint8_t>(p)];

	for (auto i = 0u; i < _countof(lists); i++) {

		for (auto&& itr = lists[i]->begin(); itr != lists[i]->end();++itr) {

			if (*itr != psrc_)	continue;

			auto temp = *itr;

			lists[i]->erase(itr);
			tgt->push_back(temp);

			return true;
		}
	}

	return false;
}

