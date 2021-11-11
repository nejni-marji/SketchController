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
