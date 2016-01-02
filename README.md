MyOS_Blundell
=============

My attempt of writing an OS from scratch (basically starting with the OS book by Nick Blundell)

Current code :
- print on screen in BIOS mode
- read from floppy disk, initialize Global descriptor table, load the kernel and switch to protected mode
- install Interrupt descriptor table, Interrupt Service Routines, Interrupt Requests
- Handle screen - print on screen, scroll
- Handle keyboard inputs
- Handle timer interrupt

ToBeDone :
- Enable functioning of shift key in keyboard driver(i.e. print capital characters, special symbols ....)
- Memory management - malloc/free
- paging
- scheduler
- multi-programming


COMPILE:
	make

RUN:
	make run

CLEAN
	make clean


REFERENCES:
* Writing a simple operating system from scratch by Nick Blundell
* http://www.osdever.net/bkerndev/Docs/title.htm
* http://www.jamesmolloy.co.uk/tutorial.html
* http://wiki.osdev.org
