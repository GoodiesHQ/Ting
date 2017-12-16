
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

 - Establish a GRE transparent ethernet tunnel to a server and send a copy of all received ethernet frames.
   - The GRE server is defined as `TING_GRE_DESTINATION` in `inc/ting/gre.h`. Change it to your desired IPv4 address. By default, it is `"7.7.7.7"` which is a bogon, so nothing will happen.
   - All ethernet frames are duplicated. This pay potentially be a LOT. Add `-DTING_CAPTURE_IP_ONLY` will change the socket's protocol to  `ETH_P_IP` instead of `ETH_P_ALL` and will strip the Ethernet header so that only get IP packets are sent across the GRE tunnel (this will still accept multicast packets).
