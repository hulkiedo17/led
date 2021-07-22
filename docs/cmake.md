# cmake led doc

if you want to compile program without debug symbols, type this:
```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
$ make
$ sudo make install
```

or with compile symbols:
```shell
$ mkdir build && cd build
$ cmake -DCMAKE_BUILD_TYPE=DEBUG -DCMAKE_INSTALL_PREFIX=/usr/local/ ..
$ make
$ sudo make install
```
