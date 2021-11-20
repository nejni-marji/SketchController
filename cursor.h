#ifndef CURSOR_H
#define CURSOR_H

#include <X11/Xlib.h>
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>
#include <iostream>

class DisplayCursor {
	public:
		DisplayCursor();
		void getDisplaySize(int& w, int& h);
		void getCursorPos(int& x, int& y);
		void setCursorPos(int x, int y);
		void sendClick(int button, bool down);
		void sendKey();
	private:
		Display* display;
		Screen* screen;
		Window root;
		int width, height;
};

typedef DisplayCursor DC;

#endif // CURSOR_H
