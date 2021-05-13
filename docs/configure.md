# configure help documentation
Here you can find some explanations about the configure.sh script that configuring the Makefile.

### options

- --prefix=your_directory - sets the destination directory of installed programs
- --enable-debug - sets the debug_flag is true(and program will compiled with debug options)
- --disable-debug - sets the debug_flag is false(and program will compiled without debug options)
- --enable-silence - sets the silence_flag is true(no detailed information about compilation will be displayed at compile time)
- --disable-silence - sets the silence_flag is false(detailed information about compilation will be displayed at compile time)
- --help - prints the help message

by default the debug_flag and silence_flag is true.

### usage
```shell
$ ./configure.sh [options]
```

if it does not work when trying to execute the script, grant it execution persmission in the following way:
```shell
$ chmod +x ./configure.sh
```
