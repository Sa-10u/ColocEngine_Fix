#pragma once
#include<cstdint>

namespace InputConfig
{
	enum class KB
	{
		Jump = 0,


		Amount
	};

	enum class CR
	{
		Jump = 0,


		Amount
	};

	uint8_t GetCode(CR key);
	uint8_t GetCode(KB key);

	void SetConfig(KB k, uint8_t code);
	void SetConfig(CR c, uint8_t code);
};

