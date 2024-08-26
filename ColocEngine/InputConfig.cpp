#include "InputConfig.h"
#include <utility>

namespace InputConfig
{
	int CRs[static_cast<uint16_t>(CR::Amount)];
	int KBs[static_cast<uint16_t>(KB::Amount)];
}

int16_t InputConfig::GetCode(CR key)
{
	return CRs[static_cast<int>(key)];
}

int16_t InputConfig::GetCode(KB key)
{
	return KBs[static_cast<int>(key)];
}

void InputConfig::SetConfig(KB k, int code)
{
	KBs[static_cast<int>(k)] = code;
}

void InputConfig::SetConfig(CR c, int code)
{
	CRs[static_cast<int>(c)] = code;
}
