# OpenHAB installation

## System prerequisities

Install process is optimized for OSMC distribution. Most of the routines are common for all Raspberry Pi distributions.

### Prerequisities

- clone repository and set environment value

```$xslt
export SMART_HOME=<repo_dir>
```

- create `openhab` user:

```$xslt
  sudo useradd openhab
  sudo usermod -u 9001 openhab
  sudo groupmod -g 9001 openhab
```

- create `openhab` directories:

```$xslt
  sudo mkdir -p /opt/openhab
  sudo ln -s ${SMART_HOME}/src/conf /opt/openhab/conf
  sudo chown -R openhab:openhab /opt/openhab
```

- provide symbolic hostname by adding line to `/etc/hosts`

```$xslt
<ip_address> openhab.master
```

- install `make` package:

```$xslt
sudo apt-get install make
```

- install docker:

```$xslt
cd conf/deploy/docker-install && sudo ./get-docker-sh
```

_Disclaimer_ Original script was altered by proper OSMC detection.

- add user to docker group according to message at the end of the docker installation

## Core packages install

```$xslt
cd $SMART_HOME/conf/deploy && sudo make openhab-core
```

## Create OpenHAB database and configure mariadb server

- run `sudo mysql -u root` and run commands from file `$SMART_HOME/deploy/resources/sql/database.sql`
- edit `/etc/mysql/my.cnf` file and add section:

```$xslt
[mysqld]
bind-address = 0.0.0.0
```

_This is because OpenHAB is started in Docker, so client address is different than localhost._

- restart mariadb server:

```$xslt
sudo systemctl restart mariadb
```

## Check port availability

- Edit `~/.kodi/userdata/guisettings.xml` and change `webserverport` to free port 80.

## OpenHAB install

```$xslt
cd $SMART_HOME/conf/deploy && sudo make openhab-all
```

## OpenHAB services

- run

```$xslt
cd $SMART_HOME/conf/deploy && sudo make pi-all
```

## USB camera and motion package

- edit `/etc/default/motion` and change value

```$xslt
start_motion_daemon=yes
```

- alter original config

```$xslt
cp $SMART_HOME/conf/deploy/resources/motion/motion.conf /etc/motion/motion/conf
```

## Raspberry Pi LIRC

- configure lirc

```$xslt
sudo /usr/share/lirc/lirc-old2new
```
