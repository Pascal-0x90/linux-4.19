#!/bin/bash

## Check if root
#if [ "$EUID" -ne 0 ]
#  then echo "Please run as root"
#  exit
#fi

apt-get install gcc-arm* bc git bison flex libssl-dev -y
