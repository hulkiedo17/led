# led
led is a line-oriented text editor in command line. This editor is similar to the standard program on unix systems - GNU ed. But i'm not going to make an exact clone of that program, it's just a pet project.

## Example of usage

<p align="center">
	<image src="/doc/led.gif">
</p>

## Features
Now the program can save text to its buffer, it can also write or add text from the buffer to a file, show the contents of the buffer, delete or insert lines of text from the buffer. In the future, functions are planned for copying, deleting a substring, concatenating strings, addressing strings, etc.

To see commands and command line arguments, use this flags: -h or --help

## How to launch
This program work only on linux(ubuntu)

Required tools: gcc, make, shell(bash)

To compile program type this:
```shell
$ ./configure
$ make
$ sudo make install
```

if you want to delete program(after installation), type:
```shell
$ sudo make uninstall
```

By the way, if you build a project without debug symbols, then do not be surprised by the warnings that will be issued during the build, this does not affect the performance, bit this will be fixed in the future.

