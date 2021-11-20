#include "cursor.h"

DC::DisplayCursor() {
	display = XOpenDisplay(NULL);
	screen  = XDefaultScreenOfDisplay(display);
	root    = XDefaultRootWindow(display);
	width   = XWidthOfScreen(screen);
	height  = XHeightOfScreen(screen);
}

void DC::getDisplaySize(int& w, int& h) {
	w=width;
	h=height;
}

void DC::getCursorPos(int& x, int& y) {
	// XQueryPointer segfaults if it writes to 0x00000000
	nullptr_t null = NULL;
	XQueryPointer(
			display, root,
			(Window*) &null,
			(Window*) &null,
			&x, &y,
			(int*) &null,
			(int*) &null,
			(unsigned int*) &null
			);
}

void DC::setCursorPos(int x, int y) {
	XTestFakeRelativeMotionEvent(display, x, y, CurrentTime);
	XFlush(display);
}

void DC::sendClick(int button, bool down) {
	XTestFakeButtonEvent(display, button, down, CurrentTime);
	XFlush(display);
}

void DC::sendKey() {
	int min;
	int max;
	int num;
	int keycode;
	int keysym;
	int useKey;

	// get and print min/max
	XDisplayKeycodes(display, &min, &max);
	printf("min: %i\n", min);
	printf("max: %i\n", max);

	KeySym* syms;
	syms = XGetKeyboardMapping(display, 8, 248, &num);
	printf("num: %i\n", num);
	for (int i=min;i<=max;i++) {
		// for (int j=0;j<num;j++) {
		int j=0; {
			// i think this is right
			// (as per:      (K - first_code) * keysyms_per_code_return + N   )
			keycode = (i-min) * num + j;
			keysym = *(syms+keycode);
			if (keysym == XK_z) {
				printf("i: %i, j: %i, keysym: %i\n", i, j, keysym);
				printf("keycode: %i\n", keycode);
				useKey = i;
				XTestFakeKeyEvent(display, useKey, true, CurrentTime);
				XTestFakeKeyEvent(display, useKey, false, CurrentTime);
			}
		}
	}
	printf("break here plox\n");
}
