# OpenHAB installation

## System prerequisities

Install process is optimized for OSMC distribution. Most of the routines are common for all Raspberry Pi distributions.

### Manual changes

- `/etc/hosts` - add line `<ip_address> openhab.master`
- install `make` package: `sudo apt-get install make`
- install docker: `cd /opt/openhab/conf/deploy/docker-install && sudo ./get-docker-sh`
- add user to docker group according to message at the end of the docker installation

## Core packages install

- `cd /opt/openhab/conf/deploy && sudo make openhab-core`

## Create OpenHAB database and configure mariadb server

- run `sudo mysql -u root` and run commands from file `/opt/openhab/deploy/resources/sql/database.sql`
- edit `/etc/mysql/my.cnf` file and add section:

```$xslt
[mysqld]
bind-address = 0.0.0.0
```

- restart mariadb server: `sudo systemctl restart mariadb`

## Check port availability

- Edit `~/.kodi/userdata/guisettings.xml` and change `webserverport` to free port 80.

## OpenHAB install

- run commands:

```$xslt
  sudo useradd openhab
  sudo usermod -u 9001 openhab
  sudo groupmod -g 9001 openhab
```

- run `cd /opt/openhab/conf/deploy && sudo make openhab-all`

## OpenHAB services

- run `cd /opt/openhab/conf/deploy && sudo make pi-all`

## USB camera and motion package

- edit `/etc/default/motion` and change value `start_motion_daemon=yes`
- run command: `cp /opt/openhab/conf/deploy/resources/motion/motion.conf /etc/motion/motion/conf`

## Raspberry Pi LIRC

- `sudo /usr/share/lirc/lirc-old2new`
