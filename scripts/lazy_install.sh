#/bin/bash

sudo ./root_install_first.sh
sudo ./install_latex.sh
sudo usermod -a -G dialout $USERNAME
sudo usermod -a -G plugdev $USERNAME
./user_install_second.sh
