# cmake led doc

if you want to compile program without debug symbols, type this:
```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE ..
$ make
```

or with compile symbols:
```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=DEBUG .. # or just: cmake ..
$ make
```

