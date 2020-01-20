CREATE USER IF NOT EXISTS 'openhab'@'localhost' IDENTIFIED BY 'somepass';

CREATE USER  IF NOT EXISTS 'openhab'@'%' IDENTIFIED BY 'somepass';

GRANT ALL PRIVILEGES ON OpenHAB.* TO 'openhab'@'localhost';

GRANT ALL PRIVILEGES ON OpenHAB.* TO 'openhab'@'%';

FLUSH PRIVILEGES;
