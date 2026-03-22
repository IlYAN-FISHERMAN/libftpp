#!/bin/sh

if [ "$1" == "run" ];
then
	cmake -B build -S .
elif [ "$1" == "install" ];
then
	cmake -B build -S .
	sudo cmake --install build --prefix /usr/local
elif [ "$1" == "clear" ];
then
  rm -rf ./build/
else
  echo "Usage:"
  echo "  sh build.sh run"
  echo "  sh build.sh clear"
  echo "  sh build.sh install"
fi
