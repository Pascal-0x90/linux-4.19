#!/bin/bash

# Colors
RED='\033[0;31m'
GREEN='\033[1;32m'
NC='\033[0m' # No Color

usb_push() {
    # Check if root
    if [ "$EUID" -ne 0 ]
      then echo "Please run as root"
      exit
    fi
    # Make some mnt points
    mkdir /mnt/boot
    mkdir /mnt/root

    # Mount to those mount points
    mount /dev/sdb1  /mnt/boot
    mount /dev/sdb2 /mnt/root

    # Install modules on to sdcard
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=/mnt/root modules_install

    # Push over kernel image and device tree
    cp arch/arm/boot/zImage /mnt/boot/kernel7.img  #copy the image file
    cp arch/arm/boot/dts/*.dtb /mnt/boot/  #//copy the hardware information
    cp arch/arm/boot/dts/overlays/*.dtb* /mnt/boot/overlays/   #//copy the device tree overlay
    cp arch/arm/boot/dts/overlays/README /mnt/boot/overlays/
    
    # Remove all removable lifeforms
    for usb_dev in /dev/disk/by-id/usb-*; do
        dev=$(readlink -f $usb_dev)
        grep -q ^$dev /proc/mounts && umount $dev
    done

    # Yeet the drive
    umount /mnt/boot
    umount /mnt/root
}

compile() {
    # Export var
    export KERNEL=kernel7

    # Compile boioii
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcm2709_defconfig
    make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs -j 16
}

menu() {
    printf "$GREEN Pascal Linux Cross Compile Script:\n"
    echo "=================================="
    printf "#\t[Cc] --> Compile kernel\t #\n"
    printf "#\t[Uu] --> Push to usb \t #\n"
    printf "#\t[Ee] --> Exit script \t #\n"
    printf "==================================$NC\n"
}

## Check if root
#if [ "$EUID" -ne 0 ]
#  then echo "Please run as root"
#  exit
#fi

# Ask compile
while true; do
    menu
    read -p "" yn 
    case $yn in
        [Cc]* ) compile;; 
        [Uu]* ) usb_push;;
        [Ee]* ) exit;;
    esac
done

