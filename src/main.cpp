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
 */

#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <Ticker.h>

#include "tsop3823ir.h"

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


// IR Use turn on the save buffer feature for more complete capture coverage.
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

  /////////////////// TEST PROGMEM: dump dei primi due valori remote KEY_CODES
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
  /////////////////////////////// END TEST /////////////////////////////////////

  //irsend.begin(); // Start the IR LED, defined in PhilipsRC6Remote::init
  irrecv.enableIRIn();  // Start the receiver

  cmdCycle.attach(0.5, processCmdQueue); // check the command queue in buffer every 0.5s

  /////////////////////////// DEBUG some info about SPIFFS
  // TODO inserirle nelle stats (home page graphs) ?? Boh!
  FSInfo info;
  if (!SPIFFS.info(info)) {
    Serial.println("SPIFFS info failed");
  }
  Serial.printf("SPIFFS Total: %u\nUsed: %u\nBlock: %u\nPage: %u\nMax open files: %u\nMax path len: %u\n",
                info.totalBytes,
                info.usedBytes,
                info.blockSize,
                info.pageSize,
                info.maxOpenFiles,
                info.maxPathLength
              );
  /////////////////////////// DEBUG
} // end SETUP

// The repeating section of the code
void loop() {

  // Check if the IR code has been received.
  if (irrecv.decode(&results)) {
    Serial.println();
    // Display a crude timestamp.
    uint32_t now = millis();
    Serial.println("------------------------IR RECEIVER DUMP BEGIN-------------------------");
    Serial.printf("Timestamp : %06u.%03u\n", now / 1000, now % 1000);
    if (results.overflow)
      Serial.printf("WARNING: IR code is too big for buffer (>= %d). "
                    "This result shouldn't be trusted until this is resolved. "
                    "Edit & increase CAPTURE_BUFFER_SIZE.\n",
                    CAPTURE_BUFFER_SIZE);
    Serial.println("------------------------IR RECEIVER START-RESULTS----------------------");
    Serial.print(resultToHumanReadableBasic(&results)); // Display the basic output of what we found.
    Serial.println("------------------------IR RECEIVER START-AC-INFO (if present)---------");
    dumpACInfo(&results); // Display any extra A/C info if we have it.
    yield(); // Feed the WDT as the text output can take a while to print.
    Serial.println("------------------------IR RECEIVER START-RAW--------------------------");
    Serial.println(resultToTimingInfo(&results)); // Output RAW timing info of the result
    yield(); // Feed the WDT (again)
    Serial.println("------------------------IR RECEIVER START-SOURCE-----------------------");
    Serial.println(resultToSourceCode(&results)); // Output the results as source code
    yield(); // Feed the WDT (again)
    Serial.println("-------------------------IR RECEIVER DUMP END---------------------------");
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
