#!/bin/sh

prefix=/usr/local
debug_flag=true
silence_flag=true

mkdir -p ./bin
cd ./bin
mkdir -p release debug
cd ../

for arg in "$@"; do
	case "$arg" in
		--prefix=*)
			prefix=`echo $arg | sed 's/--prefix=/'`
			;;
		--enable-debug)
			debug_flag=true
			;;
		--disable-debug)
			debug_flag=false
			;;
		--enable-silence)
			silence_flag=true
			;;
		--disable-silence)
			silence_flag=false
			;;
		--help)
			echo "usage: ./configure.sh [options]"
			printf "[options]:\n"
			printf "\t--prefix=<path>:		installation path prefix\n"
			printf "\t--enable-debug:		include debug symbols\n"
			printf "\t--disable-debug:		do not include debug symbols\n"
			printf "\t--enable-silence:		activate silence mode\n"
			printf "\t--disable-silence:	activate non-silence mode\n"
			printf "all invalid options are silently ignored\n"
			exit 0
			;;
	esac
done

echo "generating makefile..."

echo ".POSIX:" >Makefile
echo "PREFIX = $prefix" >>Makefile

if $debug_flag; then
	echo "DEBUG = TRUE" >>Makefile
else
	echo "DEBUG = FALSE" >>Makefile
fi

if $silence_flag; then
	echo "V = TRUE" >>Makefile
else
	echo "V = FALSE" >>Makefile
fi

cat Makefile.in >>Makefile

echo "configuration complete, type make to build"
