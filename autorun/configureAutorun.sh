#!/bin/sh
# Sets up automatic testing suite running on new installation of Debian
# This script should be run one time, with sudo priviliges, to set up
# a new copy of the testing suite code only.
echo "Starting configuration of testing suite autorun service..."
sudo cp ./autorun-TS.service /etc/systemd/system/autorun-TS.service
sudo systemctl daemon-reload
sudo systemctl enable autorun-TS
sudo systemctl start autorun-TS
echo "Testing suite autorun configuration completed."
