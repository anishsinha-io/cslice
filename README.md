### C Slices

This repository implements Golang-like slices in C. The library is pretty light on abstractions and in general is quite
efficient.

Feel free to make pull requests to improve the library or to fix any bugs. However, I would ask that you at least write
some tests before you submit one.

### Usage

Make sure you have CMake and GNU Make installed first!

To use this library, you will need to let you compiler know to look for it. This is accomplished by the following
procedures:

#### Installation

1. `git clone` this repository into any directory you want.
2. `cd` into that directory
3. run `mkdir build` to create a build directory
4. `cd` into the build directory
5. run `cmake ..`
6. run `make`
7. run `sudo make install`, and then type in your password

The above steps will install the library's public headers into `/usr/local/include` and the binary
into `/usr/local/lib` (on *nix machines).

#### Including

To include this library in a program, you will need to link it. For CMake projects, this is accomplished by the
following steps:

1. make sure that `/usr/local/include` is included using an `include_directories` directories,
   e.g. `include_directories(/usr/local/include)`
2. make sure that your `link_directories` directive includes `/usr/local/lib`, e.g. `link_directories(/usr/local/lib)`
3. finally, make sure you actually *link* this library with the `target_link_libraries` directive,
   e.g. `target_link_libraries(<your_project> PUBLIC "cslice" ...<other libraries>)`
4. Now you should be able to `#include <cslice.h>` in your program and use the functionality.

Please star this repository if you find it to be useful.

-A 
