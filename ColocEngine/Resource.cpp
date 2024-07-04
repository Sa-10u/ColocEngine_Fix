#include "Resource.h"

RTexture::RTexture():is_using(false)
{
	Name_ = {};
	tex_ = {};
}

RModel::RModel() :DrawCount_(0)
{
}

RAudioData::RAudioData()
{
	Name_ = {};
	ad_ = {};
}
