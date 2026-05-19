#!/bin/sh

if [ "$1" = "run" ];
then
	cmake -B build -S .  -DBUILD_CLIENT=OFF -DBUILD_LIB=ON -DBUILD_BINARY=ON -DBUILD_MATT_DAEMON=OFF && make -C build/
elif [ "$1" = "install" ];
then
	cmake -B build -S .  -DBUILD_CLIENT=OFF -DBUILD_LIB=ON -DBUILD_BINARY=OFF -DBUILD_MATT_DAEMON=OFF
	sudo cmake --install build --prefix /usr/local
elif [ "$1" = "clear" ];
then
  rm -rf ./build/
else
  echo "Usage:"
  echo "  sh build.sh run"
  echo "  sh build.sh clear"
  echo "  sh build.sh install"
fi
