#pragma once

#include <EGL/egl.h>

class Window
{
private:
	struct TargetState
	{
		uint32_t width;
		uint32_t height;

		EGLDisplay display;
		EGLSurface surface;
		EGLContext context;

		EGL_DISPMANX_WINDOW_T nativewindow;
	};
	TargetState m_state;

public:
	void init(uint32_t width, uint32_t height);
	
	const TargetState &getTargetState() const;

};