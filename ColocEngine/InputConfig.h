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

	int16_t GetCode(CR key);
	int16_t GetCode(KB key);

	void SetConfig(KB k, int code);
	void SetConfig(CR c, int code);
};

