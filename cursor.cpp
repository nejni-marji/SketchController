#include "cursor.h"

DC::DisplayCursor(char* d) {
	display = XOpenDisplay(d);
	screen  = XDefaultScreenOfDisplay(display);
	root    = XDefaultRootWindow(display);
	width   = XWidthOfScreen(screen);
	height  = XHeightOfScreen(screen);
}

// DC::~DisplayCursor() {
// 	XCloseDisplay(display);
// }

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

void DC::setCursorPos(int x, int y, bool isAbs) {
	XSelectInput(display, root, KeyReleaseMask);
	// void XWarpPointer(display, src_w, dest_w, src_x, src_y, src_width,
	// src_height, dest_x, dest_y)
	//
	// Set dest_w to None to use relative movement
	XWarpPointer(
			display, None, (isAbs ? root : None),
			0, 0, 0, 0, x, y
			);
	XFlush(display);
}

// void DC::debug() {
// 	int w, h, x, y;
// 	// we can see privates but i want to test the functions, yea?
// 	getDisplaySize(w, h);
// 	getCursorPos(x, y);
// 	printf("w: %i\nh: %i\n", w, h);
// 	printf("x: %i\ny: %i\n", x, y);
// }
