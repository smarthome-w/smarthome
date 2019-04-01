#!/usr/bin/env python3

from sonybraviaremote import TV, TVConfig
import sys
import os
from ruamel.yaml import YAML

def on_auth():
    return input('Pincode: ')

if len(sys.argv) != 3:
    print ("Script call: <name> <switch_item_name> <switch_item_status>")
    sys.exit(1)

switch_item_name = sys.argv[1]
switch_item_status = sys.argv[2]

data_dir = os.path.dirname(os.path.abspath(__file__))
data_dir = os.path.abspath(os.path.dirname(data_dir + "/../../broadlink-data/"))
#print (data_dir)

content = open(data_dir + "/sony.yml", "r").read()
yaml = YAML()
system_dictionary = yaml.load(content)

operations=""
operations += "start"

try:
    config = TVConfig(system_dictionary["sony_ip"], 'sony')
    tv = TV.connect(config, on_auth)

    if (switch_item_name == "RTV_VOLUME_UP"):
        tv.volume_up(2)
    if (switch_item_name == "RTV_VOLUME_DOWN"):
        tv.volume_down(2)
    if (switch_item_name == "RTV_VOLUME_MUTE"):
        tv.mute()
    if (switch_item_name == "RTV_TV_NETFLIX"):
        tv.netflix()
        tv.confirm()
    if (switch_item_name == "RTV_TV_YOUTUBE"):
        tv.youtube()
    if (switch_item_name == "RTV_TV_CONFIRM"):
        tv.confirm()
    operations+=", ==sent=="

except Exception as e:
    print (e)

print("{}: {}".format(switch_item_name, operations))
