#!/bin/sh

prefix=/usr/local
debugsym=true

mkdir bin

for arg in "$@"; do
	case  "$arg" in
	--prefix=*)
		prefix=`echo $arg | sed 's/--prefix=/'`
		;;
	--enable-debug)
		debugsym=true
		;;
	--disable-debug)
		debugsym=false
		;;
	--help)
		echo "usage: ./configure.sh [options]"
		echo "options:"
		echo "	--prefix=<path>:	installation path prefix"
		echo "	--enable-debug:		include debug symbols"
		echo "	--disable-debug:	do not include debug symbols"
		echo "all invalid options are silently ignored"
		;;
	esac
done

echo "generating makefile..."

echo ".POSIX:" >Makefile
echo "PREFIX = $prefix" >>Makefile

if $debugsym; then
	echo "DEBUG = TRUE" >>Makefile
else
	echo "DEBUG = FALSE" >>Makefile
fi

cat Makefile.in >>Makefile

echo "configuration complete, type make to build"

