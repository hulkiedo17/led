# led
led - line-oriented text editor

# program description
led is a line-oriented text editor in command line. This editor is similar to the standard program on unix systems - GNU ed. But i'm not going to make an exact copy of that program, it's just a pet project.

# features
For now, the program can save the entered text to the buffer, save the data from the buffer to the file, append the data from the file to the buffer, print the buffer, and clean the buffer. In the future, it is planned to add copying text, cutting text, addressing strings, joining strings, etc.

To see commands and command line arguments, check the files in the doc directory.

# how to launch
This program works only on linux(ubuntu)

Required tools: gcc, make, shell

To compile program type this:
```shell
$ ./configure
$ make
$ sudo make install
```

If you want to delete program(after installation), type:
```shell
$ sudo make uninstall
```

