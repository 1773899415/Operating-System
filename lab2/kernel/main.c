#include "common.h"
#include "x86.h"
#include "device.h"

void kEntry(void) {
	// Interruption is disabled in bootloader
	initSerial();// initialize serial port
	initIdt();
	initIntr();
	initSeg();
	initVga();
	initKeyTable();
	loadUMain(); // load user program, enter user space

	while(1);
	assert(0);
}
