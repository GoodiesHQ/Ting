# Ting

Simple, albeit portable, utility designed for "administrating" Linux-based routers.

Rootkits sold seperately.

### How to compile:

**Note:** If you would like to use a different libc implementation, such as musl, uclibc, or dietlibc, you may supply the respective compilers in the `CC` environment variable.

    git clone https://github.com/GoodiesHQ/Ting.git
    cd Ting
    mkdir build
    cd build
    CC="gcc -static -Os" cmake ..
    make -j`nproc`
    
|**LibC**|**Command**|**Size**|**Size (stripped)**|
|--------|-----------|--------|-------------------|
|Glibc|CC="gcc -Os -static" cmake ..|709K|639K|
|musl|CC="musl-gcc -Os -static" cmake ..|21K|14K|
|dietlibc|CC="diet gcc -Os -static" cmake ..|13K|9K|

### Features:

 - Establish a GRE tunnel to a server (`TING_GRE_DESTINATION` in `inc/ting/gre.h`) and duplicate all received IPv4 packets over the tunnel.
