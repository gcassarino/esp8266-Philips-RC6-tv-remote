/*  PhilipsRC6Remote
 *  0.3 May 2018
 *  0.2 Apr 2018
 *  0.1 Jan 2018
 *  Copyright 2018 Gianluca Cassarino
 *  Definisce i nomi dei tasti da associare ai codici IR.

The MIT License (MIT)

Original work Copyright (c) 2017 by Gianluca Cassarino

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*

*/
// NOTE text below here is the lirc.conf specific for Philips_22PFT4000_12
// is reported here just as reference
/**
begin remote

  name  Philips_22PFT4000_12_ALT2
  bits           13
  flags RC6|CONST_LENGTH
  eps            30
  aeps          100

  header       2691   868
  one           467   413
  zero          467   413
  pre_data_bits   8
  pre_data       0x77
  gap          106894
  min_repeat      2
#  suppress_repeat 2
#  uncomment to suppress unwanted repeats
  toggle_bit_mask 0x10000
  rc6_mask    0x10000

      begin codes
          KEY_REWIND               0x0FFFB2   # long key press (if tv: move to prev channel)
          KEY_REWIND_ALT           0x0FFFD4   # short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
          KEY_FASTFORWARD          0x0EFFB3   # long key press (if tv: move to next channel)
          KEY_FASTFORWARD_ALT      0x0EFFD7   # short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
          KEY_STOP                 0x0FFFCE
          KEY_PAUSE                0x0EFFCF
          KEY_RECORD               0x0FFFC8
          KEY_PLAY                 0x0FFFD3
          KEY_TVGUIDE              0x0FFF33
          KEY_SETUP                0x0FFF40
          KEY_FORMAT               0x0EFF0A
          KEY_SOURCES              0x0FFFC7
          KEY_HOME                 0x0FFFAB
          KEY_EXIT                 0x0FFF60
          KEY_RED                  0x0EFF92
          KEY_GREEN                0x0FFF91
          KEY_YELLOW               0x0EFF90
          KEY_BLUE                 0x0FFF8F
          KEY_INFO                 0x0EFFF0
          KEY_UP                   0x0EFFA7
          KEY_OPTION               0x0EFFBF
          KEY_LEFT                 0x0FFFA5
          KEY_OK                   0x0EFFA3
          KEY_RIGHT                0x0FFFA4
          KEY_BACK                 0x0EFFF5
          KEY_DOWN                 0x0FFFA6
          KEY_LIST                 0x0EFF2D
          KEY_VOLUMEUP             0x0EFFEF
          KEY_VOLUMEDOWN           0x0EFFEE
          KEY_MUTE                 0x0FFFF2
          KEY_CHANNELUP            0x0FFFDF
          KEY_CHANNELDOWN          0x0FFFDE
          KEY_1                    0x0EFFFE
          KEY_2                    0x0EFFFD
          KEY_3                    0x0EFFFC
          KEY_4                    0x0FFFFB
          KEY_5                    0x0EFFFA
          KEY_6                    0x0FFFF9
          KEY_7                    0x0FFFF8 0x0EFFF8
          KEY_8                    0x0EFFF7
          KEY_9                    0x0EFFF6
          KEY_SUBTITLE             0x0EFFB4
          KEY_0                    0x0EFFFF
          KEY_TEXT                 0x0FFFC3
          KEY_POWER                0x0EFFF3
      end codes

end remote
*/

/*
 *  PhilipsRC6Remote Version 0.1 Jan 2018
 *  Copyright 2018 Gianluca Cassarino
 */

#if (defined(__AVR__))
#include <avr/pgmspace.h>
#else
#include <pgmspace.h>
#endif

#include <Arduino.h>
#include <IRsend.h>

#ifndef PhilipsRC6_h
#define PhilipsRC6_h

// NOTE http://forum.arduino.cc/index.php?topic=166955.0

// names and values are from original Philips22PFT4000_12 lirc.conf
// the order of the list corresponds to the position of the keys on the remote control
// (except power key)
#define	KEY_REWIND 0   // long key press (if tv: move to prev channel)
#define KEY_REWIND_ALT 1   // short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
#define KEY_FASTFORWARD 2   // long key press (if tv: move to next channel)
#define KEY_FASTFORWARD_ALT 3   // short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
#define KEY_STOP 4
#define KEY_PAUSE 5
#define KEY_RECORD 6
#define KEY_PLAY 7
#define KEY_TVGUIDE 8
#define KEY_SETUP 9
#define KEY_FORMAT 10
#define KEY_SOURCES  11
#define KEY_HOME 12
#define KEY_EXIT 13
#define KEY_RED 14
#define KEY_GREEN 15
#define KEY_YELLOW 16
#define KEY_BLUE 17
#define KEY_INFO 18
#define KEY_UP 19
#define KEY_OPTION 20
#define KEY_LEFT 21
#define KEY_OK 22
#define KEY_RIGHT 23
#define KEY_BACK 24
#define KEY_DOWN 25
#define KEY_LIST 26
#define KEY_VOLUMEUP 27
#define KEY_VOLUMEDOWN 28
#define KEY_MUTE 29
#define KEY_CHANNELUP 30
#define KEY_CHANNELDOWN 31
#define KEY_1 32
#define KEY_2 33
#define KEY_3 34
#define KEY_4 35
#define KEY_5 36
#define KEY_6 37
#define KEY_7 38 //0x0EFFF8
#define KEY_8 39
#define KEY_9 40
#define KEY_SUBTITLE 41
#define KEY_0 42
#define KEY_TEXT 43
#define KEY_POWER 44

extern unsigned int KEY_CODES[];

// Class definition
class PhilipsRC6Remote {
private:
  char *_remote;
public:
  PhilipsRC6Remote(char *remote);
  void init();
  String getRemoteModel();
  void processCmd(char *cmd);
  unsigned long convertLircKeyCode(unsigned long lirc_pre_data, unsigned long lirc_key_code);
  // KEYS
  // taphold keys
  void keyRewind();     // long key press (if tv: move to prev channel)
  void keyForward();    // long key press (if tv: move to next channel)
  void keyRewindAlt();  // short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
  void keyForwardAlt(); // short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
  //
  void keyStop();
	void keyPause();
	void keyRecord();
	void keyPlay();
	void keyTvGuide();
	void keySetup();
	void keyFormat();
	void keySources();
	void keyHome();
	void keyExit();
	void keyRed();
	void keyGreen();
	void keyYellow();
	void keyBlue();
	void keyInfo();
	void keyUp();
	void keyOptions();
	void keyLeft();
	void keyOk();
	void keyRight();
	void keyBack();
	void keyDown();
	void keyList();
	void keyVolumeUp();
	void keyVolumeDown();
	void keyVolumeMute();
	void keyChannelUp();
	void keyChannelDown();
	void key1();
	void key2();
	void key3();
	void key4();
	void key5();
	void key6();
	void key7();
	void key8();
	void key9();
	void keySubtitle();
	void key0();
	void keyText();
	void keyPower();

};

#endif
