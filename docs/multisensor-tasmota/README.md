# Multisensor on Wemos D1 mini and Tasmota

## Pinout

| Wemos D1 | GPIO |    Tasmota    | Sensor  |
| :------: | :--: | :-----------: | :-----: |
|    D1    |  5   |  I2C SCL (6)  | BME280  |
|    D2    |  4   |  I2C SDA (5)  | BME280  |
|    D5    |  14  | Switch 2 (9)  |  AM312  |
|    D6    |  12  | Switch 2 (10) | contact |

## Tasmota template

```bash
{"NAME":"Multisensor","GPIO":[255,255,255,255,6,5,255,255,9,255,10,255,255],"FLAG":1,"BASE":18}
```

### Tasmota template GPIO order

```bash
GPIO# |00| 01|02| 03|04| 05| 09| 10| 12| 13| 14| 15| 16|
```

## Tasmota rules

```bash
SwitchMode1 1
SwitchTopic 0
Rule1 on switch1#state=1 do publish stat/multisensor01/CIR ON endon on switch1#state=0 do publish stat/multisensor01/CIR OFF endon
Rule1 1

SwitchMode2 1
SwitchTopic 0
Rule2 on switch2#state=1 do publish stat/multisensor01/PIR ON endon on switch2#state=0 do publish stat/multisensor01/PIR OFF endon
Rule2 1
```
