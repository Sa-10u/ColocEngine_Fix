#pragma once
#include"BUFFER.h"
#include"Resource.h"

namespace ModelManager
{
	std::vector<RModel> models_;
	std::vector<RTexture> textures_;
	std::vector<RCBuffer> CBs_;
	
	std::vector<string> modelNames_;//<-rmodel will take
	std::vector<string>texNames_;
};

