#!/bin/bash

echo "start"

sudo apt update
sudo apt dist-upgrade -y
sudo apt install -y git
sudo apt autoremove -y

echo "end"
