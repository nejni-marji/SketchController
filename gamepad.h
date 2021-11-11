#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <iostream>
#include <vector>
#include <mutex>
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#include "cursor.h"
#include "util.h"

// debug
const bool DEBUG_INPUT_TEST = true;

// stick properties
const int STICK_MAX =  32677;
const int STICK_MIN = -32678;
const int STICK_DEADZONE = 0.95*STICK_MAX;
const double STICK_WRAP_HI = 1.75*M_PI;
const double STICK_WRAP_LO = 0.25*M_PI;

// adjusting cursor speed
// TODO: add command line option for this
const double PX_PER_TURN  = 500;
const double RAD_PER_TURN = 2*M_PI;
const double PX_PER_RAD = PX_PER_TURN/RAD_PER_TURN;

// TODO: rename this?
struct Gamepad {
	int axes[SDL_CONTROLLER_AXIS_MAX];
	bool buttons[SDL_CONTROLLER_BUTTON_MAX];
};

// TODO: rename this?
struct InputData {
	std::mutex mtx;
	std::vector<SDL_GameController*> connInputs;
	int numInputs;
	bool frameReset;
	// yes, you really do need all 4 of these
	std::vector<Gamepad> inputsRaw;
	std::vector<Gamepad> inputsFrame;
	std::vector<Gamepad> inputsQuery;
	std::vector<Gamepad> inputsQueryPrev;
	// finally we can pass a DisplayCursor through here
	DisplayCursor dispCur;
};

// initialize gamepads and run polling loop
void initializeGamepads(InputData& inputData);

// poll input and write to database
void pollGamepadInput(InputData& inputData);

// query database and do "stuff"
void queryGamepadInput(InputData& inputData);

// set the contents of an input all to false
void nullifyGamepadInput(std::vector<Gamepad>& inputs, int& numInputs);

// the aforementioned "stuff"
void executeFrame(
		std::vector<Gamepad> inputsQuery,
		std::vector<Gamepad> inputsQueryPrev,
		int numInputs,
		DisplayCursor dispCur);

struct StickAngle {
	int x;
	int y;
	double r;
	double a;
	bool isDeadzone = false;
};

struct StickAngleDelta {
	StickAngle curr;
	StickAngle prev;
	double speedRad;
	int speedPx;
};

struct StickPair {
	StickAngleDelta L;
	StickAngleDelta R;
};

// calculate angle data from (x, y) positions
void calcStickAngle(StickAngle& angle);

// calculate speed as delta of two angles
void calcStickAngleDelta(StickAngleDelta& delta);

#endif // GAMEPAD_H
