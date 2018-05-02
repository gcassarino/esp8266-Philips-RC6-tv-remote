# esp8266-Philips-RC6-tv-remote
an implementation of the Philips 22PFT4000/12 tv remote control interface using Javascript and an ESP8266/IRSend as backend.

NOTE: This is currently work in progress.

This implementation for esp8266 derives from another one that I previously made for the Raspberry, using PHP and LIRC. The original PHP project for the Raspberry
is avalilable here https://github.com/gcassarino/jquery-mobile-Philips-RC6-tv-remote

I rewrote the PHP backend in C to work on the small microcontroller esp8266 using the IRSend library as the equivalent of LIRC used in the Raspberry project. It all started as a challenge to understand if it was possible to "easily" bring the functionality of my TV infrared remote control from the Raspberry version to a microcontroller with wifi functionality. The inevitable choice was the esp8266 and thanks to the excellent libraries of IRSEND and FSBrowserNG as framework for web pages I managed to replicate the functionality of the Raspberry more efficiently and without too much difficulties. To convert RC6 from LIRC to IRsend I referred to Ken Shirriff's instructions here: http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html

Apart from the fact that I'm not an experienced C programmer, I have to admit that I really enjoyed myself porting the code from PHP to C and this new version is better than the original one in PHP on the Raspberry!

NOTE: This is currently work in progress, a lot of messages come out from the serial console.
