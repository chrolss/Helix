#include "joystick.h"
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <thread>

int standardTest(Joystick joystick)
{
	while (true)
	{
		//printf("inside standard\n");
		// Restrict rate
		usleep(1000);
		//printf("GOING AROUND\n");
		// Attempt to sample an event from the joystick
		JoystickEvent event;
		if (joystick.sample(&event))
		{
			printf("found event\n");
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

void muppHelvete(int (&arr)[18])
{
		Joystick joystick("/dev/input/js0");
		usleep(100000);
		printf("GOING AROUND\n");
		int pos;
		while (true){
		// Attempt to sample an event from the joystick
		JoystickEvent event;
		if (joystick.sample(&event))
		{
			//printf("sampled\n");
			if (event.isButton())
			{
				pos = event.number;
				arr[pos] = event.value;
				//printf("Button %u is %s\n",event.number,event.value == 0 ? "up" : "down");
			}
			else if (event.isAxis())
			{
				pos = 10 + event.number;
				arr[pos] = event.value;
				//printf("Axis %u is at position %d\n", event.number, event.value);
			}
		}
		}
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

	int readings [18];
	std::thread joy(muppHelvete, std::ref(readings));

	while (true){
		usleep(1000);
		printf("x-axis: %d, y-axis: %d, z-axis: %d, throttle: %d\n",readings[10],readings[11], readings[12], readings[13]);
	}
	printf("end\n");
}
