# Smarthome

This GitHub project was created because of two reasons:

- to have backup of my code on Github
- to share ideas and solutions present in my home.

Code in this repository is not a library or product. It is just dump of specific ***working*** installation
operational from my perspective. Of course sensitive information like IP's, MAC's, SSID, passwords is hidden.

Instead of having blog I've decided to describe ideas and my solution here to put all code and documentation together.

## Legacy

- Old alarm installation with several PIR sensors cabled to the one point (not used, not ready for smart home)
- Junction boxes to the upper lights (no direct power available on direct switches)

## Resources

One bored and IT experienced guy with simple tools.

## Transformation principles

- Core smart functions have to be accessible without connection to the Internet
- Limit external *smart* services to absolute minimum (IFTTT, Stringify and other on black list)
  - reduce impact of service unavailability
  - say no to dozens of accounts in *smart* services
- No battery power. One of the dozens of 5V power supplies present at home is sufficient enough.
- Cable if possible
- Smart functions should be an augmentation of standard functions.
- Maximize security (see also *smart* services)
- Reduce costs, maximize safety
- Maximize realiability and accessibility
- Maximize fun, use simple tools for real DIY. No 3D printings and other advanced technologies.

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
- *Wow effect*

## Result achieved

There are not only dozens of sensors installed at home. There are several _smart_ functions which make life easier:

- Automated blinds controlled manually and via rules (down if Sun is low above the horison and light sensor detects dusk; up if Sun is above the horison and light sensor detects dawn)
- Supporting lights (led strips) powered on when motion is detected (from dusk to dawn)
- _Good night_ activities
- Did I forgot to close windows at home? I know it.

Have fun and  start from [Transformation to smart house](/docs/README.md).

_Disclaimer_: Labels are mixed: Polish and English. It's because Google Home Mini currently does not support Polish.
