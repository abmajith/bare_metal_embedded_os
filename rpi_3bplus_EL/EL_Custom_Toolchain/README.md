# Custom Toolchain by crosstool-NG

## Recommended Folder Structure.
```bash
mkdir -p rpi3bplus-prj # in user space
export PRJ="$PWD/rpi3bplus-prj"

#crosstool-ng
git clone https://github.com/crosstool-ng/crosstool-ng.git
export CTNG_DIR="$PWD/ct-ng-install"
mkdir -p $CTNG_DIR

#installing crosstool chain bin in user space (CTNG_DIR)
cd crosstool-ng
./bootstrap
./configure --prefix="${CTNG_DIR}/install"
make
make install 
export PATH="$CTNG_DIR/install/bin:$PATH"
#Successfully installed ct-ng in user space and available with 
# current environment variable $PATH

mkdir -p "$PRJ/x-tools"
mkdir -p "$PRJ/config"
mkdir -p "$PRJ/src"
mkdir -p "$PRJ/build"
mkdir -p "$PRJ/build/x-toolchain"
cd "$PRJ/build/x-toolchain"
ct-ng menuconfig
```

In this one it will pop up a X-window GUI, there we need to set up couple of parameters to build cross-tool chain for our RaspberryPi 3b Plus board.

### Paths and misc options
    - set ${PRJ}/src in Local tarballs directory
    - set ${CT_PREFIX:-${PRJ}/x-tools}/${CT_TARGET} in prefix directory.
### Target Options
    - set "arm" for the target architecture 
    - set Little endian for the Endianness
    - set Bitness as 64
### Toolchain Options
    - set *unknown* to Tuple's vendor string.
    - set *rpi3* to Tuple's alias
    - set *Cross* to Tool chain Type
### Operating System
    - set linux to Target OS 
    - set (choose for your taste the version), I choose 6.13
### C Library
    - set glibc to C-library

And save them with default name (.config in the current directory)

```bash
cp .config "$PRJ/config/ctng-rpi3bp.config"
ct-ng build

export CROSS_COMPILE="$PRJ$/x-tools/aarch64-unknown-linux-gnu/bin/aarch64-unknown-linux-gnu-"
$CROSS_COMPILEgcc -v
$CROSS_COMPILEld --version
```