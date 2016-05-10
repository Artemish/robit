## Overview

Here's all my Lab 1 code. All of the system and task handling code is mine, but
I borrowed serial / command handling code and basic structure from Jacob Quant's
sample code on the Moodle forum.

Oh yeah, and the serial's baud rate is set at 57.6k.

# commands

\# r x - sets the desired reference position as the offset from the current position, in degrees

\# p x - decrements the value of Kp
\# P x - increments the value of Kp

\# d x - decrements the value of Kd
\# D x - increments the value of Kd

\# l - turns on data logging
\# L - turns off data logging

\# h - prints out a help menu
\# s - prints out the state variables, i.e. gains, target, and current position

\# t - executes the trajectory defined in main.c

# relevant files

the 'include' directory contains all of the header files for the system

include/global\_vars.h - All of the system-wide variables, periods, counters, etc

include/pins.h - Structs for all of my pins

include/constants.h - some system-wide constants

The rest of the headers just contain function definitions, as per usual.

io\_wrappers.c - where some abstractions for writing to registers and pins are
defined

command.c - Contains command handling code that interfaces with the serial
buffer

init.c - Initialization code, including functions to reset the system and set
experiment-specific variables

main.c - The main loop, where the trajectory is defined and we request for
serial input to be processed

docs/report.pdf - My full lab report
