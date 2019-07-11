
#pragma once
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <pthread.h>
#include <linux/input.h>

#undef countof
#include <glm/glm/vec2.hpp>

//*******************************************************************************************
//Mega thanks to Grumpy old Git Developer Gareth Lewis for his help in putting this together
//*******************************************************************************************

// Keys[] sets the relevent index to TRUE when pressed and FALSE when released, index names can be found in <linux/input.h> and follow the usual KEY_A...KEY_Z format
// mouse keeps track of the motion of the mouse not the absolute position, which needs to be known beforehand if you plan to use the mouse for direct control. 
// wherever the GUI mouse is at startup is considered 0,0 and all movement is offset from there. If no GUI it assumes 0,0 is start. User must draw their own mouse!

class Input
{
public:
	typedef struct MouseData // A very simple structure to hold the mouse info
	{
		int	PositionX; // contains the relative position from the start point (take care to not confuse it with the GUI mouse position) 
		int	PositionY;
		int DeltaX;
		int DeltaY;
		bool LeftButton;
		bool MidButton;
		bool RightButton;

		MouseData()
			: PositionX(0), PositionY(0)
			, DeltaX(0), DeltaY(0),
			LeftButton(false), MidButton(false), RightButton(false)
		{}
	} MouseData;

	bool Keys[512]; // maximum possible keys is a little less than this, but best to be careful in case of future expansion
	MouseData TheMouse;
	bool KeyPressed;

	// Range[-1,1]
	// Origin at center of screen. y is up x is right
	glm::vec2 GetMousePos() const;

	Input();

	/************************
	must call init before use
	************************/
	bool Init(int width, int height);
	/************************
	must call after use
	************************/
	void Terminate();
	/* Requests a update for the mouse delta. */
	void Update();

private:
	struct termio original_termio;
	struct termio new_termio;

	pthread_t threadKeyboard;
	pthread_t threadMouse; // handles for the threads

	bool initCalled;
	bool mQuit;
	bool kQuit;
	bool mKilled;
	bool kKilled;
	bool mUpdate;
	bool kUpdate;

	int w, h;

	//****************************************************************************************************	
	// Intended to be a thread which processes the mouse events and stores  Mouse info in TheMouse struct.
	//****************************************************************************************************	
	static void*  ProcessMouseThread(void* arg);
	//*********************************************************************************	
	// This thread processes the keys, and stores TRUE/FALSE values in the Keys[] array.
	//*********************************************************************************	
	static void* ProcessKeyboardThread(void* arg);
	//*****************************************************************************
	//this will test for our keyboards
	//*****************************************************************************
	int AreYouMyKeyboard();
	std::string kbd; // this will be the event filename for the keyboard discovered in the init/AreYouMyKeyboard test
};

