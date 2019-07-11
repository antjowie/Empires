#include "prngs/xorshf96.h"

Xorshf96::Xorshf96(std::uint32_t seed)
{
	// Take some bits from the seed and divide it three
	constexpr std::uint32_t xMask = 0xFF00FF00;
	constexpr std::uint32_t yMask = 0xF0F0F0F0;
	constexpr std::uint32_t zMask = 0x00FF00FF;

	m_x = seed ^ xMask;
	m_y = seed ^ yMask;
	m_z = seed ^ zMask;
}

std::uint32_t Xorshf96::random()
{
	std::uint32_t t;
	m_x ^= m_x << 16;
	m_x ^= m_x >> 5;
	m_x ^= m_x << 1;

	t = m_x;
	m_x = m_y;
	m_y = m_z;
	m_z = t ^ m_x ^ m_y;

	return m_z;
}