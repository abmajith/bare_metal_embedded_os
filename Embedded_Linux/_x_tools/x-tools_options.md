# Custom Toolchain Options crosstool-NG

- **SBC** External Board Manufacture supported Tool Chain. For example Linaro GCC tool chain for ARM Architecture.
- **Ct-NG** Cross Tool Chain NG open source x-tool chain compiling (recommended) for learning.
- **APT-Package** For well known SBC, we do have `sudo apt install <sbc>-x-tools`, search according to the board.

## External Board Support
Example
### Downloading a pre-build toolchain from Linaro and extract toolchain
```bash
mkdir -p ~/dev/<sbc_name>/x-tools
cd ~/dev/<sbc>/x-tools
wget https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
tar -xf gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
export PATH=~/dev/<sbc_name>/x-tools/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu/bin:$PATH
cd ../
```

## Recommended Folder Structure for CT-NG.
```bash
mkdir -p ~/dev/ct-ng-github~ # in user space
mkdir -p ~/dev/ct-ng-install
cd ~/dev

#crosstool-ng
git clone https://github.com/crosstool-ng/crosstool-ng.git ~/dev/ct-ng-github
export CTNG_DIR="$PWD/ct-ng-install"

cd ct-ng-github
./bootstrap
./configure --prefix="${CTNG_DIR}/install"
make
make install 
export PATH="$CTNG_DIR/install/bin:$PATH"

#Test ct-ng 
ct-ng --version
# something as follow will see
GNU Make 4.3
Built for x86_64-pc-linux-gnu
```

