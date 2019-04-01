CREATE DATABASE IF NOT EXISTS OpenHAB;

CREATE USER 'openhab'@'localhost' IDENTIFIED BY 'somepass';

CREATE USER 'openhab'@'%' IDENTIFIED BY 'somepass';

GRANT ALL PRIVILEGES ON OpenHAB.* TO 'openhab'@'localhost';

GRANT ALL PRIVILEGES ON OpenHAB.* TO 'openhab'@'%';

FLUSH PRIVILEGES;
