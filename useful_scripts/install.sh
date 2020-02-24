#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "Please run as root"
  exit
fi

cp -rf usr /

echo "Scrpits succesfully installed."
