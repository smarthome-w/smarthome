# Smarthome

This GitHub project was created because of two reasons:

- to have backup of my code on Github
- to share ideas and solutions present in my home.

Code in this repository is not a library or product. It is just dump of specific **_working_** installation
operational from my perspective. Of course sensitive information like IP's, MAC's, SSID, passwords is hidden.

Instead of having blog I've decided to describe ideas and my solution here to put all code and documentation together.

## Legacy

- Old alarm installation with several PIR sensors cabled to the one point (not used, not ready for smart home)
- Junction boxes to the upper lights (no direct power available on direct switches)

## Resources

One bored and IT experienced guy with simple tools.

## Transformation principles

- Core smart functions have to be accessible without connection to the Internet
- Limit external _smart_ services to absolute minimum (IFTTT, Stringify and other on black list)
  - reduce impact of service unavailability
  - say no to dozens of accounts in _smart_ services
- No battery power. One of the dozens of 5V power supplies present at home is sufficient enough.
- Cable if possible
- Smart functions should be an augmentation of standard functions.
- Maximize security (see also _smart_ services)
- Reduce costs, maximize safety
- Maximize realiability and accessibility
- Maximize fun, use simple tools for real DIY.
- ~~No 3D printings and other advanced technologies.~~ I have Ender 3 now and it is awesome.
- Minimize usage of the devices/technologies which require to use external services.

## Transformation

![Architecture](/docs/images/arch-diagram.png)

## Smart house features control list

- Control lighting
- Control movement, doors, windows
- Control blinds
- Control power consumption
- Control heating
- Control multimedia (TV, soundbar)
- Smartphone and voice controlled

## Transformation goals

- Wife's smile
- Nice place to live
- _Wow effect_

## Result achieved

There are not only dozens of sensors installed at home. There are several _smart_ functions which make life easier:

|               Feature               |                                     Elements                                     |                                                                                                    Description                                                                                                     |                                                                                                   Example                                                                                                    |
| :---------------------------------: | :------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|    External environment control     |             Temperature, pressure, light sensors, astronomical data              | Ability to recognize the external environment status and calculate key attributes for automation: is it dusk, dawn, working day, should be supportive lights activated or deactivated, should blinds be down or up |                              Activate supporting lights only between 17:00 and 7:00 when Sun is no less than 3 degrees above the horizon (all digital values are parametrized)                               |
|    Internal environment control     |                               Temprature, humidity                               |                                                                                       Ability to view data and control them                                                                                        |                                                                                                      -                                                                                                       |
| Monitor elements of apartment/house |        Motion sensors, circuit sensors (windows, entry door, entry lock)         |                                                                   Ability to control elements of the house and usage for more complex scenarios                                                                    |                                                                  Activate entry lights (only) when entry doors are opened and you are home                                                                   |
|            Light scenes             |                          Light switches, power switches                          |                                                                                 Ability to control lights individually and in sets                                                                                 |                                                                           Activate supporting lights in family room and tv lights                                                                            |
|          Supporting lights          |                    Motion sensors, power switches with lights                    |                                                                Ability to automatically turn the supporting lights on in trigger value is achieved                                                                 |                                                          LED strips in the kitchen, corridor, on the stairs activated on move at evening and night                                                           |
|            Heating view             |                               Smart heating valves                               |                                                                      Ability to control valve state and temperature settings in smart valves                                                                       |                                                                                                      -                                                                                                       |
|           Blinds control            |                            Blinds with DYI automation                            |                                                Ability to set blinds completly open, close and partially (percentage) open using any trigger present in the system                                                 |                                                          Scroll blinds completly down if there is dust (in terms of previouisly defined condition)                                                           |
|             Power view              |                                   Power meter                                    |                                                                                Ability to control overall power consumption at home                                                                                | Enables ability to detect, that some device with high power consumption is activated (oven, iron, kettle, wasching machine). Priceless if you are out of home and you forget if some device was deactivated. |
|            Lock control             |                          Microswitch in lock (circuit)                           |                                                                                        Check if entry lock is closed/opened                                                                                        |                                                                                                      -                                                                                                       |
|        Entertaiment control         | IR/RF universal controller (Broadlink or similar), access to device API via http |                                                                                       Ability to control multimedia devices                                                                                        |                                                                                         Turn streaming services on.                                                                                          |
|           Parametrization           |                   Parametrization of OpenHAB and items, rules                    |                                                                             Ability to change home behavior without additional coding                                                                              |                                                               Set demo mode (all features on), set astronomical triggers, hours triggers, ...                                                                |

Have fun and start from [Transformation to smart house](/docs/README.md).

_Disclaimer_: Labels are mixed: Polish and English. It's because Google Home Mini currently does not support Polish.
