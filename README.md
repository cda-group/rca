# Runtime Compiler Assistent

Currently, RCA extracts 1) CPU features 2) target triple through set environment variable.

Compile RCA
```
$ mkdir build; cd build; cmake ../ && make
```

Setting Target Triple ENV
```
$ export TARGET_TRIPLE=$(gcc -dumpmachine)
```

