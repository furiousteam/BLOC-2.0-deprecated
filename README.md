Intense Coin is a cryptocurrency focused on providing a decentralized mechanism of exchange, and anonymity via untraceable and unlinkable transactions.

## Building BlockChain Coin

### On macOS

Dependencies: GCC 4.7.3 or later, CMake 2.8.6 or later, and Boost 1.55.

```
# install & update xcode
xcode-select --install
# install brew
brew install cmake
brew install boost
```
To build, change to a directory where this file is located, and run `make`. The resulting executables can be found in `build/release/src`.

### On Ubuntu 16.04

```
apt-get update
apt-get install sudo
sudo apt-get -y install build-essential cmake libboost-all-dev  vim git pkg-config
sudo apt-get -y install libssl-dev libzmq3-dev libunbound-dev libsodium-dev libminiupnpc-dev libunwind8-dev
sudo apt-get -y install liblzma-dev libreadline6-dev libldns-dev libexpat1-dev doxygen graphviz
sudo apt-get -y install libgtest-dev git
```

**Advanced options:**

* Parallel build: run `make -j<number of threads>` instead of `make`.
* Debug build: run `make build-debug`.
* Test suite: run `make test-release` to run tests in addition to building. Running `make test-debug` will do the same to the debug version.
* Building with Clang: it may be possible to use Clang instead of GCC, but this may not work everywhere. To build, run `export CC=clang CXX=clang++` before running `make`.


### On Windows
Dependencies: MSVC 2013 or later, CMake 2.8.6 or later, and Boost 1.55. You may download them from:

* http://www.microsoft.com/
* http://www.cmake.org/
* http://www.boost.org/

To build, change to a directory where this file is located, and run these commands: 
```
mkdir build
cd build
cmake -G "Visual Studio 12 Win64" ..
```

If you are building on an older processor without AVX support, add the following options to cmake:

```
-DPORTABLE=1 -DWITH_AVX2=0
```

And then Build from within MS Visual Studio. You may find it helpful to explicitly include Boost library paths:

```
cmake -G -DBOOST_ROOT="C:\boost_1_64_0_built" -DBOOST_INCLUDEDIR="C:/boost_1_64_0_built/lib32-msvc-14.1" -DBOOST_LIBRARYDIR="C:\boost_1_64_0_built\libs" ..
 ```
