#include "ResourceManager.h"

namespace ResourceManager
{
	std::vector<RModel> models_;
	std::vector<RTexture> textures_;
};

void ResourceManager::Init()
{
	models_.clear();
	textures_.clear();
	
}

void ResourceManager::ModelLoad(string str)
{
}

void ResourceManager::TexLoad(string str)
{
}
