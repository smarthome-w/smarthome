CREATE USER IF NOT EXISTS 'openhab'@'localhost' IDENTIFIED BY 'openhab';

CREATE USER  IF NOT EXISTS 'openhab'@'%' IDENTIFIED BY 'openhab';

GRANT ALL PRIVILEGES ON OpenHAB.* TO 'openhab'@'localhost';

GRANT ALL PRIVILEGES ON OpenHAB.* TO 'openhab'@'%';

FLUSH PRIVILEGES;
