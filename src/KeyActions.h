/*
 *  KeyActions Version
 *  0.3 May 2018
 *  0.2 Apr 2018
 *  0.1 Jan 2018
 *  Copyright 2018 Gianluca Cassarino
 */

// defines a buffer cmdBuf for commands sequence
// process the commands in the buffer
#include <ESPAsyncWebServer.h>
#include "PhilipsRC6Remote.h" // (Philips RC6 Remote)

const int bufSize = 255;
const int keySize = 16;
// Static array of 255 char elements each of 16 char
// 255 strings of 15 characters (+1 terminator)
extern char cmdBuf[bufSize][keySize];

// function prototype declaration
void keySequence(AsyncWebServerRequest *request);
void processCmdQueue(); // command queue processor
unsigned int countDigits(unsigned int i);
