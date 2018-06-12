# esp8266-Philips-RC6-tv-remote
an implementation of the Philips 22PFT4000/12 tv remote control interface using Javascript and an ESP8266/IRSend as backend.

NOTE: This is currently work in progress.

This implementation for esp8266 derives from another one that I previously made for the Raspberry, using PHP and LIRC. The original PHP project for the Raspberry
is avalilable here https://github.com/gcassarino/jquery-mobile-Philips-RC6-tv-remote

I rewrote the PHP backend in C to work on the small microcontroller esp8266 using the IRSend library as the equivalent of LIRC used in the Raspberry project. It all started as a challenge to understand if it was possible to "easily" bring the functionality of my TV infrared remote control from the Raspberry version to a microcontroller with wifi functionality. The inevitable choice was the esp8266 and thanks to the excellent library IRremoteESP8266 and using FSBrowserNG as framework for serving web pages I managed to replicate the functionality of the Raspberry more efficiently and without too much difficulties. To convert RC6 from LIRC to IRsend I referred to Ken Shirriff's instructions here: http://www.righto.com/2010/12/64-bit-rc6-codes-arduino-and-xbox.html

I made some small changes to the library of Germán Martín: now all the files of the administrative backend are placed a single directory "admin" to which the relative access permissions are assigned. This is to obtain a better separation of the files of the system from those of the various applications. The other applications reside in their specific folders. Furthermore it is possible to log in and log out from the administrative area. Two variants of the remote control can be used without the need for an Internet connection and directly from the Access Point mode as they do not use external libraries.

Here are some screenshot of the TV remote UI and the server admin backend:

| ![TV Remote home](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/TV-Remote-ESP8266-home.png) | 
|:--:| 
| *TV Remote home* |
 
| ![Administration menu](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/ESP8266-admin-home.png) | 
|:--:| 
| *Administration menu* |

| ![Philips TV remote](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/Philips-TV-Remote-22PFT4000_12-full.png) | 
|:--:|
| *Philips TV remote using Jquery/Jquery Mobile (loaded from external website, needs a connection)* |

The full version mimics the same look and functionalities of the real TV remote (plus the macros at the bottom).


| ![Philips TV remote light](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/Philips-TV-Remote-light.png) | 
|:--:|
| *Philips TV remote light version has the same functionalities as the above version, with the exception of the long press of the keys, present only in the JQuery Mobile version* |

| ![Philips TV remote custom](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/Philips-TV-Remote-custom-night-mode.png) | 
|:--:|
| *Philips TV remote custom in night mode* |

| ![Philips TV remote custom edit mode open](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/Philips-TV-Remote-custom-edit-mode.png) | 
|:--:|
| *Philips TV remote custom with Edit Mode open* |

The "custom" version allows you to customize the remote control keys: you can define the position of the key in the board, the function, the color and the text above the key. It is also possible to create sequences of multiple keys to allow more structured actions such as accessing the HDMI ports or setting the sleep timer which are usually operations that require the pressure of a sequence of keys. There is also a visual feedback regarding the sending of the command, green means ok, red means problems (no command sent). 

All web page layouts are fully responsive and accessible from smartphones and tablets.

## Todo
- dump the keyboard map to save the remote control configurations and then reload later or as a backup in case of reset.

## Notes
- I use version 2.0 of the ntpclient library instead of version 2.5.
- As per the original application, resetting the card will lose all previous settings.
- Since I'm not an expert C programmer and given my great ability to forget things, the code is widely commented where modified by me. Well, I'm not a C programmer at all... Just having fun.


## Getting Started

This project was tested on a Wemos D1 mini clone, please refer to this scheme for wiring connections

![Fritzing scheme](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources/blob/master/esp8266-Philips-RC6-tv-remote_schem.png)


### Installing

This code is a PlatformIO project, open the project from PlatformIO.


## Authors

* [Gianluca Cassarino](https://github.com/gcassarino)


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details

## Acknowledgments/Credits

* FSBrowserNG by Germán Martín [(gmag11)](https://github.com/gmag11)
* Lee H-W [(species5618)](https://github.com/species5618) for writing several useful additional features for FSBrowserNG
* ESPAsyncTCP by Hristo Gochkov [(me-no-dev)](https://github.com/me-no-dev)
* Ken Shirriff's [blog](http://www.righto.com/)
* IRremoteESP8266 by Ken Shirriff, Sebastien Warin, Mark Szabo, Roi Dayan, David Conran, Massimiliano Pinto (https://platformio.org/lib/show/1089/IRremoteESP8266)
* Please look at the [resources related repository](https://github.com/gcassarino/esp8266-Philips-RC6-tv-remote-resources) for the Fritzing scheme and screenshots.

NOTE: This is currently work in progress, a lot of messages come out from the serial console.
