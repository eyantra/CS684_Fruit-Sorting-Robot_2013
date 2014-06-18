Main file is fruit_sorting_bot_FBpart.c. It depends on some other c files as follows:

1.lcd1.c: has various functions related to LCD. such as initializing LCD, printing string or charactets on LCD etc.


2.servo1.c: has various functions required for controlling servomotors such as initializing, changing angle etc. 
	Barrior gate & routing channels are controlled using servomotors. 
	So functions in this file are used for moving gate up & down & changing routing path

3.dc_motor1.c: has various functions required for controlling dcmotor such as initializing, setting its direction to rotate, rotate, stop etc.
	these functions are used to move piston forward & backward with required speed

4. ir_proximity.c: has various functions related to IT proximity sensors
	these functions are used for reading values from 2 IR proximity sensors used while extraxting fruit.

5. values.h: a header file which contains all constants/literals. 
	They are declared here instead of using them as magic numbers.

Main logic to perform task of fruit sorting & controlling various components for that is inside main file (Only FB V related)
Image processing related part which works on PC is different. These two communicate via serial communication using USB to serial converter.

**hex file fruit_sorting_bot_FBpart.hex reside in default folder which is to be loaded onto FB V.