#include "cursor.h"

DC::DisplayCursor(char* d) {
	display = XOpenDisplay(d);
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
	// bool XQueryPointer(display, w, root_return, child_return, root_x_return,
	// root_y_return, win_x_return, win_y_return, mask_return)
	//
	// XQueryPointer segfaults if it writes to 0x00000000
	nullptr_t null = NULL;
	XQueryPointer(
			display, root,
			(Window*) &null,
			(Window*) &null,
			&x, &y, // dereference values passed by reference
			(int*) &null,
			(int*) &null,
			(unsigned int*) &null
			);
}

void DC::setCursorPos(int x, int y) {
	XTestFakeMotionEvent(display, x, y, CurrentTime);
	XFlush(display);
}

void DC::sendClick(int button, bool down) {
	XTestFakeButtonEvent(display, button, down, CurrentTime);
	XFlush(display);
}
