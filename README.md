# ZiFi
Wifi hacking tool using ESP8266 ( Evil-Twin method )

<p align="center">
<a href="https://github.com/sankethj/z-cam/"><img title="Tool" src="https://img.shields.io/badge/Tool-ZiFi-blue.svg?style=for-the-badge"></a>
<a><img title="Version" src="https://img.shields.io/badge/Version-1.0-blue.svg?style=for-the-badge"></a>
<a><img title="Maintainence" src="https://img.shields.io/badge/Maintenance-Yes-blue.svg?style=for-the-badge"></a>
</p>

![logo](/Images/ZiFi.png)

## FEATURES :
* [+] Deauth
* [+] Evil-Twin
* [+] User Interface

## TESTED ON :
* Nodemcu
* Probably will work in all-other boards too...

## INSTALLATION :
* Install [Arduino IDE](https://www.arduino.cc/en/software)
* Add Esp8266 in Additional Board Manager `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
* Esp8266 board  [NODEMCU](https://www.amazon.in/dp/B010O1G1ES/ref=cm_sw_r_apan_glt_i_MAFEQVVXSRR69JXNYFA3)
* Install your board, choose correct Port.
* Compile and Upload.

## CONCEPT :
* Connect to the Access Point named `ZiFi` with password `Eviltwin` from your device.
* Select the target you want (list of available APs refreshes every 15secs - page reload is required).
* Select The Attack Mode. If you choose Deauth it will start deauthing the clients in that network.
* Then Start Evil-twin attack, which will create the clone of the selected network.
* The web interface will be unavailable during Evil-twin attack mode, You need to reconnect.
* Reconnect after some time, it will display you the correct password in Result section.

## DEMONSTRATION VIDEO:
[![YOUTUBE](/Images/ZiFi_yt.PNG)](https://youtu.be/tMtVHLnd8XE)

## CREDITS :
* [M1z23R](https://github.com/M1z23R)  --> MODIFIED VERSION OF HIS SCRIPT
* [Spacehuhn Technologies](https://github.com/SpacehuhnTech)   --> TEMPLATE
* [125K](https://github.com/125K)     --> TEMPLATE

## WARNING :
Use it only against your own networks and devices!
Please check the legal regulations in your country before using it.
We don't take any responsibility for what you do with this program.
