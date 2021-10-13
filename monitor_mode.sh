#!/bin/bash

systemctl stop NetworkManager.service wpa_supplicant.service alsa-restore.service
airmon-ng start wlp0s2

read  -n 1 -p "Press Key to disable Monitor:" mainmenuinput

airmon-ng stop phy0.mon
systemctl start NetworkManager.service wpa_supplicant.service alsa-restore.service
exit
