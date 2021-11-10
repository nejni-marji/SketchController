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

// // i stole most of this code from a forum post lol, wish me luck
// void DC::sendClickOld(int button, char mode) {
// 	// make and initialize an event object
// 	XEvent event;
// 	memset(&event, 0x00, sizeof(event));

// 	// start filling the event with real data
// 	switch (mode) {
// 		case ('P'):
// 			event.type = ButtonPress;
// 			break;
// 		case ('R'):
// 			event.type = ButtonRelease;
// 			break;
// 	}
// 	event.xbutton.button = button;
// 	event.xbutton.same_screen = True;
// 	XQueryPointer(
// 			display,
// 			root,
// 			&event.xbutton.root,
// 			&event.xbutton.window,
// 			&event.xbutton.x_root,
// 			&event.xbutton.y_root,
// 			&event.xbutton.x,
// 			&event.xbutton.y,
// 			&event.xbutton.state
// 			);
// 	event.xbutton.subwindow = event.xbutton.window;

// 	// this loop seems to store the last valid subwindow into window
// 	// i'm not really sure why that's necessary, but i'll keep it for now
// 	while(event.xbutton.subwindow)
// 	{
// 		event.xbutton.window = event.xbutton.subwindow;

// 		XQueryPointer(
// 				display,
// 				event.xbutton.window,
// 				&event.xbutton.root,
// 				&event.xbutton.subwindow,
// 				&event.xbutton.x_root,
// 				&event.xbutton.y_root,
// 				&event.xbutton.x,
// 				&event.xbutton.y,
// 				&event.xbutton.state
// 				);
// 	}

// 	// 0xfff is a full event mask to pass all events through to the client
// 	// i'm not sure what that means *exactly*, but it sounds good
// 	if ( XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0 ) {
// 		fprintf(stderr, "Error sending event !!!\n");
// 	}

// 	// finally flush the display
// 	XFlush(display);
// }

void DC::sendClick(int button, bool down) {
	Display *display = XOpenDisplay(NULL);
	XTestFakeButtonEvent(display, button, down, CurrentTime);
	XFlush(display);
	XCloseDisplay(display);
}
