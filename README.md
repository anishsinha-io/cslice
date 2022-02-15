### C Slices

This repository implements Golang-like slices in C. The library is pretty light on abstractions and in general is quite efficient. 

Feel free to make pull requests to improve the library or to fix any bugs. However, I would ask that you at least write some tests before you submit one.

### Using

`git clone` this repository. `cd` into the directory you cloned this repo into. Run `mkdir build`. `cd` into the build directory. Run `cmake ..`. Run `make`. Run `sudo make install`. Now, make sure you include `/usr/local/include` in whatever project you want to use this in, and link `/usr/local/lib`. Add `cslice` to your `target_link_libraries` in your `CMakeLists.txt`. That should be it. If you have any errors, please open an issue on the [repo](https://github.com/anish-sinha1/cslice), which is here.