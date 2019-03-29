# Transformation to smart house

## Hardware building blocks

Main assumption for hardware is alternative libraries accessibility (any shell, python api).

|Element|Function|Pros|Cons|Product|
|:-------:|:--------:|:-------:|:-------:|:-------:|
|Rasbperry Pi 3B|Automation server|Linux based, easy to maintain|-|-|
|Broadlink RM 3 Pro+|Universal IR and RF transmitter|-|-|-|
|Broadlink TC2|Light switch|RF controlled (no WiFi range problems), nice outlook|***Lack of current state check***|-|
|Broadlink SP2|Remote WiFi switch|Brighten manual toggle|Lack of manual network setup|-|
|Shelly 1|Remote WiFi switch|-|-|-|
|iNodeMeter|Power meter (optical version)|Linux based scripts for automation|Problems with range. Be ready to buy additional USB bluetooth dongle and do additional wire or additional Ethernet expander.|[iNode Meter](https://inode.pl/iNode-Energy-Meter-opto,p,27)|
|ESP8266 (nodeMCU)|Universal WiFi building block|Embedded UART, 5V power supply|-|-|
|IR LED Controller (44 keys version)|RGB LED strip controller|-|-|[IR RGB Dimmer](https://www.aliexpress.com/wholesale?SearchText=RGB+44+keys+remote+dimmer)|
|EQ3 Bluetooth|Programmable Thermostat|-|-|-|
|Google Home Mini|Voice control|-|-|-|

*Disclaimer*: I have router from my ISP without ability to match MAC and IP address. It requires additional attention and hacks. So ability to manually setup network is important to me

### What if discussion

#### Broadlink TC2

- It looks pretty well
- There is no availability problem due to RF transmission (RM Pro is required).
- ***Does not require separate power, so it is perfect for installation with separated junction box for lights***
- ***But there is no chance to retrieve current state. If it is changed manually you have no chance to detect it***
- There's no good alternative: Shelly 1 in the junction box and standard switches, Sonoff is not solution for my electrical installation.

#### Broadlink SP2

- It has physical, highlighted toogle
- Does not requires dedicated electrical contact, you can use cables to attach
- Reasonable in hidden places
- Alternatives: Any RF switch with RM Pro+ routines
- DIY block with toggle: parts costs (we have here 230V) like ESP with shield, 230V-5V stepdown supply with good quality without fire risk, chassis which keeps my fingers away from 230V - it is not cheaper than SP2 or any other.

#### Shelly 1

- I've used it only for cross junction on corridor
- Reasonable alternative for my junction box based light installation if I stay with existing mechanical switches.

## IT Architecture

## Hardware products delivered by project

### Automation server

Raspberry Pi with some software and hardware components:

- OSMC distribution (KODI ready as a secondary function)
- OpenHAB on Docker
- Additional ecosystem components: MySQL, MQTT server
- VPN Server (for connectivity)

### Universal sensor

ESP8266 (nodeMCU) based sensor with optional functions:

- external temperature (DS18B20)
- internal temperature (DHT22)
- humidity (DHT22)
- pressure (BMP280)
- PIR
- CIR
- light intensity detector (photoresistor)

### Blind controller

- Servo with continuous rotation fix (MG995)
- mechanical limit switch (one)
- legacy blinds
- ESP8266 (nodeMCU) driver

### PIR/CIR controller

- ESP8266 (nodeMCU) based circuit open/close state controller
