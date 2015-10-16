Name: Arya Mohades
UTEID: am52293
UTCS ID: am611
Class: CS354 Computer Graphics
Project #: 3

Running the program:

Compile using "make"
Run using "./modelviewer <path to obj file>" (e.g. ./modelviewer models/cessna.obj)

Sample runs:
./modelviewer models/cessna.obj
./modelviewer models/cessna.obj -f commands.txt (sample commands file included in submission)

Sample commands (press i with window focused to start terminal input)
L feline4k
D
L cessna.obj
T 0.1 0.0 0.1
I


Toggles:

n - toggle face normals
m - toggle vertex normals
1 - render in point mode
2 - render in wireframe mode
3 - render in solid mode
4 - render in shaded mode


Controls:

middle click and drag - pan camera
right click and drag - zoom camera
left click and drag - orbit camera
hold 'w' 'a' 's' 'd' keys to move object up/left/down/right respectively (Reset using 'I' command as seen below)


Interactive control:

With the opengl window in focus, press 'i' key on keyboard to toggle an input command in terminal:

	L <objectfilename>: Loads a new current object; does not have to include .obj extension
						(e.g. "L cessna" or "L cessna.obj" both work)

	D: Delete current object and load previous object (if exists)

	I: Neutralize effects of any transformations

	T <tx> <ty> <tz>: Add a translation to the object transform

	S <sx> <sy> <sz>: Add a scale to the object transform

	R <theta> <ax> <ay> <az>: Add a rotation to the object transform

	V: not implemented

	W: not implemented


Specifying a commands file:

When running the program, you can specify a command file using -f <filename>.
The program will read the list of commands and apply them to the model transform before starting interactive control.

Notes:
- Translating object has wildly different results e.g. translating cessna by T 0.1 0.0 0.0 barely has an effect whereas
	the same translation on the bu_head moves the head so far...

