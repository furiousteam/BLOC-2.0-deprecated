<h1 align="center">
  <br>
  <a href="https://bloc.money" target="_blank"><img src="https://user-images.githubusercontent.com/1941393/40306571-73d3c700-5cff-11e8-8a75-9f1498106e27.png" alt="BLOC" width="350"></a>
  <br>
  BLOC
  <br>
</h1>

<h4 align="center">A secure, private, decentralized, egalitarian, fungible, fast <a href="https://bloc.money" target="_blank">cryptocurrency</a>.</h4>

<p align="center">
  <a href="https://www.gnu.org/licenses/gpl-3.0">
    <img src="https://img.shields.io/badge/License-GPL%20v3-blue.svg"
         alt="License: GPL v3">
  </a>
</p>

<p align="center">
  <a href="#key-features">Key Features</a> •
  <a href="#how-to-use">How To Use</a> •
  <a href="#download">Download</a> •
  <a href="#api-references">API References</a> •
  <a href="#credits">Credits</a> •
  <a href="#related">Related</a> •
  <a href="#license">License</a>
</p>

## Key Features

* Decentralized
* True egalitarian Proof of Work
* Secure
* Private
* Untraceable
* Transparent
* Fast transactions speeds
* Fungible
* Double spending protection

The [BLOC.money](https://bloc.money) is the new digital cash powered by an open-source blockchain with anonymous transactions including exclusive mining tools which provide a true egalitarian money distribution. Everybody can earn passive income while sleeping or just anywhere, anytime by simply using a mobile phone.

Our Proof Of Work algorithm is designed to make low end and high end hardware mining roughly equally efficient and restrict ASIC mining.

## How to Use

To clone and build the BLOC core, you'll need Git installed on your computer. From your command line:

```
# Clone this repository
$ git clone https://github.com/furiousteam/blockchain-coin.git

# Go into the repository
$ cd blockchain-coin
```

### macOS instructions

After installing [Xcode](https://developer.apple.com), make sure the Command Line Tools are installed as well:

```
xcode-select --install
```

BLOC also needs [CMake](https://cmake.org/) 2.8.6 or later as well as [Boost](https://www.boost.org/) 1.55 (make sure [Homebrew](https://brew.sh) is also installed):

```
brew install cmake
brew install boost
```

Build everything:

```
make
```

### Ubuntu (16.04) instructions

Install the needed dependencies:

```
apt-get update
apt-get install sudo
sudo apt-get -y install build-essential cmake libboost-all-dev  vim git pkg-config
sudo apt-get -y install libssl-dev libzmq3-dev libunbound-dev libsodium-dev libminiupnpc-dev libunwind8-dev
sudo apt-get -y install liblzma-dev libreadline6-dev libldns-dev libexpat1-dev doxygen graphviz
sudo apt-get -y install libgtest-dev git
```

Build everything:

```
make
```

### Windows instructions

Make sure to install the following dependencies:

* [MS Visual Studio](https://www.visualstudio.com) 2013 or later
* [Boost for Visual Studio](https://dl.bintray.com/boostorg/release/1.64.0/binaries/boost_1_64_0-msvc-14.1-64.exe)
* [QT for Visual Studio](http://download.qt.io/official_releases/qt/5.10/5.10.0/qt-opensource-windows-x86-5.10.0.exe)
* [MSBuild](https://www.microsoft.com/en-us/download/details.aspx?id=48159)
* [CMake](https://cmake.org/install/)

To build everything, inside the project folder:

```
mkdir build
cd build
cmake -DSTATIC=1 -Wno-dev ^
-DCMAKE_PREFIX_PATH="C:\Qt\Qt5.10.0\5.10.0\msvc2015_64" ^
-DBOOST_ROOT="C:\local\boost_1_64_0_64" ^
-DBOOST_INCLUDEDIR="C:\local\boost_1_64_0_64\lib64-msvc-14.1" ^
-DBOOST_LIBRARYDIR="C:\local\boost_1_64_0_64\libs" ^
-G "Visual Studio 15 Win64" ..

# Open Solution file in Build folder and Build Release.
```

If you are building on an older processor without AVX support, add the following options to `cmake`:

```
-DPORTABLE=1 -DWITH_AVX2=0
```

### Advanced settings

* **Parallel build**: use `make -j<number of threads>` instead of `make`.
* **Debug build**: run `make build-debug`.
* **Test suite (Release)**: run `make test-release` to run tests in addition to building.
* **Test suite (Debug)**: run `make test-debug` to run tests in addition to building.
* **Building with Clang**: it's possible to use Clang instead of GCC. To build, run `export CC=clang CXX=clang++` before running make.

## BLOCd

`BLOCd` is used to sync the blockchain locally and is needed to run `simplewallet`, `walletd` and `miner`.

```
./BLOCd
```

![BLOCd](https://user-images.githubusercontent.com/1941393/40308931-1848e628-5d08-11e8-96b2-0022d96f3e10.png)

```
Commands:
  exit            Shutdown the daemon
  help            Show this help
  print_bc        Print blockchain info in a given blocks range, print_bc <begin_height> [<end_height>]
  print_block     Print block, print_block <block_hash> | <block_height>
  print_cn        Print connections
  print_pl        Print peer list
  print_pool      Print transaction pool (long format)
  print_pool_sh   Print transaction pool (short format)
  print_tx        Print transaction, print_tx <transaction_hash>
  set_log         set_log <level> - Change current log level, <level> is a number 0-4
```

The complete API reference is available here:

* [Daemon command line arguments and commands](https://wiki.bytecoin.org/wiki/Daemon_command_line_arguments_and_commands)
* [Daemon HTTP RPC API](https://wiki.bytecoin.org/wiki/Daemon_HTTP_RPC_API)
* [Daemon JSON RPC API](https://wiki.bytecoin.org/wiki/Daemon_JSON_RPC_API)

## simplewallet

`simplewallet` is a simple CLI wallet for BLOC.

![simplewallet](https://user-images.githubusercontent.com/1941393/40309016-5c4ab64e-5d08-11e8-9115-1bcccc0d0e87.png)

```
Usage: simplewallet [--wallet-file=<file>|--generate-new-wallet=<file>] [--daemon-address=<host>:<port>] [<COMMAND>]
General options:
  --help                          Produce help message
  --version                       Output version information

Wallet options:
  --wallet-file arg               Use wallet <arg>
  --generate-new-wallet arg       Generate new wallet and save it to <arg>
  --password arg                  Wallet password
  --daemon-address arg            Use daemon instance at <host>:<port>
  --daemon-host arg               Use daemon instance at host <arg> instead of
                                  localhost
  --daemon-port arg (=0)          Use daemon instance at port <arg> instead of
                                  8081
  --command arg
  --set_log arg
  --testnet                       Used to deploy test nets. The daemon must be
                                  launched with --testnet flag
  --rpc-bind-ip arg (=127.0.0.1)  Specify ip to bind rpc server
  --rpc-bind-port arg             Starts wallet as rpc server for wallet
                                  operations, sets bind port for server
  --rpc-password arg              Specify the password to access the rpc
                                  server.
  --rpc-legacy-security           Enable legacy mode (no password for RPC).
                                  WARNING: INSECURE. USE ONLY AS A LAST RESORT.

Commands:
  address              Show current wallet public address
  balance              Show current wallet balance
  bc_height            Show blockchain height
  exit                 Close wallet
  help                 Show this help
  incoming_transfers   Show incoming transfers
  keys                 Show wallet private keys
  list_transfers       Show all known transfers
  payments             payments <payment_id_1> [<payment_id_2> ... <payment_id_N>] - Show payments <payment_id_1>, ... <payment_id_N>
  reset                Discard cache data and start synchronizing from the start
  save                 Save wallet synchronized data
  set_log              set_log <level> - Change current log level, <level> is a number 0-4
  start_mining         start_mining [<number_of_threads>] - Start mining in daemon
  stop_mining          Stop mining in daemon
  transfer             transfer <mixin_count> <addr_1> <amount_1> [<addr_2> <amount_2> ... <addr_N> <amount_N>] [-p payment_id] [-f fee] - Transfer <amount_1>,... <amount_N> to <address_1>,... <address_N>, respectively. <mixin_count> is the number of transactions yours is indistinguishable from (from 0 to maximum available)
```

You can also specify various settings for the RPC interface:

```
./simplewallet --wallet-file filename --password yourpassword --rpc-bind-ip=0.0.0.0 --rpc-bind-port 2053 --rpc-password rpcpassword
```

The complete API reference is available here:

* [Simplewallet JSON RPC API](https://wiki.bytecoin.org/wiki/Simplewallet_JSON_RPC_API)
* [Simplewallet command line arguments and commands](https://wiki.bytecoin.org/wiki/Simplewallet_command_line_arguments_and_commands)

## walletd

`walletd` is a more advanced CLI wallet that can handle many wallets at once inside a container.

Start by creating a config file:

```
container-file = yourfile
container-password = password
daemon-port = 2086
bind-port = 2053
testnet = no
bind-address = 0.0.0.0
rpc-password = rpcpassword
```

Then generate the container:

```
./walletd --container-file=mywallet --container-password=password --generate-container
```

Start walletd:

```
./walletd --config=yourfile.conf --rpc-password yourrpcpassword
```

The complete API reference is available here:

* [Bytecoin RPC Wallet JSON RPC API](https://wiki.bytecoin.org/wiki/Bytecoin_RPC_Wallet_JSON_RPC_API)
* [Bytecoin RPC Wallet](https://wiki.bytecoin.org/wiki/Bytecoin_RPC_Wallet)

## Download

You can [download](https://bloc.money/download) latest stable version of the BLOC software for Windows, macOS and Linux.

## API References

* [Simplewallet JSON RPC API](https://wiki.bytecoin.org/wiki/Simplewallet_JSON_RPC_API)
* [Simplewallet command line arguments and commands](https://wiki.bytecoin.org/wiki/Simplewallet_command_line_arguments_and_commands)
* [Bytecoin RPC Wallet JSON RPC API](https://wiki.bytecoin.org/wiki/Bytecoin_RPC_Wallet_JSON_RPC_API)
* [Bytecoin RPC Wallet](https://wiki.bytecoin.org/wiki/Bytecoin_RPC_Wallet)
* [Daemon command line arguments and commands](https://wiki.bytecoin.org/wiki/Daemon_command_line_arguments_and_commands)
* [Daemon HTTP RPC API](https://wiki.bytecoin.org/wiki/Daemon_HTTP_RPC_API)
* [Daemon JSON RPC API](https://wiki.bytecoin.org/wiki/Daemon_JSON_RPC_API)

## Credits

This software uses code from several open source packages.

* [Add project](https://todo.com)
* [Add project](https://todo.com)
* [Add project](https://todo.com)
* [Add project](https://todo.com)
* [Add project](https://todo.com)

## Related

* [BLOC.money](https://bloc.money/) - Official website of the BLOC cryptocurrency
* [BLOC Explorer](https://bloc-explorer.com) - Blockchain Explorer for BLOC
* [BLOC Mining](https://bloc-mining.com/) - In-browser Javascript miner for BLOC
* [BLOC Developer](https://bloc-developer.com) - In-depth documentation for all APIs related to BLOC

## License

GPL v3
