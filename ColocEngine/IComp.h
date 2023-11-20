#pragma once
#include <string>
#include <vector>
#include <stack>

using std::string;

class IComp;

struct Entity
{
	string Tag;
	void Update();

	std::vector<IComp> comps;


};


