#pragma once
#include "HUD.h"

class Cursor : public HUD
{
public:
	Cursor(const DrawData &drawData);

	virtual void drawTransparent(const Camera &camera) const override final;
};