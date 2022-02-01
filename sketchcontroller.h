#ifndef SKETCHCONTROLLER_H
#define SKETCHCONTROLLER_H

struct Config {
	bool verbose = false;
	int speed = 500;
	double deadzone = 0.75;
	bool invertX = false;
	bool invertY = true;
	bool swapSticks = false;
};

Config CONFIG;

#include <stdio.h>
#include <getopt.h>
#include <signal.h>
#include <thread>
#include "gamepad.cpp"
#include "cursor.cpp"
#include "util.cpp"

void sigintHandler(int sig);
void printHelp();

#endif // SKETCHCONTROLLER_H
