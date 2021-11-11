#ifndef CURSOR_H
#define CURSOR_H

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <iostream>

class DisplayCursor {
	public:
		DisplayCursor();
		void getDisplaySize(int& w, int& h);
		void getCursorPos(int& x, int& y);
		void setCursorPos(int x, int y);
		void sendClick(int button, bool down);
	private:
		Display* display;
		Screen* screen;
		Window root;
		int width, height;
};

typedef DisplayCursor DC;

#endif // CURSOR_H
