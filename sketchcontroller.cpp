#include "sketchcontroller.h"

int main(int argc, char** argv) {
	// parse command-line options
	while (true)
	{
		switch(getopt(argc, argv, "hvs:d:xXyYzZ"))
		{
			case 'v':
				CONFIG.verbose = true;
				continue;

			case 's':
				CONFIG.speed = atoi(optarg);
				continue;

			case 'd':
				CONFIG.deadzone = atof(optarg);
				continue;

			case 'x':
				CONFIG.invertX = true;
				continue;

			case 'X':
				CONFIG.invertX = false;
				continue;

			case 'y':
				CONFIG.invertY = true;
				continue;

			case 'Y':
				CONFIG.invertY = false;
				continue;

			case 'z':
				CONFIG.swapSticks = true;
				continue;

			case 'Z':
				CONFIG.swapSticks = false;
				continue;


			case '?':
			case 'h':
			default :
				printHelp();
				return 0;
				break;

			case -1:
				break;
		}

		break;
	}

	// make database, start threads
	InputData inputData;
	inputData.dispCur = DisplayCursor();
	// inputData.dispCur.debug();
	initializeGamepads(inputData);
	std::thread threadPollGamepad (pollGamepadInput, std::ref(inputData));
	std::thread threadQueryGamepad (queryGamepadInput, std::ref(inputData));

	// finally, join threads
	threadPollGamepad.join();
	threadQueryGamepad.join();

	// release mouse, just in case
	inputData.dispCur.sendClick(1, false);
	inputData.dispCur.sendClick(3, false);

	return 0;
}

void printHelp() {
	printf(
			"Usage: sketchcontroller [ -h -v -xXyYzZ ] [ -s SPEED ]\n"
			"  -h           show this message and exit\n"
			"  -v           run verbosely\n"
			"  -s SPEED     set cursor speed in pixels/turn   (default: 500)\n"
			"  -d DEADZONE  set analog stick deadzone   (default: 0.75)\n"
			"  -x           invert x-axis\n"
			"  -X           do not invert x-axis   (default)\n"
			"  -y           invert y-axis   (default)\n"
			"  -Y           do not invert y-axis\n"
			"  -z           swap left and right sticks\n"
			"  -Z           do not swap sticks   (default)\n"
		);
}
