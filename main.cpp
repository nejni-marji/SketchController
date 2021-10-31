#include "main.h"

// void testPos(DC c, int x, int y, bool isAbs=false) {
// 	c.setCursorPos(x, y, isAbs);
// 	c.debug();
// 	sleep(1);
// }

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

	// int w, h, x, y;
	// pointer.getDisplaySize(w, h);

	/*
	DisplayCursor pointer = DisplayCursor();

	int w, h, x, y;
	pointer.getDisplaySize(w, h);
	// pointer.debug();

	testPos(pointer, w/2, h/2, true);
	testPos(pointer, 100, 100, false);
	testPos(pointer, 0, 0, true);
	testPos(pointer, 500, 100, false);
	*/

	return 0;
}
