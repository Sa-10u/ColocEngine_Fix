#include "S_Sound.h"
#include<list>

namespace S_Sound
{
	IXAudio2* audio_ = {};
	IXAudio2MasteringVoice* master_ = {};
	IXAudio2SourceVoice* SEs_[SE_Amount] = {};
	IXAudio2SourceVoice* BGMs_[BGM_Amount] = {};

	std::list<IXAudio2SourceVoice*> useSE = {};
	std::list<IXAudio2SourceVoice*> idleSE = {};
	std::list<IXAudio2SourceVoice*> standbySE = {};
	
	std::list<IXAudio2SourceVoice*> useBGM = {};
	std::list<IXAudio2SourceVoice*> idleBGM = {};
	std::list<IXAudio2SourceVoice*> standbyBGM = {};

	IMFMediaType* type_ = {};

	bool Init()
	{
		auto&& res = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(res))	return false;

		res = MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);
		if (FAILED(res))	return false;

		res = XAudio2Create(&audio_, 0);
		if (FAILED(res))	return false;

		res = audio_->CreateMasteringVoice(&master_);
		if (FAILED(res))	return false;

		MFCreateMediaType(&type_);
		type_->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);

		for (auto i = 0u; i < SE_Amount; ++i) {
			idleSE.push_back(SEs_[i]);
		}
		for (auto i = 0u; i < BGM_Amount; ++i) {
			idleBGM.push_back(BGMs_[i]);
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

	bool CreateBGM(const AudioData* data)
	{
		return false;
	}

	bool CreateSE(const AudioData* data ,bool isLoop)
	{
		auto buf = idleSE.front();
		idleSE.pop_front();

		auto fmt = data->format_.Format;
		fmt.wBitsPerSample = data->format_.Format.nBlockAlign * 8 / data->format_.Format.nChannels;

		auto&& res = audio_->CreateSourceVoice
		(
			&buf,
			reinterpret_cast<WAVEFORMATEX*>(&fmt)
		);
		if (FAILED(res))	return false;

		{
			XAUDIO2_BUFFER temp = {};
			temp.pAudioData = data->pBuf_.data();
			temp.Flags = XAUDIO2_END_OF_STREAM;
			temp.AudioBytes = data->pBuf_.size() * sizeof(data->pBuf_[0]);
			temp.LoopCount = isLoop ? XAUDIO2_LOOP_INFINITE : false;

			res = buf->SubmitSourceBuffer(&temp);
			if (FAILED(res))	return false;
		}
		standbySE.push_back(buf);
		
		return true;
	}

	bool Starts(bool isSE,bool isBGM)//when stop or end,call back of self push to list
	{

		auto itr = []
		(auto& sli ,auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				(*itr)->Start();
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

					(*itr)->Stop();
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

					(*itr)->DestroyVoice();
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

				if ((*itr) != ptr)	itr++; continue;


				(*itr)->Start();
				rli.push_back(*itr);
				sli.erase(itr);

				return true;
			}

			return false;
		};

	auto l_stop = [](IXAudio2SourceVoice* ptr, auto& sli, auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				if ((*itr) != ptr)	itr++; continue;


				(*itr)->Stop();
				rli.push_back(*itr);
				sli.erase(itr);

				return true;
			}

			return false;
		};

	auto l_destroy = [](IXAudio2SourceVoice* ptr, auto& sli, auto& rli)
		{
			for (auto itr = sli.begin(); itr != sli.end();) {

				if ((*itr) != ptr)	itr++; continue;

				(*itr)->DestroyVoice();
				rli.push_back(*itr);
				sli.erase(itr);

				return true;
			}

			return false;
		};

	bool StartSE(IXAudio2SourceVoice* ptr)
	{
		return l_start(ptr, standbySE, useSE);
	}

	bool StartBGM(IXAudio2SourceVoice* ptr)
	{
		return  l_start(ptr, standbyBGM, useBGM);
	}

	bool StopSE(IXAudio2SourceVoice* ptr)
	{
		return l_stop(ptr, useSE, standbySE);
	}

	bool StopBGM(IXAudio2SourceVoice* ptr)
	{
		return l_stop(ptr, useBGM, standbyBGM);
	}

	bool DestroySE(IXAudio2SourceVoice* ptr)
	{
		return l_destroy(ptr, standbySE, idleSE);
	}

	bool DestroyBGM(IXAudio2SourceVoice* ptr)
	{
		return l_destroy(ptr, standbyBGM, idleBGM);
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

				memcpy(ad->pBuf_.data() , lbuf, curLen);
				pos += curLen;

				break;

				//static int a = (0, changeVecLen(Data, maxLen));//unuse
			}
		}

		ad->name_ = str;
	}
	

}

AudioData::~AudioData()
{
	//free(pBuf_);
	pBuf_.clear();
}
