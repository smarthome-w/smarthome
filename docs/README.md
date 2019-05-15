# Transformation to smart house

## Hardware building blocks

Main assumption for hardware is alternative libraries accessibility (any shell, python api).

|               Element               |                   Function                   |                         Pros                         |                                                             Cons                                                             |                                          Product                                           |
| :---------------------------------: | :------------------------------------------: | :--------------------------------------------------: | :--------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------: |
|           Rasbperry Pi 3B           |              Automation server               |            Linux based, easy to maintain             |                                                              -                                                               |                                             -                                              |
|         Broadlink RM 3 Pro+         |       Universal IR and RF transmitter        |                          -                           |                                                              -                                                               |                                             -                                              |
|            Broadlink TC2            |                 Light switch                 | RF controlled (no WiFi range problems), nice outlook |                                              **_Lack of current state check_**                                               |                                             -                                              |
|              Shelly 1               |              Remote WiFi switch              |                          -                           |                                                              -                                                               |                                             -                                              |
|             iNodeMeter              |        Power meter (optical version)         |          Linux based scripts for automation          | Problems with range. Be ready to buy additional USB bluetooth dongle and do additional wire or additional Ethernet expander. |                [iNode Meter](https://inode.pl/iNode-Energy-Meter-opto,p,27)                |
|          ESP8266 (nodeMCU)          |        Universal WiFi building block         |            Embedded UART, 5V power supply            |                                                              -                                                               |                                             -                                              |
| IR LED Controller (44 keys version) |           RGB LED strip controller           |                          -                           |                                                              -                                                               | [IR RGB Dimmer](https://www.aliexpress.com/wholesale?SearchText=RGB+44+keys+remote+dimmer) |
|        Equiva eQ3 Bluetooth         |          Smart radiator thermostat           |                          -                           |                                                              -                                                               |                                             -                                              |
|          Google Home Mini           |               Voice controller               |                          -                           |                                                              -                                                               |                                             -                                              |
|            Sonoff Basic             |     Universal WiFi switch building block     |                          -                           |                                                              -                                                               |                                             -                                              |
|            Broadlink SP2            |              Remote WiFi switch              |                Brighten manual toggle                |                                     Lack of manual network setup, no native MQTT support                                     |                                             -                                              |
|         ESP32 (in progress)         | Universal WiFi building block with Bluetooth |            Embedded UART, 5V power supply            |                                                              -                                                               |                                             -                                              |

_Disclaimer_: I have router from my ISP without ability to match MAC and IP address. It requires additional attention and hacks. So ability to manually setup network is important to me

### What if discussion

#### Broadlink TC2

- It looks pretty well
- There is no availability problem due to RF transmission (RM Pro is required).
- **_Does not require separate power, so it is perfect for installation with separated junction box for lights_**
- **_But there is no chance to retrieve current state. If it is changed manually you have no chance to detect it_**
- There's no good alternative: Shelly 1 in the junction box and standard switches, Sonoff is not solution for my electrical installation.

#### Broadlink SP2

- It has physical, highlighted toogle
- Does not requires dedicated electrical contact, you can use cables to attach
- Reasonable in hidden places
- No native MQTT support
- Alternatives: Any RF switch with RM Pro+ routines
- Funny alternative: [Sonoff with capacity button](https://www.youtube.com/watch?v=B38e1HAMUXs)
- DIY block with toggle: parts costs (we have here 230V) like ESP with shield, 230V-5V stepdown supply with good quality without fire risk, chassis which keeps my fingers away from 230V - it is not cheaper than SP2 or any other.

#### Shelly 1

- I've used it only for cross junction on corridor
- Reasonable alternative for my junction box based light installation if I stay with existing mechanical switches.

## IT Architecture

![Architecture](/docs/images/arch-diagram.png)

Solution consists of two parts:

- `core` - maximum availability, no access to external services, all processed within local network
- `external visibility` - ability to control system via external services (voice from Google), availability depends on myOpenHAB and Google services metrics

## Hardware products delivered by project

### Automation server

Raspberry Pi with some software and hardware components:

- OSMC distribution (KODI ready as a secondary function)
- OpenHAB on Docker
  - [Installation process](server-deployment/README.md)
  - [Most important software features](/docs/software-features/README.md)
- Additional ecosystem components: MySQL, MQTT server
- VPN Server (for connectivity)

#### Why OpenHAB

- I hate _configuration file programming_ and _configuration UI programming)_
- OpenHAB provides ability to store all configuration in text files
- Rules (mini-programs) are written in Java-oriented script language
- Direct access to server OS from rule (i.e. to run own scripts)

### Universal sensor

ESP8266 (nodeMCU) based sensor with optional functions:

- external temperature (DS18B20)
- internal temperature (DHT22, BME280)
- humidity (DHT22, BME280)
- pressure (BMP280, BME280)
- PIR - detect opening/closure the circuit in sensor
- CIR - detect opening/closure the circuit
- light intensity detector (photoresistor)
- [Arduino sketch](/src/conf/arduino/multisketch.sketch/multisketch.sketch.ino)

#### Future plans for universal sensor

- Migration to ESP32 to handle Equiva eQ3 control with Bluetooth LE transmission
- Migration from DHT22 (it's a unstable junk) to BME280

### Blind controller

- Servo with continuous rotation fix (MG995)
  - PWM modulation to turn left or right the servo mechanism
  - different number of PWM impulses for left/right (up/down) due to resistance to movement - values are calculated in driver
- mechanical limit switch (one)
- legacy blinds and mechanical modifications

  - M8 screw with hexagon head mounted on the axis of existing mechanism
  - servo handles from aluminum (C-shapes)
  - mechanical limit switch mounted to the mechanism on a piece of hard plastic

- ESP8266 (nodeMCU) driver entry signals
  - manual up and manual down - the highest priority, limit switch is ignored
  - virtual up and virtual down (UP, DOWN) - the highest priority, limit switch is ignored
  - virtual stop (STOP) - stops the movement
  - automatic up and automatic down (AUTOUP, AUTODOWN) - set to the minimum or maximum position
  - perentage value - set to specific value
  - debug - share all important values to assess driver and mechanism status
- [Arduino sketch](/src/conf/arduino/multisketch.sketch/multisketch.sketch.ino)

![Blind preparation](images/blind-preparation.png)
![Blind completed](images/blind-completed.png)
![Blind mounted](images/blind-mounted.png)

#### Mechanical issues discussion

I have two blinds (250 cm and 180 cm) with roller's diameter 32 mm. Typical motor for blinds are mounted in roller with 40 mm diameter and are much more expensive (servo with cost about 7$, motor with cost 40$ + cost of roller upgrade to 40 mm).
In first version of mechanism I've replaced one part with home made (a piece of steel from market with one whole for axis) and home made roller plug.
After some time I purchased second roller. In this one roller was attached on aluminum chassis. It is much more stable, so I've decided to upgrade my first mechanism for less than 15\$.

To join servo and roller axis I've used a piece of steel wire nailed on the screw. You have to just do one hole in screw's head. That's enough. Screw is mounted to the original mechanism by cotter. No epoxy glue, mechanism can be dissassembled and assembled without problems.

#### Limit detection discussion

Limit detection is MUST BE. Using servo you cannot predict real position after some time.

First generation of limit switch (I've installed both: upper and bottom) was based on magnetic door switch. It was not reliable and after some time I've had problems with proper state detection. Using stronger magnet was not good - reed was failing due strong magnetic field.

Roller encoders (the cheapest one was about 1.5\$ and accuracy 1/20 of rotation). But you must have direct access to the axis (I have not) or build a kind of gear between servo and encoder.

The best solution for me is one mechanical limit switch on the top and position calculation. Typical scenario is that roller is on top position (with ability to restart position indicators) at least once a day. It is enough to have proper position detection accuracy.

#### Future plans for blind controller

- Limit servo noise

### PIR/CIR controller

- ESP8266 (nodeMCU) based circuit open/close state controller
- additional feature: detect entry as the combination of main door lock circuit and ground floor pir circuit states
- [Arduino sketch](/src/conf/arduino/multisketch.sketch/multisketch.sketch.ino)

### Common drivers features

- OTA driver upgrade
- Watchdog to detect WiFI network disconnection and restart device
- Heartbeat messages sent every minute
- Current functions present in each hardware piece is configurable in [devices.h](/src/conf/arduino/lib/devices.h)
