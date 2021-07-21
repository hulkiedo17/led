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
	printf "\t-d (--debug)   - \n"
	printf "\t-r (--release) - \n"
	printf "\t-c (--clean)   - \n"
	printf "\t-h (--help)    - \n"
}

check_options() {
	while [[ $# -gt 0 ]]; do
		key="$1"
		case $key in
			-d|--debug)
				debug
				shift
				;;
			-r|--release)
				release
				shift
				;;
			-c|--clean)
				clean
				shift
				;;
			-h|--help)
				help
				shift
				;;
			*)
				print "unknown option"
				shift
				;;
		esac
	done
}

check_options "$@"
