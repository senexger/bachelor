#!/bin/bash

systemctl stop NetworkManager.service wpa_supplicant.service alsa-restore.service
airmon-ng start wlp0s20f0u1

read  -n 1 -p "Press Key to disable Monitor:" mainmenuinput

airmon-ng stop wlp0s20f0u1mon
systemctl start NetworkManager.service wpa_supplicant.service alsa-restore.service
exit
