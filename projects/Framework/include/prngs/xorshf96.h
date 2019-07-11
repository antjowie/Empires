#pragma once
#include "PRNG.h"

// Period 2^96-1
// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
class Xorshf96 : public PRNG
{
private:
	std::uint32_t m_x;
	std::uint32_t m_y;
	std::uint32_t m_z;

public:
	Xorshf96(std::uint32_t seed);
	virtual std::uint32_t random() override final;

};
