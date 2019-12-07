# OpenHAB installation

## System prerequisities

Install process is optimized for OSMC distribution. Most of the routines are common for all Raspberry Pi distributions.

### Prerequisities

- clone repository and set environment value

```bash
export SMART_HOME=<repo_dir>
```

- create `openhab` user:

```bash
  sudo useradd openhab
  sudo usermod -u 9001 openhab
  sudo groupmod -g 9001 openhab
```

- create `openhab` directories:

```bash
  sudo mkdir -p /opt/openhab
  sudo ln -s ${SMART_HOME}/src/conf /opt/openhab/conf
  sudo chown -R openhab:openhab /opt/openhab
```

- provide symbolic hostname by adding line to `/etc/hosts`

```bash
<ip_address> openhab.master
```

- install `make` package:

```bash
sudo apt-get install make
```

- install docker:

  - osmc

```bash
cd conf/deploy/docker-install && sudo ./get-docker-sh
```

- raspbian

```bash
sudo apt-get install docker.io
```

_Disclaimer_ Original script was altered by proper OSMC detection.

- add user to docker group according to message at the end of the docker installation

## Core packages install

```bash
cd $SMART_HOME/conf/deploy && sudo make openhab-core
```

## Create OpenHAB database and configure mariadb server

- run `sudo mysql -u root` and run commands from file `$SMART_HOME/deploy/resources/sql/database.sql`
- edit `/etc/mysql/my.cnf` file and add section:

```bash
[mysqld]
bind-address = 0.0.0.0
```

_This is because OpenHAB is started in Docker, so client address is different than localhost._

- restart mariadb server:

```bash
sudo systemctl restart mariadb
```

## OpenHAB install

```bash
cd $SMART_HOME/conf/deploy && sudo make openhab-all
```

## OpenHAB services

- run

```bash
cd $SMART_HOME/conf/deploy && sudo make pi-all
```

## USB camera and motion package

- edit `/etc/default/motion` and change value

```bash
start_motion_daemon=yes
```

- alter original config

```bash
cp $SMART_HOME/conf/deploy/resources/motion/motion.conf /etc/motion/motion/conf
```

## Raspbian buster settings

### /boot/config.txt

```bash
dtparam=watchdog=on
dtoverlay=pi3-disable-bt
hdmi_ignore_cec_init=1
```

### bluetooth

In `/etc/systemd/system/bluetooth.target.wants/bluetooth.service` change:

```bash
ExecStart=/usr/lib/bluetooth/bluetoothd --noplugin=sap
```

Packages for iNode:

## iNode packages and settings

```bash
apt-get install bluez-hcidump bc
```

## EQ3 settings

crontab:

```bash
0/10 * * * /opt/openhab/conf/scripts/EQ3/eq3.sh
```
