/*
 *  PhilipsRC6Remote
 *  0.4 Apr 2018
 *  0.1 Jan 2018
 *  Copyright 2018 Gianluca Cassarino
 *  Definisce i codici del telecomando del TV Philips22PFT4000_12 nel formato
 *  originale LIRC (lirc.conf).
 *  Definisce i comandi corrispondenti ai tasti del telecomando.
 *  Converte i codici dei segnali IR codificati con il protocollo Philips RC6
 *  dal formato Linux LIRC (lirc.conf) al formato di IRsend per Arduino.
 *  Per convertire RC6 from LIRC to IRsend
 *  ho fatto riferimento alle indicazioni di Ken Shirriff qui:
 *  http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html
 *  Avevo già raccolto i comandi nel formato LIRC tempo prima con un Raspberry
 *  e non avevo voglia di rifare lo stesso lavoro di nuovo per
 *  IRsend, così ho aggiunto questa funzione questa libreria.
 *  I codici LIRC sono relativi al telecomando TV Philips22PFT4000_12
 *  ma dovrebbe convertire tutti i codici nel formato RC6
 *  da LIRC -> IRsend (non testato)
 */

#include <Arduino.h>
#include "PhilipsRC6Remote.h"

IRsend irsend(15); // An IR LED is controlled by GPIO pin 15 (D8)

// names and values are from original Philips22PFT4000_12 lirc.conf
unsigned long KEY_CODES[45] PROGMEM = {
  0x0FFFB2,   // long key press (if tv: move to prev channel)
  0x0FFFD4,   // short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
  0x0EFFB3,   // long key press (if tv: move to next channel)
  0x0EFFD7,   // short key press (if tv: check if usb disk is connected, otherwise prints a message to tv)
  0x0FFFCE,
  0x0EFFCF,
  0x0FFFC8,
  0x0FFFD3,
  0x0FFF33,
  0x0FFF40,
  0x0EFF0A,
  0x0FFFC7,
  0x0FFFAB,
  0x0FFF60,
  0x0EFF92,
  0x0FFF91,
  0x0EFF90,
  0x0FFF8F,
  0x0EFFF0,
  0x0EFFA7,
  0x0EFFBF,
  0x0FFFA5,
  0x0EFFA3,
  0x0FFFA4,
  0x0EFFF5,
  0x0FFFA6,
  0x0EFF2D,
  0x0EFFEF,
  0x0EFFEE,
  0x0FFFF2,
  0x0FFFDF,
  0x0FFFDE,
  0x0EFFFE,
  0x0EFFFD,
  0x0EFFFC,
  0x0FFFFB,
  0x0EFFFA,
  0x0FFFF9,
  0x0FFFF8, //0x0EFFF8
  0x0EFFF7,
  0x0EFFF6,
  0x0EFFB4,
  0x0EFFFF,
  0x0FFFC3,
  0x0EFFF3};

PhilipsRC6Remote::PhilipsRC6Remote(char *remote) {
  this->_remote = remote;
  this->init();
}

void PhilipsRC6Remote::init(){
  irsend.begin(); // Start the IR LED
}

// vedi: http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html
// 1 - leggo il valore pre_data e lo concateno con il valore del comando scelto,
// es: MUTE in lirc RC6 Philips è 0x0FFFF2 mentre il file ha un valore
// di pre_data = 0x77 - concatenando ottengo 0x770FFFF2
// 2 - inverto il valore (binary NOT)
// ottengo 0x88F0000D
//
unsigned long PhilipsRC6Remote::convertLircKeyCode(unsigned long lirc_pre_data, unsigned long lirc_key_code){
  // algoritmo base, testato solo con RC6 13 bits
  unsigned long x = ((unsigned long)lirc_pre_data << 24) | lirc_key_code;
  unsigned long y = ~x;
  //
  Serial.printf("Lirc KeyCode to Arduino KeyCode: %x -> %x\n", x, y);
  return y;
}

String PhilipsRC6Remote::getRemoteModel(){
  String str(this->_remote);
  return str;
}

// KEYS
// NOTE:
// funziona se tolgo il valore di repeat come argomento di irsend (era 1)
// altrimenti se lascio il valore di repeat solo una parte dei tasti funziona
// in particolare funziona un tasto si e uno no, es.: 1 si, 2 no, 3 si, 4 no,...
// Questo forse dipende dal fatto che queste chiamate avvengono in una callback
// asincrona (webserver/callbackJSON), in effetti nei primi test (fuori dalla
// callback) il tasto KEY_POWER con repeat=1 ha sempre funzionato, mentre dopo non piu
void PhilipsRC6Remote::keyPower(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_POWER]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyRewind(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_REWIND]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyForward(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_FASTFORWARD]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyRewindAlt(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_REWIND_ALT]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyForwardAlt(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_FASTFORWARD_ALT]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyStop(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_STOP]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyPause(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_PAUSE]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyRecord(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_RECORD]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyPlay(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_PLAY]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyTvGuide(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_TVGUIDE]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keySetup(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_SETUP]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyFormat(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_FORMAT]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keySources(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_SOURCES]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyHome(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_HOME]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyExit(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_EXIT]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyRed(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_RED]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyGreen(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_GREEN]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyYellow(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_YELLOW]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyBlue(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_BLUE]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyInfo(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_INFO]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyUp(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_UP]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyOptions(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_OPTION]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyLeft(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_LEFT]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyOk(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_OK]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyRight(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_RIGHT]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyBack(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_BACK]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyDown(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_DOWN]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyList(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_LIST]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyVolumeUp(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_VOLUMEUP]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyVolumeDown(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_VOLUMEDOWN]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyVolumeMute(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_MUTE]) );
  //Serial.println(key, HEX); // TEST
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyChannelUp(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_CHANNELUP]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyChannelDown(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_CHANNELDOWN]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key1(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_1]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key2(){
  /////////////////////////////////// TEST ///////////////////////////////////
  //int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_2]) );
  //irsend.sendRC6(key, 20, 1);
  // ok rawData
  //uint16_t rawData[41] = {2676, 882,  454, 862,  486, 396,  454, 428,  484, 858,  904, 412,  458, 432,  458, 424,  510, 372,  488, 394,  454, 430,  462, 420,  460, 422,  458, 424,  456, 426,  454, 428,  484, 400,  480, 402,  458, 422, 904, 860,  488};  // RC6 2
  //irsend.sendRaw(rawData, 41, 38);
  // valore lirc cablato
  //unsigned long key = convertLircKeyCode( 0x77, 0x0EFFFD );
  //irsend.sendRC6(0x88f10003, 20, 1); // valore di key3, ok
  // funziona se inviato due volte
  //irsend.sendRC6(0xFFF10002, 20, 1);
  //irsend.sendRC6(0x88F10002, 20);
  // OK funziona se tolgo il valore di repeat come argomento di irsend (era 1)
  /////////////////////////////////// TEST ///////////////////////////////////
  unsigned long key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_2]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key3(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_3]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key4(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_4]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key5(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_5]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key6(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_6]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key7(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_7]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key8(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_8]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key9(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_9]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::key0(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_0]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keySubtitle(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_SUBTITLE]) );
  irsend.sendRC6(key, 20);
}
void PhilipsRC6Remote::keyText(){
  int key = convertLircKeyCode( 0x77, pgm_read_dword(&KEY_CODES[KEY_TEXT]) );
  irsend.sendRC6(key, 20);
}
