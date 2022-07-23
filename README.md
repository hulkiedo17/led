# What is this?
led is a line-oriented text editor in command line. This editor is similar to the standard program on unix systems - GNU ed. But i'm not going to make an exact clone of that program, it's just a pet project.

# Features

Now the program can:
- save text to it's buffer
- write or append text to file
- show the contents of the buffer
- insert or delete text in buffer
- set own prompt in editor

# Example of usage

<p align="center">
	<image src="/etc/led.gif">
</p>

# How to launch

This program work only on linux(ubuntu).

Required tools: gcc, cmake, make, shell(bash).

To compile and install program type this:

```shell
$ ./build.sh
```

Or use cmake directly:

```shell
$ mkdir build && cd build
$ # or if you want release version, change option to -DCMAKE_BUILD_TYPE=RELEASE
$ cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
$ make
$ sudo make install
```

# Options

Enter the -h option to check all program options:

```sh
$ led -h
usage: led [options...]

[cli options]:
	-h        - prints this help message
	-v        - shows the program version
	-f [file] - set the filename by default

[program commands]:
	q  - quit
	sp [str] - set custom prompt
	sd - set default prompt
	sf [file] - set filename
	cs - clear screen
	c  - clean buffer
	p  - print buffer
	pl - print buffer with numbered lines
	pc - print buffer by characters
	pf - print filename
	a  - append data to buffer(without \n)
	an - append data to buffer(with \n)
	w [file] - safe buffer to file
	wa [file] - safe buffer to file(append, not rewrite)
	f [file] - fill buffer from file
	fn [file] - fill buffer from file(with \n)
	ia [line] [str] - insert data after specified line
	ian [line] [str] - insert data after specified line(with \n)
	ib [line] [str] - insert data before specified line
	ibn [line] [str] - insert data before specified line(with \n)
	dl [line] - delete the specified line
	dr [line1] [line2] - delete the lines in the specified range
```

Also, you can check build options, by typing -h to build script:

```sh
$ ./build.sh -h
usage: ./build.sh [options...]

options:
	-d - compile and install debug version
	-r - compile and install release version
	-c - delete compiled object files
	-h - prints help message
```

It is recommended to use this script from the main repository folder, not a build or other nested directories.

# TODO list

- [ ] add linked list, and delete overhead with buffers
- [x] delete getopt_long(), add getopt()
- [ ] rewrite parser
- [x] rewrite build system
- [x] rewrite documentation
- [ ] add a documentation about the program internals
- [ ] add a delete_substring()
- [ ] add a copying function
- [ ] add a addressing strings
- [ ] add a concatenate strings function
- [ ] add a insert function that uses an relative indices in the row for inserting data

