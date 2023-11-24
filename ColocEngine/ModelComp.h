#pragma once
#include "IComp.h"
class RModel;

class ModelComp : public IComp
{
public:
	RModel* ptr;
	ModelComp(string tag);

private:
	std::vector<string> PS;
	std::vector<string> VS;
};

