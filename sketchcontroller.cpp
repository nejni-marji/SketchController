#include "sketchcontroller.h"

int main() {
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

	return 0;
}
