#include "Game.h"

#include <sys/time.h>
#include <GLES2/gl2.h>
#include <cstdio>

bool Game::init(unsigned width, unsigned height, Scene* initialScene, ObjectsToLoad objectsToLoad)
{
	// Load window
	m_window.init(width,height);

	// Load DrawFactory
	if(objectsToLoad)
		objectsToLoad(m_factory);

	// Load input
	if (m_input.Init(width, height))
		printf("Input threads started up successfully\n");
	else
		printf("ERROR: Input threads failes to start up\n");

	// Load initial scene
	m_scenes.pushInitialScene(initialScene);
	initialScene->onCreate(m_factory);
	
	return true;
}

void Game::run()
{
	// we're going to keep track of time so need some structures to hold the time in linux's strange format
	struct timeval t1, t2;
	struct timezone tz;
	float deltatime;
	float totaltime = 0.0f;
	unsigned int frames = 0;

	gettimeofday(&t1, &tz);
	while (/*totaltime < 60.f && */!m_scenes.empty())
	{
		// Clear buffer and get new elapsed time
		gettimeofday(&t2, &tz);
		//deltatime = (float)((float)t2.tv_sec - (float)t1.tv_sec + ((float)t2.tv_usec - (float)t1.tv_usec) * 0.0000001f);
		deltatime = float(t2.tv_sec - t1.tv_sec) + (float(t2.tv_usec - t1.tv_usec) * 1.e-6f);
		totaltime += deltatime;
		t1 = t2;

		if (deltatime > 2.f)
			deltatime = 1.f / 60.f;

		glStencilMask(0xFF);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(0x00);

		// This is where the game logic is
		m_input.Update();
		m_scenes.run(m_factory, m_input, deltatime);

		// After our draw we need to swap buffers to display on screen
		eglSwapBuffers(m_window.getTargetState().display, m_window.getTargetState().surface);

		if (totaltime > 1.0f)
		{
			printf("%4i frames rendered in %1.4f seconds -> FPS=%3.4f\n", frames, totaltime, (float)frames / totaltime);
			totaltime -= 1.0f;
			frames = 0;
		}
		frames++; 
	}

	// This never seems to work for some reason so I'll just leave it
	//m_input.Terminate();
}