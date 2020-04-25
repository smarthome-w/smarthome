# Multisensors

## Tasmota template GPIO order

```bash
GPIO# |00| 01|02| 03|04| 05| 09| 10| 12| 13| 14| 15| 16|
```

## Multisensor on Wemos D1 mini and Tasmota

### Pinout

| Wemos D1 | GPIO |    Tasmota    | Sensor  |
| :------: | :--: | :-----------: | :-----: |
|    D1    |  5   |  I2C SCL (6)  | BME280  |
|    D2    |  4   |  I2C SDA (5)  | BME280  |
|    D5    |  14  | Switch 1 (9)  |  AM312  |
|    D6    |  12  | Switch 2 (10) | contact |

### Tasmota template

```bash
{"NAME":"Multisensor","GPIO":[255,255,255,255,6,5,255,255,9,255,10,255,255],"FLAG":1,"BASE":18}
```

### Tasmota rules

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

### Hardware

- multisensor01 - Bedroom sensor (01)
- multisensor02 - Kitchen sensor (02)
- multisensor04 - Alice room sensor (04)

## Multisensor on NodeMCU and Tasmota

### Pinout

| Wemos D1 | GPIO |   Tasmota   | Sensor  |
| :------: | :--: | :---------: | :-----: |
|    D1    |  5   | I2C SCL (6) | BMP280  |
|    D2    |  4   | I2C SDA (5) | BMP280  |
|    D4    |  2   | DS18x20 (4) | DS18B20 |
|    D7    |  13  | SI7021 (3)  |  DHT22  |

### Tasmota template

```bash
{"NAME":"Multisensor2","GPIO":[255,255,4,255,6,5,255,255,255,3,255,255,255],"FLAG":1,"BASE":18}
```

```bash
GPIO# |00| 01|02| 03|04| 05| 09| 10| 12| 13| 14| 15| 16|
```

### Hardware

- multisensor03 - Familyroom sensor (03)

## Multisensor on NodeMCU and Tasmota (2)

### Pinout

| Wemos D1 | GPIO |    Tasmota    | Sensor  |
| :------: | :--: | :-----------: | :-----: |
|    D4    |  2   |  DS18x20 (4)  | DS18B20 |
|    D5    |  14  | Switch 1 (9)  |  AM312  |
|    D6    |  12  | Switch 2 (10) | contact |
|    D7    |  13  |  SI7021 (3)   |  DHT22  |

### Tasmota template

```bash
{"NAME":"Multisensor3","GPIO":[255,255,4,255,255,255,255,255,10,3,9,255,255],"FLAG":1,"BASE":18}
```

```bash
GPIO# |00| 01|02| 03|04| 05| 09| 10| 12| 13| 14| 15| 16|
```

### Hardware

- multisensor05 - Ann room sensor (05)
