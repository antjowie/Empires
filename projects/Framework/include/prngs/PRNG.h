#pragma once
#include <cstdint>

class PRNG
{
public:
	virtual std::uint32_t random() = 0;
};
