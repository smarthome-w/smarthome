#!/usr/bin/env python3
import broadlink
import time
import sys
import os
import codecs
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

content = open(data_dir + "/system.yml", "r").read()
yaml = YAML()
system_dictionary = yaml.load(content)

content = open(data_dir + "/ircolors.yml", "r").read()
yaml = YAML()
mapping_dictionary = yaml.load(content)

code = mapping_dictionary["mapping_dictionary"]["RGB_" +
                                                switch_item_status]["COLOR"]

operations = ""
operations += "start"

try:
    # settings
    device = broadlink.rm(host=(system_dictionary["HW_RMPro"], 80), mac=bytearray.fromhex(
        system_dictionary["HW_RMPro_MAC"]), devtype="rm")

    operations += ", connecting"
    device.auth()
    operations += ", connected"
    device.host
    for i in range(5):
        decode_hex = codecs.getdecoder("hex_codec")
        device.send_data(decode_hex(code)[0])
    operations += ", ==sent=="
except Exception as e:
    print(e)

print("{}: {}".format(switch_item_name, operations))
