# Runtime Compiler Assistent

Currently, RCA extracts CPU features, target triple, and kernel version.

**Compile RCA:**
```
$ mkdir build; cd build; cmake ../ && make
```

**Sample output:**
```
$ {"arch":"x86","triple":"x86_64-linux-gnu","kernel":"4.15.0-041500-generic","uarch":"INTEL_HSW","brand":"Intel(R) Core(TM) i7-4510U CPU @ 2.00GHz","flags":["aes","avx","avx2","bmi1","bmi2","cx16","erms","f16c","fma3","sse4_1","sse4_2","ssse3"]}
```
