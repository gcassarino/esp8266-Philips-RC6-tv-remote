/* Philips RC6 Remote IR transmitter
 * Defines the html endpoint and the related callback for the IR remote command
 * transmitter. Gianluca Cassarino 2018
 *
 * The IR receiver is used just for testing the output received from the IR led
 * transmitter and is based on IRremoteESP8266: IRrecvDumpV2 - dump details of
 * IR codes with IRrecv.
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Example circuit diagram:
 *  https://github.com/markszabo/IRremoteESP8266/wiki#ir-receiving
 * Changes:
 *   Version 0.2 April, 2017
 *     - Decode from a copy of the data so we can start capturing faster thus
 *       reduce the likelihood of miscaptures.
 * Based on Ken Shirriff's IrsendDemo Version 0.1 July, 2009, Copyright 2009 Ken Shirriff, http://arcfn.com
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <Ticker.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>

/* FSBrowserNG dependencies */
#include <ESP8266WiFi.h>
#include "FS.h"
#include <WiFiClient.h>
#include "ESPAsyncTCP.h" // versione presa da github, altrimenti non funziona (con platformio.ini 305)
#include <ESPAsyncWebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>
#include "FSWebServerLib.h" // versione a cui ho applicato alcune modifiche
#include <Hash.h>
/**/

// tv remote key action manager
#include "KeyActions.h"

// An IR detector/demodulator is connected to GPIO pin 5(D1 on a NodeMCU
// board).
uint16_t RECV_PIN = 5;
// As this program is a special purpose capture/decoder, let us use a larger
// than normal buffer so we can handle Air Conditioner remote codes.
uint16_t CAPTURE_BUFFER_SIZE = 1024;

// Nr. of milli-Seconds of no-more-data before we consider a message ended.
// NOTE: Don't exceed MAX_TIMEOUT_MS. Typically 130ms.
#define TIMEOUT 15U  // Suits most messages, while not swallowing repeats.
// #define TIMEOUT 90U  // Suits messages with big gaps like XMP-1 & some aircon
                        // units, but can accidently swallow repeated messages
                        // in the rawData[] output.

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);

decode_results results;  // Somewhere to store the results

Ticker cmdCycle; // used to process the command buffer queue

//---------------------------------------------------------------------------------
// crPrintHEX
//---------------------------------------------------------------------------------

// http://forum.arduino.cc/index.php?topic=114568.0
void crPrintHEX(unsigned long DATA, unsigned char numChars) {
    unsigned long mask  = 0x0000000F;
    mask = mask << 4*(numChars-1);

    for (unsigned int i=numChars; i>0; --i) {
         Serial.print(((DATA & mask) >> (i-1)*4),HEX);
         mask = mask >> 4;
        }

    Serial.print("  ");
}


// boolean syncEventTriggered = false; // True if a time even has been triggered
// NTPSyncEvent_t ntpEvent; // Last triggered event

void setup() {

  // Status message will be sent to the PC at 115200 baud
  Serial.begin(115200);//, SERIAL_8N1, SERIAL_TX_ONLY);

  // WiFi is started inside library
  SPIFFS.begin(); // Not really needed, checked inside library and started if needed
  ESPHTTPServer.begin(&SPIFFS);

  /* add setup code here */

  // NTP.onNTPSyncEvent([](NTPSyncEvent_t event) {
	// 	ntpEvent = event;
	// 	syncEventTriggered = true;
	// });

  // callback for the received IR commands
  ESPHTTPServer.setJSONCallback(keySequence);
  ESPHTTPServer.on("/act", HTTP_GET, keySequence);

  // enabling the cache for the page UI
  // load time goes from ~400ms to ~50ms
  // I can confirm that the speed is OMG :)
  ESPHTTPServer.serveStatic("/tv/index.html", SPIFFS, "/tv/index.html").setCacheControl("max-age=86400"); // 24h
  ESPHTTPServer.serveStatic("/tvl/index.html", SPIFFS, "/tvl/index.html").setCacheControl("max-age=86400"); // 24h
  // NOTE /tvc/ (custom UI) does not cache the page

  //////////////////////////// IR

  // TEST PROGMEM: dump dei primi due valori remote KEY_CODES
  for (int i = 0; i < 2; i++){
    // unsigned long is 32 bit, use 32-bit 'dword'
    unsigned long w = pgm_read_dword(&KEY_CODES[i]);
    // print the 16 bits in a few different ways.
    //Serial.print( w, DEC);        Serial.print("  ");
    //Serial.print( (int) w, DEC);  Serial.print("  ");
    // Serial.print( w, HEX);        Serial.print("  ");
    // Serial.print( (int) w, HEX);  Serial.print("  ");
    // Serial.printf("%02X", w);
    crPrintHEX(w,6);
    Serial.println(); 
  }

  //irsend.begin(); // Start the IR LED, defined in PhilipsRC6Remote::init
  irrecv.enableIRIn();  // Start the receiver
  cmdCycle.attach(0.5, processCmdQueue); // check the command queue in buffer every 0.5s

  /////////////////////////// DEBUG some info about SPIFFS
  // TODO inserirle nelle stats (home page graphs) ?? Boh!
  FSInfo info;
  if (!SPIFFS.info(info)) {
    Serial.println("info failed");
  }
  Serial.printf("Total: %u\nUsed: %u\nBlock: %u\nPage: %u\nMax open files: %u\nMax path len: %u\n",
                info.totalBytes,
                info.usedBytes,
                info.blockSize,
                info.pageSize,
                info.maxOpenFiles,
                info.maxPathLength
              );
  /////////////////////////// DEBUG
} // end SETUP

// Display encoding type
void encoding(decode_results *results) {
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break;
    case NEC:          Serial.print("NEC");           break;
    case NEC_LIKE:     Serial.print("NEC (non-strict)");  break;
    case SONY:         Serial.print("SONY");          break;
    case RC5:          Serial.print("RC5");           break;
    case RC5X:         Serial.print("RC5X");          break;
    case RC6:          Serial.print("RC6");           break;
    case RCMM:         Serial.print("RCMM");          break;
    case DISH:         Serial.print("DISH");          break;
    case SHARP:        Serial.print("SHARP");         break;
    case JVC:          Serial.print("JVC");           break;
    case SANYO:        Serial.print("SANYO");         break;
    case SANYO_LC7461: Serial.print("SANYO_LC7461");  break;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break;
    case SAMSUNG:      Serial.print("SAMSUNG");       break;
    case LG:           Serial.print("LG");            break;
    case WHYNTER:      Serial.print("WHYNTER");       break;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break;
    case PANASONIC:    Serial.print("PANASONIC");     break;
    case DENON:        Serial.print("DENON");         break;
    case COOLIX:       Serial.print("COOLIX");        break;
    case NIKAI:        Serial.print("NIKAI");         break;
  }
  if (results->repeat) Serial.print(" (Repeat)");
}

// Dump out the decode_results structure.
void dumpInfo(decode_results *results) {
  if (results->overflow)
    Serial.printf("WARNING: IR code too big for buffer (>= %d). "
                  "These results shouldn't be trusted until this is resolved. "
                  "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                  CAPTURE_BUFFER_SIZE);

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  serialPrintUint64(results->value, 16);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

uint16_t getCookedLength(decode_results *results) {
  uint16_t length = results->rawlen - 1;
  for (uint16_t i = 0; i < results->rawlen - 1; i++) {
    uint32_t usecs = results->rawbuf[i] * RAWTICK;
    // Add two extra entries for multiple larger than UINT16_MAX it is.
    length += (usecs / UINT16_MAX) * 2;
  }
  return length;
}

// Dump out the decode_results structure.
void dumpRaw(decode_results *results) {
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen - 1, DEC);
  Serial.println("]: ");

  for (uint16_t i = 1; i < results->rawlen; i++) {
    if (i % 100 == 0)
      yield();  // Preemptive yield every 100th entry to feed the WDT.
    if (i % 2 == 0) {  // even
      Serial.print("-");
    } else {  // odd
      Serial.print("   +");
    }
    Serial.printf("%6d", results->rawbuf[i] * RAWTICK);
    if (i < results->rawlen - 1)
      Serial.print(", ");  // ',' not needed for last one
    if (!(i % 8)) Serial.println("");
  }
  Serial.println("");  // Newline
}

// Dump out the decode_results structure.
void dumpCode(decode_results *results) {
  // Start declaration
  Serial.print("uint16_t ");               // variable type
  Serial.print("rawData[");                // array name
  Serial.print(getCookedLength(results), DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (uint16_t i = 1; i < results->rawlen; i++) {
    uint32_t usecs;
    for (usecs = results->rawbuf[i] * RAWTICK;
         usecs > UINT16_MAX;
         usecs -= UINT16_MAX)
      Serial.printf("%d, 0", UINT16_MAX);
    Serial.print(usecs, DEC);
    if (i < results->rawlen - 1)
      Serial.print(", ");  // ',' not needed on last one
    if (i % 2 == 0) Serial.print(" ");  // Extra if it was even.
  }

  // End declaration
  Serial.print("};");  //

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  serialPrintUint64(results->value, HEX);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {
    // Some protocols have an address &/or command.
    // NOTE: It will ignore the atypical case when a message has been decoded
    // but the address & the command are both 0.
    if (results->address > 0 || results->command > 0) {
      Serial.print("uint32_t address = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
      Serial.print("uint32_t command = 0x");
      Serial.print(results->command, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("uint64_t data = 0x");
    serialPrintUint64(results->value, 16);
    Serial.println(";");
  }
}

// The repeating section of the code
void loop() {

  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    Serial.println();
    Serial.println("------------------------IR RECEIVER START-RESULTS----------------------");
    dumpInfo(&results);           // Output the results
    Serial.println("------------------------IR RECEIVER START-RAW--------------------------");
    dumpRaw(&results);            // Output the results in RAW format
    Serial.println("------------------------IR RECEIVER START-SOURCE-----------------------");
    dumpCode(&results);           // Output the results as source code
    Serial.println("-------------------------IR RECEIVER END-------------------------------");
    Serial.println();
  }

  // if (syncEventTriggered) {
	// 	//processSyncEvent(ntpEvent);
  //   Serial.print("SYNC EVT TRIGGERED: ");
  //   Serial.print(NTP.getTimeDateString());
  //   Serial.println("");
	// 	syncEventTriggered = false;
	// }

  // Serial.println(cmdBuf[0]);

  // DO NOT REMOVE. Attend OTA update from Arduino IDE
  ESPHTTPServer.handle();
}
