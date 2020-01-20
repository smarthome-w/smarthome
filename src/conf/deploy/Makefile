RESOURCES:=$(CURDIR)/resources

DOCKER_TAG=smarthomew-openhab

MACHINE=$(shell uname -m)

VERSION=2.5.0
OS_PLATFORM=amd64

HOST_IP=`hostname -I | awk '{print $$1}'`

ifeq ($(MACHINE),armv7l)
	OS_PLATFORM=armhf
endif

ifeq ($(OS_PLATFORM),)
	@echo Platform not supported
	exit 1
endif

openhab-init:
	@sudo useradd openhab
	@sudo usermod -u 9001 openhab
	@sudo groupmod -g 9001 openhab

openhab-core:
	@sudo apt-get install mariadb-client mariadb-server mosquitto mosquitto-clients moreutils -y
ifeq ($(OS_PLATFORM),armhf)
	@sudo apt install docker.io docker-compose -y
endif
	@sudo apt-get install python3 python3-dev python3-pip python3-venv gcc libffi-dev -y
	@sudo apt-get install logrotate vim motion git -y
	@sudo apt-get install usbutils -y

	@sudo pip3 install setuptools
	@sudo pip3 install wheel
	@sudo pip3 install paho-mqtt ruamel.yaml
ifeq ($(OS_PLATFORM),armhf)
	@sudo pip3 install py-sony-bravia-remote git+https://github.com/mjg59/python-broadlink.git
	@sudo pip3 install git+https://github.com/aparraga/braviarc.git
endif

openhab-db:
	# Prepare mysql connectivity from any network (important for docker)
	@sudo echo -e "[mysqld]\n" > /etc/mysql/conf.d/openhab.cnf
	@sudo echo -e "bind-address = 0.0.0.0" >> /etc/mysql/conf.d/openhab.cnf
	# Restart mariadb service
	@sudo systemctl restart mariadb

	@sudo mysql -u root -e "CREATE DATABASE IF NOT EXISTS OpenHAB;"
	@sudo mysql -u root OpenHAB < resources/sql/database.sql

#  /etc/default/motion start_motion_daemon=yes

openhab-docker-build: openhab-docker-clean
	docker pull openhab/openhab:$(VERSION)-$(OS_PLATFORM)-debian
	docker tag openhab/openhab:$(VERSION)-$(OS_PLATFORM)-debian $(DOCKER_TAG)	

openhab-resources-workspace:
	@mkdir -p $(RESOURCES)

openhab-docker-stop:
	-@docker rm -f $(DOCKER_TAG)

openhab-docker-clean:
	@docker image prune -f

pihole-service-install:
	@sudo cp $(RESOURCES)/service/pihole.service /etc/systemd/system/pihole.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable pihole

openhab-service-install:
	@sudo cp $(RESOURCES)/service/openhab.service /etc/systemd/system/openhab.service
	@sed -i "s/DOCKER_TAG/$(DOCKER_TAG)/g" /etc/systemd/system/openhab.service
	@sed -i "s/HOST_IP/$(HOST_IP)/g" /etc/systemd/system/openhab.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab

openhab-servo-service-install:
	@sudo cp $(RESOURCES)/service/openhab-servo.service /etc/systemd/system/openhab-servo.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab-servo
	@sudo systemctl restart openhab-servo

openhab-broadlink-mqtt-service-install:
	@sudo cp $(RESOURCES)/service/openhab-broadlink-mqtt.service /etc/systemd/system/openhab-broadlink-mqtt.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab-broadlink-mqtt
	@sudo systemctl restart openhab-broadlink-mqtt

openhab-sony-mqtt-service-install:
	@sudo cp $(RESOURCES)/service/openhab-sony-mqtt.service /etc/systemd/system/openhab-sony-mqtt.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab-sony-mqtt
	@sudo systemctl restart openhab-sony-mqtt

openhab-server-service-install:
	@sudo cp $(RESOURCES)/service/openhab-server.service /etc/systemd/system/openhab-server.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab-server
	@sudo systemctl restart openhab-server

openhab-powermeter-service-install:
	@sudo cp $(RESOURCES)/service/openhab-powermeter.service /etc/systemd/system/openhab-powermeter.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab-powermeter
	@sudo systemctl restart openhab-powermeter

openhab-camera-service-install:
	@sudo cp $(RESOURCES)/service/openhab-camera.service /etc/systemd/system/openhab-camera.service
	@sudo systemctl daemon-reload
	@sudo systemctl enable openhab-camera
	@sudo systemctl restart openhab-camera

openhab-service-restart:
	@sudo systemctl restart openhab
	@tail -f /opt/openhab/userdata/logs/openhab.log

openhab-info:
	@echo MACHINE=$(MACHINE)
	@echo OS_PLATFORM=$(OS_PLATFORM)
	@echo HOST_IP=$(HOST_IP)

openhab-develop: openhab-info openhab-docker-stop openhab-docker-build

openhab-basic: openhab-info openhab-docker-stop openhab-docker-build \
	openhab-service-install openhab-service-restart

openhab-all: openhab-info openhab-docker-stop openhab-docker-build \
	openhab-service-install openhab-broadlink-mqtt-service-install \
	openhab-sony-mqtt-service-install openhab-powermeter-service-install \
	openhab-server-service-install openhab-service-restart

# bmp280
# http://faradaysclub.com/?p=1325
# VIN 	Pin 1 (3.3V)
# Ground 	Pin 6
# SCK 	Pin 5 (SCL1)
# SDI 	Pin 3 (SDA1)
