/*
 Name:		Spider Firmware
 version:   0.0.4.3(alpha)
 Created:	4/15/2016 2:26:33 PM
 Author:	Chia Ching Yen

 2016 Studio HSUEH & KANE

 Spider Firmware is designed for a cable mounted installation...
*/


/*
The firmware consists discrete .cpp and header files.
This .ino file keeps the compatibility with Arduino IDE.
See more detailed code in spider_main.cpp
*/

#include "spider_main.h"


/*
UPDATE LOG
0.0.4.3(alpha)
>First workable version.
>Fix code_seen() always true problem.
>Refine stepper_move(,,)function.
>Add global variable :mainSpeed
>Add command : M220 set speed, now speed is mutable via command.
>Replace sysRes with define expression 

0.0.4.2(alpha)
>Add Steppers' coordinates and belts' length properties.
>Add function Distance_2Pt for general use.

0.0.4.1(alpha)
>Merge Command.cpp into Spider_main.cpp
>Finish basic command interpreter.
>First Command: M9 T0* H000* S000 

0.0.4(alpha)
>Move from Arduino IDE to Visual Studio.
>Renamed project into 'Spider.'
>Refine the anatomy of firmware.
>Add more comment in source code.

0.0.3(alpha)
>Named this project 'PENTA'
>Add a basic command interpreter function.

0.0.2(alpha)
>Add the SeriaEvent function to grab command via serial port.

0.0.1(alpha)
>Start scripting this new fimware base on a previous Arduino-based project.
*/

/*
To-Do / Known Bugs
Simplify Stepper Class, move Coord caculation to planner.cpp 
*/
