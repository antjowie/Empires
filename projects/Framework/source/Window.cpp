#include "Window.h"

#include <bcm_host.h>
#include <cassert>
#include <GLES2/gl2.h>

static const EGLint attribute_list[] =
{
	EGL_RED_SIZE,
	8,
	EGL_GREEN_SIZE,
	8,
	EGL_BLUE_SIZE,
	8,
	EGL_ALPHA_SIZE,
	8,
	//EGL_SAMPLE_BUFFERS,1, // of we want AA, we need to uncomment these, there's a small time penalty but nicer graphics
	//EGL_SAMPLES,4, 
	EGL_DEPTH_SIZE,
	5,
	EGL_STENCIL_SIZE,
	8,
	EGL_SURFACE_TYPE,
	EGL_WINDOW_BIT,
	EGL_NONE
};

static const EGLint context_attributes[] =
{
	EGL_CONTEXT_CLIENT_VERSION,
	2,
	EGL_NONE
};

void Window::init(uint32_t width, uint32_t height)
{
	EGLBoolean result;
	EGLint num_config;

	bcm_host_init(); //RPI needs this

	//RPI setup is a little different to normal EGL
	DISPMANX_ELEMENT_HANDLE_T DispmanElementH;
	DISPMANX_DISPLAY_HANDLE_T DispmanDisplayH;
	DISPMANX_UPDATE_HANDLE_T DispmanUpdateH;
	VC_RECT_T dest_rect;
	VC_RECT_T src_rect;
	EGLConfig config;

	// get an EGL display connection
	m_state.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

	// initialize the EGL display connection
	result = eglInitialize(m_state.display, NULL, NULL);
	
	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(m_state.display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);

	// get an appropriate EGL frame buffer configuration
	result = eglBindAPI(EGL_OPENGL_ES_API);
	assert(EGL_FALSE != result);

	// create an EGL rendering context
	m_state.context = eglCreateContext(m_state.display, config, EGL_NO_CONTEXT, context_attributes);
	assert(m_state.context != EGL_NO_CONTEXT);

	// create an EGL window surface

	m_state.width = width;
	m_state.height = height;

	dest_rect.x = 0;
	dest_rect.y = 0;
	dest_rect.width = m_state.width; // it needs to know our window size
	dest_rect.height = m_state.height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = (width) << 16;
	src_rect.height = (height) << 16;

	DispmanDisplayH = vc_dispmanx_display_open(0);
	DispmanUpdateH = vc_dispmanx_update_start(0);

	DispmanElementH = vc_dispmanx_element_add(
		DispmanUpdateH,
		DispmanDisplayH,
		0/*layer*/,
		&dest_rect,
		0/*source*/,
		&src_rect,
		DISPMANX_PROTECTION_NONE,
		0 /*alpha value*/,
		0/*clamp*/,
		(DISPMANX_TRANSFORM_T)0/*transform*/);

	m_state.nativewindow.element = DispmanElementH;
	m_state.nativewindow.width = m_state.width;
	m_state.nativewindow.height = m_state.height;
	vc_dispmanx_update_submit_sync(DispmanUpdateH);
	
	m_state.surface = eglCreateWindowSurface(m_state.display, config, &(m_state.nativewindow), NULL);
	assert(m_state.surface != EGL_NO_SURFACE);

	// connect the context to the surface
	result = eglMakeCurrent(m_state.display, m_state.surface, m_state.surface, m_state.context);
	assert(EGL_FALSE != result);

	// Some OpenGLES2.0 states that we might need
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	
	glEnable(GL_CULL_FACE);

	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0x00);

	// Setup the viewport
	glViewport(0, 0, width, height);
}

const Window::TargetState & Window::getTargetState() const
{
	return m_state;
}
