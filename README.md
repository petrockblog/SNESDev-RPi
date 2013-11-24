SNESDev-RPi
===========

SNESDev is a user-space driver for the RetroPie GPIO Adapter for the Raspberry Pi. It implements two (S)NES game controllers and a virtual keyboard for up to two (S)NES controllers and a button that are connected to the GPIO pins of the Raspberry Pivia the RetroPie GPIO Adapter (http://blog.petrockblock.com/2012/10/21/the-retropie-gpio-adapter/). 

Installation
------------

Manual installation:

First of all, make sure that Git is installed:

```shell
sudo apt-get update
sudo apt-get install -y git
```


SNESDev is downloaded and installed with

```shell
cd
git clone git://github.com/petrockblog/SNESDev-RPi.git
cd SNESDev-RPi
./build.sh
```

The lines above build two needed libraries and SNESDev-Rpi.

Alternatively, you can use the RetroPie Setup Script (https://github.com/petrockblog/RetroPie-Setup) for installing and configuring SNESDev.

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

Configuring SNESDev-Rpi
-----------------------

SNESDev-Rpi is configured with the help of the configuration file ```/etc/snesdev.cfg```. It is a lightweighted configuration file and well commented.


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
