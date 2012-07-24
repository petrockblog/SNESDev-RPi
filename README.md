SNESDev-RPi
===========

Simulates a virtual keyboard for two SNES controllers that are connected to the GPIO pins of the Raspberry Pi.

Installation
------------

First of all, make sure that Git is installed:

```shell
sudo apt-get update
sudo apt-get install -y git
```

SNESDev needs the BCM 2835 library installed. It is downloaded and installed with

```shell
# install BCM library
echo "Installing BCM2835 library"
cd
wget http://www.open.com.au/mikem/bcm2835/bcm2835-1.3.tar.gz
tar -zxvf bcm2835-1.3.tar.gz
cd bcm2835-1.3
./configure
make
sudo make install
cd
rm bcm2835-1.3.tar.gz
```
SNESDev is downloaded and installed with

```shell
# install SNESDev
cd
git clone git://github.com/petrockblog/SNESDev-RPi.git
cd SNESDev-RPi
# make sure that old stuff is removed before compilation and linking
make clean
make
```

Running
-------

In order to run SNESDev mae sure that the uinput module is loaded. You can check this with

```shell
lsmod
```

The module is loaded with

```shell
modprobe uinput
```

If you want to have the uinput module automatically loaded, you can add "uinput" to the file 
/etc/modules.

SNESDev has to be run as background process with

```shell
/SNESDev-RPi/bin/SNESDev &
```

For comments, corrections, and suggestions visit https://github.com/petrockblog/SNESDev-RPi.

Have fun!


Raspberry Pi is a trademark of the Raspberry Pi Foundation.
