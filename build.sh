#!/bin/bash

prefix=/usr/local/

debug() {
	mkdir -p build
	cd build

	cmake "-DCMAKE_BUILD_TYPE=DEBUG" "-DCMAKE_INSTALL_PREFIX=$prefix" ".."
	make
	sudo make install
}

release() {
	mkdir -p build
	cd build

	cmake "-DCMAKE_BUILD_TYPE=RELEASE" "-DCMAKE_INSTALL_PREFIX=$prefix" ".."
	make
	sudo make install
}

clean() {
	cd build
	make clean
}

help() {
	printf "[cli options]:\n"
	printf "\t-d - compile and install debug version\n"
	printf "\t-r - compile and install release version\n"
	printf "\t-c - delete compiled object files\n"
	printf "\t-h - prints help message\n"
}

check_options() {
	if [ -z $* ]; then
		release
		exit 0
	fi

	while getopts "drch" opt; do
		case $opt in
			d) debug ;;
			r) release ;;
			c) clean ;;
			h) help ;;
			*) echo "unknown option" ;;
		esac
	done
}

check_options "$@"
