#/bin/bash

sudo ./root_install_first.sh
sudo usermod -a -G dialout $USERNAME
sudo usermod -a -G plugdev $USERNAME
./user_install_second.sh
