#include "InputConfig.h"
#include <utility>

namespace InputConfig
{
	uint8_t CRs[static_cast<uint16_t>(CR::Amount)];
	uint8_t KBs[static_cast<uint16_t>(KB::Amount)];
}

uint8_t InputConfig::GetCode(CR key)
{
	return CRs[static_cast<uint8_t>(key)];
}

uint8_t InputConfig::GetCode(KB key)
{
	return KBs[static_cast<uint8_t>(key)];
}

void InputConfig::SetConfig(KB k, uint8_t code)
{
	KBs[static_cast<uint8_t>(k)] = code;
}

void InputConfig::SetConfig(CR c, uint8_t code)
{
	CRs[static_cast<uint8_t>(c)] = code;
}