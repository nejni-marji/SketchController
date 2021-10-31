#ifndef CURSOR_H
#define CURSOR_H

#include <X11/Xlib.h>
#include <unistd.h>
#include <iostream>

class DisplayCursor {
	public:
		// constructors
		DisplayCursor(): DisplayCursor(0) {};
		DisplayCursor(char* d);
		// this destructor seemed like a good idea but it causes crashes
		// ~DisplayCursor();
		// data functions
		void getDisplaySize(int& w, int& h);
		void getCursorPos(int& x, int& y);
		void setCursorPos(int x, int y, bool isAbs=false);
		// void debug();
	private:
		Display* display;
		Screen* screen;
		Window root;
		int width, height;
};

typedef DisplayCursor DC;

#endif // CURSOR_H
