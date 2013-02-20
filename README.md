SNESDev-RPi
===========

Implements two SNES game controllers and a virtual keyboard for up to two SNES controllers and a button that are connected to the GPIO pins of the Raspberry Pi. 

Installation
------------

First of all, make sure that Git is installed:

```shell
sudo apt-get update
sudo apt-get install -y git
```

SNESDev needs the BCM 2835 library installed. It is downloaded and installed with

```shell
echo "Installing BCM2835 library"
cd
wget http://www.open.com.au/mikem/bcm2835/bcm2835-1.14.tar.gz
tar -zxvf bcm2835-1.14.tar.gz
cd bcm2835-1.14
./configure
make clean
make
sudo make install
cd
rm bcm2835-1.14.tar.gz
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
sudo modprobe uinput
```

If you want to have the uinput module automatically loaded, you can add "uinput" to the file 
/etc/modules.

SNESDev has to be run as background process with

```shell
sudo ~/SNESDev-RPi/bin/SNESDev &
```

In order to access the uinput device SNESDev has to be run as root. This is (obviously) not so nice and is currently an issue. If you have a solution or suggestion for that, feel free to submit a pull request or send me a mail!

Running SNESDev with Arguments
------------------------------

SNESDev can be started with an argument that enables the polling of only the button, only the game pads, or both:

```shell
SNESDev 1 poll controllers only
SNESDev 2 poll button only
SNESDev 3 poll controllers and button (DEFAULT)
```


Running SNESDev as a service
----------------------------

Thanks to Ewan Meadows, SNESDev-RPi comes woth a script that allows SNESDev to be run as a service. The installation command for that are

```shell
chmod +x ./scripts/SNESDev
sudo cp ./bin/SNESDev /usr/local/bin/
sudo cp ./scripts/SNESDev /etc/init.d/
# This command installs the init.d script so it automatically starts on boot
sudo update-rc.d SNESDev defaults
# This command starts the daemon now so no need for a reboot
sudo service SNESDev start
```

Button Polling
--------------

If you use the functionality for polling a button (on GPIOin P1-11), a three-state automaton is implemented:
 
- press and hold: send "r" key (for rewind function of RetroArch)
- press and release three times: send "ESC"
- press and release five times: shutdown

For comments, corrections, and suggestions visit https://github.com/petrockblog/SNESDev-RPi.

Have fun!


Raspberry Pi is a trademark of the Raspberry Pi Foundation.
