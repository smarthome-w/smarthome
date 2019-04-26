#!/usr/bin/env python3
import broadlink
import time
import sys
import os
from ruamel.yaml import YAML

if len(sys.argv) != 3:
    print("Script call: <name> <switch_item_name> <switch_item_status>")
    sys.exit(1)

switch_item_name = sys.argv[1]
switch_item_status = sys.argv[2]

data_dir = os.path.dirname(os.path.abspath(__file__))
data_dir = os.path.abspath(os.path.dirname(
    data_dir + "/../../broadlink-data/"))
#print (data_dir)

content = open(data_dir + "/sp2.yml", "r").read()
yaml = YAML()
mapping_dictionary = yaml.load(content)

mac = mapping_dictionary["mapping_dictionary"][switch_item_name]["MAC"]
ip = mapping_dictionary["mapping_dictionary"][switch_item_name]["IP"]

operations = ""
operations += "start"
try:
    device = broadlink.sp2(
        host=(ip, 80), mac=bytearray.fromhex(mac), devtype="sp2")

    operations += ", connecting"
    device.auth()
    operations += ", connected"
    device.host
    if switch_item_status == "ON":
        device.set_power(True)
    if switch_item_status == "OFF":
        device.set_power(False)
    if switch_item_status == "STATUS":
        device_power = device.check_power()
        operations += ", power:+++{}+++".format(device_power)

    operations += ", ==sent=="
except Exception as e:
    print(e)

print("{}: {}".format(switch_item_name, operations))
