# SketchController

SketchController is a program that uses an external gamepad to simulate the effect of an Etch-A-Skech, as a mouse cursor.

## Installation

### Dependencies
- libsdl2-dev
- libx11-dev
- libxtst-dev
- libc6-dev

### Building

```bash
git clone https://github.com/nejni-marji/SketchController
cd SketchController
make
```

## Usage

Rotating the left and right stick will move the cursor horizontally and vertically, respectively.
The right bumper acts as a left click, and the left bumper acts as a right click.
Pressing the guide/home button exits the program.

## Task List

- [ ] command line options
    - [ ] set cursor speed
    - [ ] invert stick controls
    - [ ] enable debug log
    - [ ] adjustable internal framerate?

## Notes

This is a Linux (and X11) specific program. I don't really want to personally make a Windows build of this, but if you do, and you have any questions about the source, feel free to ask me about it.
