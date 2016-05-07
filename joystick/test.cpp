#include "joystick.h"
#include <unistd.h>
#include <stdio.h>

int standardTest(Joystick joystick)
{
	while (true)
	{
		// Restrict rate
		usleep(1000);

		// Attempt to sample an event from the joystick
		JoystickEvent event;
		if (joystick.sample(&event))
		{
			if (event.isButton())
			{
				printf("Button %u is %s\n",event.number,event.value == 0 ? "up" : "down");
			}
			else if (event.isAxis())
			{
				printf("Axis %u is at position %d\n", event.number, event.value);
			}
		}
	}
}

int sampleTest(Joystick joystick){
	// perform one sampling of all buttons and axes at one time
	// and recall this function for every loop iteration
	JoystickEvent event;
	if (joystick.sample(&event)){
		printf("found event\n");
		if (event.isAxis()){
			if (event.number == 0){
				printf("Axis 0: %d \n", event.value);
			}
		}
	}
	return 1;
}

int main(int argc, char** argv)
{
	// Create an instance of Joystick
	Joystick joystick("/dev/input/js0");

	// Ensure that it was found and that we can use it
	if (!joystick.isFound())
	{
		printf("open failed.\n");
		return -1;
	}

	//standardTest(joystick);
	while (true){
		sampleTest(joystick);
		usleep(10000);
	}
}
