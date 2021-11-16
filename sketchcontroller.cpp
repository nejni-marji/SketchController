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

	// release mouse, just in case
	inputData.dispCur.sendClick(1, false);
	inputData.dispCur.sendClick(3, false);

	return 0;
}
