# makefile help documentation
here you can find some explanations about the makefile that builds this project.

### options
- install - installs the project if you compile him before this.
- uninstall - uninstall the project if you installed him before this.
- clean - cleans the bin/debug or bin/release directory from the object files.
- cleandep - cleans the bin/debug or bin/release directory from the *.d(dependencies) files.

### main internal variables
- CC - by default is gcc (if you want to change this variable, define the CC=your_compiler at the options while you calls the make program, example: make CC=clang).
- LDFLAGS - by default is empty(without values)(you also can change this variable like the previous variable, at the cli).
- EXEC - default executable name.

