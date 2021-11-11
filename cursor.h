#ifndef CURSOR_H
#define CURSOR_H

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include <iostream>

class DisplayCursor {
	public:
		// constructors
		DisplayCursor();
		// data functions
		void getDisplaySize(int& w, int& h);
		void getCursorPos(int& x, int& y);
		void setCursorPos(int x, int y);
		// void sendClickOld(int button, char mode);
		void sendClick(int button, bool down);
		// button to click, mode is press or release
		// void debug();
	private:
		Display* display;
		Screen* screen;
		Window root;
		int width, height;
};

typedef DisplayCursor DC;

#endif // CURSOR_H
