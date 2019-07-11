#include "Input.h"
#include <dirent.h>
#include <sstream>
#include <unistd.h>
#include <fcntl.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
// this lets us redirect our OS keyboard systems

#define bitset(byte, mask)	(((byte) & (mask)) != 0)
#define clamp(v, x, y)		(((v) < (x) ? (x) : ((v) > (y) ? (y) : (v))))

/*
{ Structure-diagram, mice file format }
The format for the /dev/input/mice file (3 bytes).

0               1               3
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|L|R|M|         |   Relative X  |   Relative Y  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

L:				Left button						(1 bit)
The state of the left mouse button, pressed or released.

R:				Right button					(1 bit)
The state of the right mouse button, pressed or released.

M:				Middle button					(1 bit)
The state of the middle mouse button, pressed or released.

Relative X		Relative horizontal movement	(8 bits)
The relative mouse movement during the event.

Relative Y		Relative vertical movement		(8 bits)
The relative mouse movement during the event.
*/
void* Input::ProcessMouseThread(void* arg)
{
	/* Attempt to open the all mice file. */
	Input *input = reinterpret_cast<Input*>(arg);
	printf("Starting mouse thread.\n");
	int file = open("/dev/input/mice", O_RDONLY);

	/* Check if the file is found. */
	if (file != -1)
	{
		/* Buffer to store the file data. */
		unsigned char data[3];

		/* Loop untill terminate is called. */
		while (!input->mQuit)
		{
			/* Make sure the binary data is correct. */
			if (read(file, data, sizeof(data)) > 0)
			{
				unsigned char btnFlags = data[0];
				int x = static_cast<signed char>(data[1]);
				int y = -static_cast<signed char>(data[2]);

				/* Update mouse buttons. */
				input->TheMouse.LeftButton = bitset(btnFlags, 0x1);
				input->TheMouse.RightButton = bitset(btnFlags, 0x2);
				input->TheMouse.MidButton = bitset(btnFlags, 0x4);

				/* Update mouse delta. */
				input->TheMouse.DeltaX += x;
				input->TheMouse.DeltaY += y;

				/* Update mouse position. */
				input->TheMouse.PositionX += x;
				input->TheMouse.PositionY += y;

				/* Make sure the absolute mouse doesn't exit the window. */
				input->TheMouse.PositionX = clamp(input->TheMouse.PositionX, 0, input->w);
				input->TheMouse.PositionY = clamp(input->TheMouse.PositionY, 0, input->h);
			}

			/* After an update is requested, reset the delta. */
			if (input->mUpdate)
			{
				input->TheMouse.DeltaX = 0;
				input->TheMouse.DeltaY = 0;
				input->mUpdate = false;
			}
		}

		/* Make sure we close the file after excecution. */
		close(file);
	}

	/* Tell the input object that the mouse thread is closing. */
	printf("Killing mouse thread.\n");
	input->mKilled = true;
	pthread_exit(nullptr);
}

/*
{ Structure-diagram, keyboard file format }
The format for the /dev/input/by-path/<port>-<id>-event-kbd file (24 bytes).

0               1               3				4
0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|																|
+							Seconds								+
|																|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|																|
+						  Microseconds							+
|						  										|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Type              |              Code             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             Value                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

Seconds				UNIX timestamp					(64 bits)
The time in seconds since the UNIX epoch at the time of the event.

Microseconds		High precision timestamp		(64 bits)
The time in microseconds at the event time, useful for times that
cannot be represented in seconds alone.

Type:				Event type						(16 bits)
The type of event, for example EV_KEY for key button press.

Code:				Event code						(16 bits)
The code of the event, for example KEY_BACKSPACE for the backspace key.

Value:				Event value						(32 bits)
The value of the event, for example 1 for key pressed 0 for key release.
*/
void* Input::ProcessKeyboardThread(void* arg)
{
	/* Open the specified keyboard file. */
	Input *input = reinterpret_cast<Input*>(arg);
	printf("Starting keyboard thread.\n");
	int file = open(input->kbd.c_str(), O_RDONLY);

	/* Check if file is not found. */
	if (file != -1)
	{
		input_event data;

		/* Loop untill terminate is called. */
		while (!input->kQuit)
		{
			/* Make sure the event data is read correctly. */
			if (read(file, &data, sizeof(data)) > 0)
			{
				if (data.type == EV_KEY)
				{
					input->KeyPressed = true;
					input->Keys[data.code] = data.value != 0;
				}
			}

			/* After an update is requested reset the keypress event. */
			if (input->kUpdate)
			{
				input->KeyPressed = false;
				input->kUpdate = false;
			}
		}

		/* Make sure we close the file after excecution. */
		close(file);
	}

	/* Tell the input object that the keyboard thread is closing. */
	printf("Killing keyboard thread.\n");
	input->kKilled = true;
	pthread_exit(nullptr);
}

glm::vec2 Input::GetMousePos() const
{
	glm::vec2 pos(TheMouse.PositionX, h - TheMouse.PositionY);
	pos.x /= static_cast<float>(w);
	pos.y /= static_cast<float>(h);
	pos *= 2.f;
	pos -= 1.f;
	return pos;
}

Input::Input()
	: initCalled(false), 
	mQuit(false), kQuit(false),
	mKilled(false), kKilled(false),
	mUpdate(false), kUpdate(false)
	
{
	for (size_t i = 0; i < sizeof(Keys); i++)
	{
		Keys[i] = false;
	}
}

bool Input::Init(int width, int height)
{
	/* Make sure we don't create a new thread when we are already running threads. */
	if (initCalled)
	{
		printf("Cannot call Init again before calling Terminate!\n");
		return false;
	}
	else initCalled = true;

	/* Set screen width and height. */
	w = width;
	h = height;
	int result;

	/* Create mouse thread with Input argument. */
	result = pthread_create(&threadMouse, nullptr, &ProcessMouseThread, this);
	if (result) printf("Unable to create mouse thread: %d!\n", errno);

	/* Attempt to find a usable keyboard. */
	if (AreYouMyKeyboard())
	{
		/* Create keyboard thread with Input argument. */
		result = pthread_create(&threadKeyboard, nullptr, &ProcessKeyboardThread, this);
		if (result) printf("Unable to create keyboard thread: %d!\n", errno);
	}
	else
	{
		kKilled = true;
		printf("Unable to find keyboard!\n");
	}

	// Redirect input
	int fd = fileno(stdin);
	ioctl(fd, TCGETA, &original_termio);

	new_termio = original_termio;
	new_termio.c_cc[VMIN] = 0; //http://gleenders.blogspot.nl/2014/08/banana-pi-resizing-sd-card-root.html
	new_termio.c_cc[VTIME] = 0;
	new_termio.c_lflag = 0; // stop echo (and other things)

	ioctl(fd, TCSETA, &new_termio);

	/* Return whether the thread creation was successful. */
	return result == 0;
}

void Input::Terminate()
{
	/* Make sure we don't terminate non-initialized threads. */
	if (!initCalled)
	{
		printf("Cannot call Terminate before calling Init!\n");
		return;
	}

	/* Call quit for both threads. */
	mQuit = true;
	kQuit = true;

	/* Make sure we wait untill the files are closed before continueing. */
	while (!(mKilled && kKilled));
	initCalled = false;

	int fd = fileno(stdin);
	ioctl(fd, TCSETA, &original_termio);  // put the old one back
}

void Input::Update()
{
	if (!mUpdate) mUpdate = true;
	if (!kUpdate) kUpdate = true;
}

// tests for the keyboard, which can be on different events in Linux
// thanks to my student Petar Dimitrov for this improvement to the keyboard search systems
int Input::AreYouMyKeyboard()
{
	/*
	 Note linux machines may have their key and mouse event files access protected,
	 in which case open a command line terminal, and enter
	 sudo chmod  a+r /dev/input/ (assuming your input event files are there)
	 this is kinda frowned on by linux users, but I don't know a way to overcome this in code yet.
	 it may also be possible to get VisualGDB to execute the build as sudo for root access but I find that flakey
	*/
	// Some bluetooth keyboards are registered as "event-mouse".
	// If this is your case, then just change this variable to event-mouse.
	std::string pattern = "event-kbd"; //<-change to event-mouse if your BT keyboard is a "mouse" or test for a failure to find a kbd, then try as a mouse.
	std::string file = "";

	DIR *dir;
	struct dirent *ent;

	printf("Checking for active keyboard\n");

	if ((dir = opendir("/dev/input/by-path/")) != nullptr)
	{
		while ((ent = readdir(dir)) != nullptr)
		{
			fprintf(stdout, "%s\n", ent->d_name);
			file = std::string(ent->d_name);

			if (!file.compare(".") || !file.compare("..")) continue;

			if (file.substr(file.length() - pattern.length()) == pattern)
			{
				kbd = "/dev/input/by-path/" + file;
				fprintf(stdout, "%s\n", kbd.c_str());

				return true;
			}
		}
	}

	return false;
}