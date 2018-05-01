/*
 *  KeyActions Version
 *  0.2 Apr 2018
 *  0.1 Jan 2018
 *  Copyright 2018 Gianluca Cassarino
 *  Trasforma le sequenze di tasti "key" ricevuti (da un backend
 *  nel mio caso JSON) in sequenze di comandi da inviare tramite IRsend.
 *  Implementa un buffer per i comandi ricevuti.
 *  Gestisce la sequenza di comandi ricevuta e li accoda in un buffer
 *  per essere eseguiti come comandi IRsend.
 *  NOTE:
 *  Nel mio caso uso la lib intermedia PhilipsRC6Remote per convertire in
 *  runtime i codici del mio telecomando dal formato LIRC al formato IRsend
 *  ma si possono inviare direttamente i comandi nel formato nativo di IRsend.
 *  I nomi dei tasti (es: keyForward) sono relativi al telecomando
 *  del TV Philips22PFT4000_12
 *  Si puo processare il buffer periodicamente attraverso la funzione
 *  processCmdQueue impostando un Ticker, ad esempio:
 *  Ticker cmdCycle;
 *  cmdCycle.attach(0.5, processCmdQueue); call processCmdQueue every 0.5s
 */

// defines a buffer for commands sequence
// process the commands in the buffer
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "KeyActions.h"

char remoteModel[] = "philips_22PFT4000_12_ALT2";
PhilipsRC6Remote remote(remoteModel);

// global command buffer holds commands that will be send to tv via IR
// 255 abbastanza per una sequenza di oltre 20 comandi
// 16 caratteri max per ogni comando (15 + /u0)
char cmdBuf[255][16]=
{
    // TEST VALUES
    // "keyVolumeMute",
    // "keyVolumeMute",
    // "keyVolumePippo"
};

// controlla se ci sono comandi nel buffer e li esegue
// uno alla volta. Estrae l'ultimo comando dalla coda e lo esegue
void processCmdQueue(){
  int lastElem = 0;
  int firstElem = 0;
  int curElem = 0;

  // PRIMO ELEMENTO !=0
  for (int i = 0; i < bufSize; i++) {
    if(cmdBuf[i][0] != '\0'){ // A string is an array of char (bytes), where the last byte is a zero (an actual zero, not an ASCII '0').
      // ho elementi non nulli
      //Serial.print("FIRST ELEM: ");
      //Serial.println(cmdBuf[i]);
      firstElem = i;
      break; // esco subito dopo il primo
      //Serial.println(cmdBuf[i]);
      //Serial.println(strlen(cmdBuf[i]));
    }
    if(i == 254 && cmdBuf[i][0] == '\0') {
      //Serial.println("buffer vuoto");
      //se l'arr è vuoto esco qui
      return;
    };
  }

  curElem = firstElem;

  ////////////////////////////////DEBUG/////////////////////////////////////////
  Serial.print("INDICE FIRST ELEM NON NULLO NEL COMMAND BUFFER: ");
  Serial.println(firstElem);
  Serial.print("FIRST ELEM: ");
  Serial.println(cmdBuf[firstElem]);
  Serial.print("LAST ELEM: ");
  for (int i = 0; i < bufSize; i++) {
    if(cmdBuf[i][0] != '\0'){
      lastElem = i;
    }
  }
  Serial.println(cmdBuf[lastElem]);
  ////////////////////////////////DEBUG/////////////////////////////////////////

  // eseguo il comando
  if (strcmp(cmdBuf[curElem], "printRemoteModel") == 0) {
    String model;
    model = remote.getRemoteModel();
    Serial.print("RemoteModel: ");
    Serial.println(model); // printRemoteModel è usato solo per debug
  } // KEYS
  else if (strcmp(cmdBuf[curElem], "keyPower") == 0){
    remote.keyPower();
  } else if(strcmp(cmdBuf[curElem], "keyStop") == 0){
    remote.keyStop();
  } else if(strcmp(cmdBuf[curElem], "keyPause") == 0){
    remote.keyPause();
  } else if(strcmp(cmdBuf[curElem], "keyRecord") == 0){
    remote.keyRecord();
  } else if(strcmp(cmdBuf[curElem], "keyRewind") == 0){
    remote.keyRewind();
  } else if(strcmp(cmdBuf[curElem], "keyRewindAlt") == 0){
    remote.keyRewindAlt();
  } else if(strcmp(cmdBuf[curElem], "keyPlay") == 0){
    remote.keyPlay();
  } else if(strcmp(cmdBuf[curElem], "keyForward") == 0){
    remote.keyForward();
  } else if(strcmp(cmdBuf[curElem], "keyForwardAlt") == 0){
    remote.keyForwardAlt();
  } else if(strcmp(cmdBuf[curElem], "keyTvGuide") == 0){
    remote.keyTvGuide();
  } else if(strcmp(cmdBuf[curElem], "keySetup") == 0){
    remote.keySetup();
  } else if(strcmp(cmdBuf[curElem], "keyFormat") == 0){
    remote.keyFormat();
  } else if(strcmp(cmdBuf[curElem], "keySources") == 0){
    remote.keySources();
  } else if(strcmp(cmdBuf[curElem], "keyHome") == 0){
    remote.keyHome();
  } else if(strcmp(cmdBuf[curElem], "keyExit") == 0){
    remote.keyExit();
  } else if(strcmp(cmdBuf[curElem], "keyRed") == 0){
    remote.keyRed();
  } else if(strcmp(cmdBuf[curElem], "keyGreen") == 0){
    remote.keyGreen();
  } else if(strcmp(cmdBuf[curElem], "keyYellow") == 0){
    remote.keyYellow();
  } else if(strcmp(cmdBuf[curElem], "keyBlue") == 0){
    remote.keyBlue();
  } else if(strcmp(cmdBuf[curElem], "keyInfo") == 0){
    remote.keyInfo();
  } else if(strcmp(cmdBuf[curElem], "keyUp") == 0){
    remote.keyUp();
  } else if(strcmp(cmdBuf[curElem], "keyOptions") == 0){
    remote.keyOptions();
  } else if(strcmp(cmdBuf[curElem], "keyLeft") == 0){
    remote.keyLeft();
  } else if(strcmp(cmdBuf[curElem], "keyOk") == 0){
    remote.keyOk();
  } else if(strcmp(cmdBuf[curElem], "keyRight") == 0){
    remote.keyRight();
  } else if(strcmp(cmdBuf[curElem], "keyBack") == 0){
    remote.keyBack();
  } else if(strcmp(cmdBuf[curElem], "keyDown") == 0){
    remote.keyDown();
  } else if(strcmp(cmdBuf[curElem], "keyList") == 0){
    remote.keyList();
  } else if(strcmp(cmdBuf[curElem], "keyVolumeUp") == 0){
    remote.keyVolumeUp();
  } else if(strcmp(cmdBuf[curElem], "keyVolumeDown") == 0){
    remote.keyVolumeDown();
  } else if(strcmp(cmdBuf[curElem], "keyVolumeMute") == 0){
    remote.keyVolumeMute();
  } else if(strcmp(cmdBuf[curElem], "keyChannelUp") == 0){
    remote.keyChannelUp();
  } else if(strcmp(cmdBuf[curElem], "keyChannelDown") == 0){
    remote.keyChannelDown();
  } else if(strcmp(cmdBuf[curElem], "key1") == 0){
    remote.key1();
  } else if(strcmp(cmdBuf[curElem], "key2") == 0){
    remote.key2();
  } else if(strcmp(cmdBuf[curElem], "key3") == 0){
    remote.key3();
  } else if(strcmp(cmdBuf[curElem], "key4") == 0){
    remote.key4();
  } else if(strcmp(cmdBuf[curElem], "key5") == 0){
    remote.key5();
  } else if(strcmp(cmdBuf[curElem], "key6") == 0){
    remote.key6();
  } else if(strcmp(cmdBuf[curElem], "key7") == 0){
    remote.key7();
  } else if(strcmp(cmdBuf[curElem], "key8") == 0){
    remote.key8();
  } else if(strcmp(cmdBuf[curElem], "key9") == 0){
    remote.key9();
  } else if(strcmp(cmdBuf[curElem], "key0") == 0){
    remote.key0();
  } else if(strcmp(cmdBuf[curElem], "keySubtitle") == 0){
    remote.keySubtitle();
  } else if(strcmp(cmdBuf[curElem], "keyText") == 0){
    remote.keyText();
  } else if(strcmp(cmdBuf[curElem], "sleep") == 0){
    // do nothing, salta un giro
  } else {
    // do nothing, salta un giro
  }


  //OK 1 - cancello l'ultimo elemento LIFO (l'elemento estratto dal vettore)
  // for (int j = 0; j < keySize; j++) {
  //   cmdBuf[curElem][j] = '\0';
  // }

  //////////////////////////////////////////////////////////////////////////////

  // OK 2 - azzero tutte le lettere che compongono l'elemento nell'array
  /*for (int j = 0; j < keySize; j++) {
    Serial.print("RIMUOVO: ");
    Serial.println(cmdBuf[curElem][j]);
    cmdBuf[curElem][j] = '\0';
  }*/

  // versione più rapida del punto 2 sopra
  // azzero solo la prima lettera, tanto sopra controllo solo quella
  cmdBuf[curElem][0] = '\0';

}

// - Gestisce la sequenza ricevuta
// - spacchetta l'argomento di keySetValue in singoli comandi
// - invia i comandi al buffer
void keySequence(AsyncWebServerRequest *request){

    String actions = request->arg("actions");

    // Convert from String Object to String.
    char buf[255]; // abbastanza per una sequenza di oltre 20 comandi
    actions.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *action;
    // NOTE non funzionano gli invii multipli perche Delay non funziona nelle
    // callback come indicato nella documentazione
    // userò un buffer, vedi di seguito
    int bufIndex = 0; // count occurrences nel buffer
    while ((action = strtok_r(p, ",", &p)) != NULL) { // delimiter is the comma

      Serial.print("action: ");
      Serial.println(action);

      // TODO sul keySetValue
      // un caso da definire in cui come argomento
      // passo un codice ricevuto dalla libreria (usando il ricevitore IR)
      // es inizia con 0x e termina con virgola
      // NOTE è un inferno data la varietà
      // dei protocolli dei telecomandi sarebbe necessario
      // definire troppi casi diversi, TODO APPROFONDIRE

      // verifica argomento = keySetValue
      if(strcmp(action, "keySetValue") == 0) {
        String value = request->arg("value");
        Serial.print("trovato keySetValue");
        // verifica se l'argomento JSON "value" != "false"
        if (value != "false") {
          Serial.print(", value[s]: ");
          Serial.println(value);
          // conversione String -> toCharArray
          // Convert from String Object to String.
          char ksvBuf[255]; // abbastanza per una sequenza di oltre 20 comandi
          value.toCharArray(ksvBuf, sizeof(ksvBuf));
          // extract tokens from string delimited by comma
          char *ksvp = ksvBuf;
          char *ksvVal;
          while ((ksvVal = strtok_r(ksvp, ",", &ksvp)) != NULL) {
            Serial.print("value: ");
            Serial.print(ksvVal);
            // CHECK IF NUMBER
            char *chk = NULL;
            // strtol elimina anche lo zero iniziale
            long ksvValInt = strtol( ksvVal, &chk, 10 ); // 10 -> limitato ai decimali
            if ( !isspace( *chk ) && *chk != 0 ) {
              Serial.print(" is not a valid integer string.");
              //Serial.print(ksvVal);
              // in questo caso accodo il comando ricevuto nel buffer
              // non faccio controlli qui se il comando è valido o no
              Serial.println(" Queuing the command in the buffer.");
              // send key/action to command queue
              strncpy(cmdBuf[bufIndex], ksvVal, keySize );
              bufIndex++;
              //
            } else {
              // il valore è un numero intero valido
              if ( ksvValInt > 9999 ) {
                Serial.print(" number out of range (max 9999 channels): ");
                Serial.println(ksvValInt);
              } else {
                Serial.print(" is a valid integer string: ");
                Serial.print(ksvVal);
                Serial.print(" to int: ");
                Serial.println(ksvValInt);
                //
                if(ksvValInt > 9){
                  Serial.println("Number is > 9, splitting into single digits.");
                  // metto le singole digit in un vettore
                  unsigned int digNum=countDigits(ksvValInt);
                  int d[digNum];
                  while (digNum--) {
                    d[digNum]=ksvValInt%10;
                    ksvValInt/=10;
                  }
                  // inserisco i valori del vettore con le singole digits nel
                  // buffer generale cmdBuf
                  for (size_t e = 0; e < sizeof(d)/sizeof(int); e++) {
                    //Serial.print("KEY_");
                    //Serial.println(d[e]);
                    // converto da intero in stringa
                    char curBuf[4]; //key9 is 4 digits long
                    int n = sprintf(curBuf, "key%d", d[e]);
                    //Serial.println(curBuf);
                    // send key/action to command queue
                    strncpy(cmdBuf[bufIndex], curBuf, keySize );
                    bufIndex++;
                  }

                } else {
                  // il valore è tra 0 e 9
                  // creo KEY_numero (max 1 digit, 0-9)
                  // Serial.print("0-9 KEY_");
                  // Serial.println(ksvValInt);
                  // inserisco il valore ksvValInt con la singola cifra nel
                  // buffer generale cmdBuf
                  // converto da intero in stringa
                  char curBuf[4]; //key9 is 4 char long
                  int n = sprintf(curBuf, "key%d", ksvValInt);
                  // send key/action to command queue
                  strncpy(cmdBuf[bufIndex], curBuf, keySize );
                  bufIndex++;
                }
              }
            }
          }
        } else {
          Serial.println(" value: false");
        }
      } else {
        // il comando non è keySetValue, lo inserisco nel buffer
        // BUFFER BASE (ok)
        // send key/action to command queue
        strncpy(cmdBuf[bufIndex], action, keySize );
        bufIndex++;
      }

      /////////////////////////////////////////////////////////////////////////

      // BUFFER BASE (ok)
      // send key/action to command queue
      // strncpy(cmdBuf[bufIndex], action, keySize );
      // bufIndex++;

    } // end while

    ///////
  	String values = "{\"success\":\"Action \\\""  + actions + "\\\" has been sent.\"}";
    request->send(200, "text/plain", values);
  	values = "";
}

// funzioni di supporto
// conta le cifre in un numero
unsigned int countDigits(unsigned int i) {
  unsigned int ret=1;
  while (i/=10) ret++;
  return ret;
}
