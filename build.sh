#!/bin/bash

#  snesdev_build.sh - Shell script for building SNESDev
# 
#  (c) Copyright 2012-2013  Florian Müller (contact@petrockblock.com)
# 
#  Permission to use, copy, modify and distribute this script in both binary and
#  source form, for non-commercial purposes, is hereby granted without fee,
#  providing that this license information and copyright notice appear with
#  all copies and any derived work.
# 
#  This software is provided 'as-is', without any express or implied
#  warranty. In no event shall the authors be held liable for any damages
#  arising from the use of this software.
# 
#  This script is freeware for PERSONAL USE only. Commercial users should
#  seek permission of the copyright holders first. Commercial use includes
#  charging money for this script or software derived from this script.
# 
#  The copyright holders request that bug fixes and improvements to the code
#  should be forwarded to them so everyone can benefit from the modifications
#  in future versions.
# 
#  Many, many thanks go to all people that provide the individual packages!!!
# 
#  Raspberry Pi is a trademark of the Raspberry Pi Foundation.
# 

scriptdir=`dirname $0`
scriptdir=`cd $scriptdir && pwd`

# build BCM 2835 library for config file management
echo -e "\n= = = = = = = = = =\nBuilding BCM 2835 library\n= = = = = = = = = =\n"
pushd "$scriptdir/libs/bcm2835-1.14/"
./configure --prefix="$scriptdir/libs/bcm2835-1.14/installdir/"
make
make install
popd

# build Confuse library for config file management
echo -e "\n= = = = = = = = = =\nBuilding Confuse library\n= = = = = = = = = =\n"
pushd "$scriptdir/libs/confuse-2.7/"
./configure --prefix="$scriptdir/libs/confuse-2.7/installdir/"
make
make install
popd

# build SNESDev-Rpi
echo -e "\n= = = = = = = = = =\nBuilding SNESDev-Rpi\n= = = = = = = = = =\n"
LIBCONFUSE_DIR="$scriptdir/libs/confuse-2.7/installdir" LIBBCM2835_DIR="$scriptdir/libs/bcm2835-1.14/installdir" make 
