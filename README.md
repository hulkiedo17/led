# led
led is a line-oriented text editor in command line. This editor is similar to the standard program on unix systems - GNU ed. But i'm not going to make an exact clone of that program, it's just a pet project.

## Example of usage

<p align="center">
	<image src="/docs/led.gif">
</p>

## Features
Now the program can save text to its buffer, it can also write or add text from the buffer to a file, show the contents of the buffer, delete or insert lines of text from the buffer. In the future, functions are planned for copying, deleting a substring, concatenating strings, addressing strings, etc.

To see commands and command line arguments, use this flags: -h or --help

## How to launch
This program work only on linux(ubuntu)

Required tools: gcc, cmake, make, shell(bash)

To compile program type this:
```shell
$ ./build.sh -r
```
Or use more familiar way:
```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
$ make
$ sudo make install
```
