#include "joystick.h"
#include <unistd.h>
#include <stdio.h>

int standardTest(Joystick joystick)
{
	while (true)
	{
		// Restrict rate
		usleep(1000);
		//printf("GOING AROUND\n");
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

int sampleTest(Joystick joystick, JoystickEvent event, int* _readings){
	// perform one sampling of all buttons and axes at one time
	// and recall this function for every loop iteration
	//JoystickEvent event;
	for (int i = 0; i<18; i++){
		if (joystick.sample(&event)){
				//printf("found event\n");
				/*
				if (event.isButton())
				{
					printf("Button %u is %s\n",event.number,event.value == 0 ? "up" : "down");
				}
				else if (event.isAxis())
				{
					printf("Axis %u is at position %d\n", event.number, event.value);
				}
				*/
				_readings[i] = event.value;
			}
	}
	//printf("Exit\n");
	return 1;
}

int main(int argc, char** argv)
{
	/*
	// Create an instance of Joystick
	Joystick joystick("/dev/input/js0");

	// Ensure that it was found and that we can use it
	if (!joystick.isFound())
	{
		printf("open failed.\n");
		return -1;
	}
	*/
	int readings[18];
	JoystickEvent event;
	Joystick joystick("/dev/input/js0");
	standardTest(joystick);
	/*
	while (true){
		//printf("Entering\n");
		Joystick joystick("/dev/input/js0");
		sampleTest(joystick, event, readings);
		printf("X-axis: %d, Y-axis: %d, z-axis: %d\n", readings[12], readings[13], readings[14]);
		usleep(1000);
		joystick.~Joystick();
	}
	*/
}
