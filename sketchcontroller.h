#ifndef SKETCHCONTROLLER_H
#define SKETCHCONTROLLER_H

struct Config {
	bool verbose    = false;
	int  speed      = 500;
	bool invertX    = false;
	bool invertY    = true;
	bool swapSticks = false;
};

Config CONFIG;

#include <stdio.h>
#include <getopt.h>
#include <thread>
#include "gamepad.cpp"
#include "cursor.cpp"
#include "util.cpp"

void printHelp();

#endif // SKETCHCONTROLLER_H
