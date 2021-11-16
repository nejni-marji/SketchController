#include "gamepad.h"

void initializeGamepads(InputData& inputData) {
	// connected gamepad inputs
	std::vector<SDL_GameController*> connInputs;
	// list of gamepad inputs
	std::vector<Gamepad> inputsRaw;
	int numInputs;

	// initialize SDL's game controller subsystem
	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
		SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	// how many controllers are there
	numInputs = 0;
	for (int i=0; i<SDL_NumJoysticks(); i++)
		if (SDL_IsGameController(i))
			numInputs++;

	// if we have controllers attached
	if (numInputs > 0) {
		for (int i=0; i<numInputs; i++) {
			SDL_GameController* pad = SDL_GameControllerOpen(i);
			if (SDL_GameControllerGetAttached(pad) == 1)
				connInputs.push_back(pad); // add to vector
			else
				std::cout << "SDL_GetError() = " << SDL_GetError() << std::endl;
		}
		SDL_GameControllerEventState(SDL_ENABLE);
	}

	// vectors are empty, set their size
	inputsRaw.resize(numInputs);

	// initalize the status to empty
	nullifyGamepadInput(inputsRaw, numInputs);

	// write to database
	while (!inputData.mtx.try_lock()) {};
	inputData.connInputs = connInputs;
	inputData.numInputs = numInputs;
	inputData.inputsRaw = inputsRaw;
	inputData.inputsFrame = inputsRaw;
	inputData.mtx.unlock();
}

void pollGamepadInput(InputData& inputData) {
	/*
 	 * This function does the following:
	 * 10) read data from database
	 * 20) while (true) {
	 * 30) if (frameReset) { nullify inputsFrame }; frameReset = false;
	 * 40) poll for SDL events
	 * 45) copy only true values from inputsRaw to inputsFrame
	 * 50) write inputs to database
	 * 60) exit the loop if the guide button is pressed
	 * 70) }
	 */

	// read from database
	while (!inputData.mtx.try_lock()) {};
	std::vector<SDL_GameController*> connInputs = inputData.connInputs;
	int numInputs = inputData.numInputs;
	std::vector<Gamepad> inputsRaw = inputData.inputsRaw;
	std::vector<Gamepad> inputsFrame = inputData.inputsFrame;
	inputData.mtx.unlock();

	while (true) {
		// poll on events
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					return;
					break;

				case SDL_CONTROLLERDEVICEADDED:
					// printf("DEVICEADDED cdevice.which = %i\n", event.cdevice.which);
					break;

				case SDL_CONTROLLERBUTTONDOWN:
					for (int i=0; i<numInputs; i++) {
						if (event.cbutton.which == SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(connInputs[i])
							)) {
							inputsRaw[i].buttons[event.cbutton.button] = true;
						}
					}
					break;

				case SDL_CONTROLLERBUTTONUP:
					for (int i=0; i<numInputs; i++) {
						if (event.cbutton.which == SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(connInputs[i])
							)) {
							inputsRaw[i].buttons[event.cbutton.button] = false;
						}
					}
					break;

				case SDL_CONTROLLERAXISMOTION:
					for (int i=0; i<numInputs; i++) {
						if (event.cbutton.which == SDL_JoystickInstanceID(
							SDL_GameControllerGetJoystick(connInputs[i])
							)) {
							inputsRaw[i].axes[event.caxis.axis] = event.caxis.value;
						}
					}
					break;
			} // switch
		} // event poll

		// connect to database, checking frameReset, and updating inputsFrame
		while (!inputData.mtx.try_lock()) {};
		// do frame resets
		bool frameReset = inputData.frameReset;
		if (frameReset) {
			nullifyGamepadInput(inputsFrame, numInputs);
			frameReset = false;
		}
		// copy true values from inputsRaw to inputsFrame
		for (int i=0; i<numInputs; i++) {
			// copy axes no matter what
			for (int a=0; a<SDL_CONTROLLER_AXIS_MAX; a++) {
				inputsFrame[i].axes[a] = inputsRaw[i].axes[a];
			}
			// copy button data iff true
			for (int b=0; b<SDL_CONTROLLER_BUTTON_MAX; b++) {
				if (inputsRaw[i].buttons[b])
					inputsFrame[i].buttons[b] = inputsRaw[i].buttons[b];
			}
		}
		// write to database
		inputData.numInputs = numInputs;
		inputData.frameReset = frameReset;
		inputData.inputsRaw = inputsRaw;
		inputData.inputsFrame = inputsFrame;
		inputData.mtx.unlock();

		// check if we should exit
		for (int i=0; i<numInputs; i++) {
			// guide button exits program
			if (inputsRaw[i].buttons[SDL_CONTROLLER_BUTTON_GUIDE])
				return;
		}
	}
};

void queryGamepadInput(InputData& inputData) {
	// TODO: is this required?
	sleepFrame(60);

	// initialize database (inputsQueryPrev causes segfault if empty)
	while (!inputData.mtx.try_lock()) {};
	// prev = data.query
	// query = data.frame
	// data.query = query
	// data.prev = prev
	std::vector<Gamepad> inputsQueryPrev = inputData.inputsQuery;
	std::vector<Gamepad> inputsQuery = inputData.inputsFrame;
	inputData.inputsQuery = inputsQuery;
	inputData.inputsQueryPrev = inputsQueryPrev;
	inputData.mtx.unlock();

	while (true) {
		// read from database
		while (!inputData.mtx.try_lock()) {};
		// prev = data.query
		// query = data.frame
		// data.query = query
		// data.prev = prev
		// frameReset = true
		std::vector<Gamepad> inputsQueryPrev = inputData.inputsQuery;
		std::vector<Gamepad> inputsQuery = inputData.inputsFrame;
		int numInputs = inputData.numInputs;
		inputData.frameReset = true;
		inputData.inputsQuery = inputsQuery;
		inputData.inputsQueryPrev = inputsQueryPrev;
		DisplayCursor dispCur = inputData.dispCur;
		inputData.mtx.unlock();
		// inputsQuery and inputsQueryPrev are now the meaningful input data that we care about

		for (int i=0; i<numInputs; i++) {
			// guide button exits program
			if (inputsQuery[i].buttons[SDL_CONTROLLER_BUTTON_GUIDE]) {
				return;
			}
			bool isButtonPressed = false;
			for (int b=0; b<SDL_CONTROLLER_BUTTON_MAX; b++) {
				if (inputsQuery[i].buttons[b])
					isButtonPressed = true;
				if (inputsQueryPrev[i].buttons[b])
					isButtonPressed = true;
			}
			// show debug data if verbose flag is enabled
			// TODO: move this to executeFrame and add angle data to it
			if (CONFIG.verbose) {
				if (isButtonPressed) {
					printf("PAD: %i  ", i);
					printf("AXES:  ");
					for (int a=0; a<SDL_CONTROLLER_AXIS_MAX; a++) {
						printf(
							"%6i ",
							inputsQueryPrev[i].axes[a]
						);
					}
					printf("BUTTONS:  ");
					for (int b=0; b<SDL_CONTROLLER_BUTTON_MAX; b++) {
						printf(
							"%i",
							inputsQueryPrev[i].buttons[b]
						);
					}
					printf("\n");
				}
				if (isButtonPressed) {
					printf("PAD: %i  ", i);
					printf("AXES:  ");
					for (int a=0; a<SDL_CONTROLLER_AXIS_MAX; a++) {
						printf(
							"%6i ",
							inputsQuery[i].axes[a]
						);
					}
					printf("BUTTONS:  ");
					for (int b=0; b<SDL_CONTROLLER_BUTTON_MAX; b++) {
						printf(
							"%i",
							inputsQuery[i].buttons[b]
						);
					}
					printf("\n");
				}
			}
		}
		// run cursor calculations for each controller
		executeFrame(inputsQuery, inputsQueryPrev, numInputs, dispCur);
	sleepFrame();
	}
};

void nullifyGamepadInput(std::vector<Gamepad>& inputs, int& numInputs) {
	for (int i=0; i<numInputs; i++) {
		for (int a=0; a<SDL_CONTROLLER_AXIS_MAX; a++) {
			inputs[i].axes[a] = 0;
		}
		for (int b=0; b<SDL_CONTROLLER_BUTTON_MAX; b++) {
			inputs[i].buttons[b] = false;
		}
	}
}

void executeFrame(std::vector<Gamepad> inputsQuery, std::vector<Gamepad> inputsQueryPrev, int numInputs, DisplayCursor dispCur) {
	for (int i=0; i<numInputs; i++) {
		// handle button presses
		if (
				inputsQuery[i].buttons[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER]
				&&
				! inputsQueryPrev[i].buttons[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER]
		) {
			// RB: press
			dispCur.sendClick(1, true);
		} else if (
				! inputsQuery[i].buttons[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER]
				&&
				inputsQueryPrev[i].buttons[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER]
		) {
			// RB: release
			dispCur.sendClick(1, false);
		} else if (
				! inputsQuery[i].buttons[SDL_CONTROLLER_BUTTON_LEFTSHOULDER]
				&&
				inputsQueryPrev[i].buttons[SDL_CONTROLLER_BUTTON_LEFTSHOULDER]
		) {
			// LB: release
			dispCur.sendClick(3, true);
		} else if (
				inputsQuery[i].buttons[SDL_CONTROLLER_BUTTON_LEFTSHOULDER]
				&&
				! inputsQueryPrev[i].buttons[SDL_CONTROLLER_BUTTON_LEFTSHOULDER]
		) {
			// LB: release
			dispCur.sendClick(3, false);
		}


		// handle mouse motion
		// StickPair -> StickAngleDelta -> StickAngle
		StickPair sticks = {{{
			inputsQuery[i].axes[0],
			inputsQuery[i].axes[1],
		}, {
			inputsQueryPrev[i].axes[0],
			inputsQueryPrev[i].axes[1],
		}}, {{
			inputsQuery[i].axes[2],
			inputsQuery[i].axes[3],
		}, {
			inputsQueryPrev[i].axes[2],
			inputsQueryPrev[i].axes[3],
		}}};

		// calculate speeds
		calcStickAngleDelta(sticks.L);
		calcStickAngleDelta(sticks.R);

		// swap sticks if flag is enabled
		if (CONFIG.swapSticks) {
			StickPair swappedSticks = {sticks.R, sticks.L};
			sticks = swappedSticks;
		}

		// invert sticks if respective flag is enabled
		if (CONFIG.invertX) {
			sticks.L.speedRad *= -1;
			sticks.L.speedPx  *= -1;
		}
		if (CONFIG.invertY) {
			sticks.R.speedRad *= -1;
			sticks.R.speedPx  *= -1;
		}

		if (sticks.L.speedPx || sticks.R.speedPx) {
			dispCur.setCursorPos(
					sticks.L.speedPx,
					sticks.R.speedPx);
		}
	}
}

void calcStickAngleDelta(StickAngleDelta& delta) {
	// calculate angles
	calcStickAngle(delta.prev);
	calcStickAngle(delta.curr);
	// adjust for the jump from 360° to 0°
	if (STICK_WRAP_HI < delta.prev.a && delta.curr.a < STICK_WRAP_LO) {
		delta.curr.a += 2*M_PI;
	} else if (STICK_WRAP_HI < delta.curr.a && delta.prev.a < STICK_WRAP_LO) {
		delta.prev.a += 2*M_PI;
	}
	if (delta.prev.isDeadzone || delta.curr.isDeadzone) {
		delta.speedRad = 0;
		delta.speedPx  = 0;
	} else {
		delta.speedRad = delta.curr.a - delta.prev.a;
		// px = rad * px/turn / rad/turn
		delta.speedPx  = delta.speedRad * CONFIG.speed/RAD_PER_TURN;
	}
}

void calcStickAngle(StickAngle& angle) {
	angle.r = sqrt( angle.x*angle.x + angle.y*angle.y );
	// reset values if within the deadzone
	if (angle.r < STICK_DEADZONE) {
		angle.x = 0;
		angle.y = 0;
		angle.r = 0;
		angle.a = 0;
		angle.isDeadzone = true;
	}
	// adjust the angles so that they can be used more easily later
	angle.a = atan2(angle.y, angle.x);
	if (angle.a<0) {
		angle.a+=2*M_PI;
	}
}
