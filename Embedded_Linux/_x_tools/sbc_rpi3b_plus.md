# System Setup for RaspberryPi 3B Plus Board

## Crosstool-ToolChain
Follow [Cross Tool Chain](./x-tools_options.md) build options.
If you followed Ct-NG method, continue below o.w can pass to next section
### Paths and misc options
- Recommended saving the downloads by ct-ng (${pwd}/src) Local tarballs
- Recommended saving the output (i.e x-tool) (${CT_PREFIX:-/home/<username>/dev/<sbc_name>/x-tools}/${CT_HOST:+HOST-${CT_HOST}/}${CT_TARGET}) Prefix directory 

the target option should be trivial 
### Target Options
- Target Architecture (arm)
- Endianness: (Little endian)
- Bitness: (64-bit)

### Operating System
It is usually better to specific about the linux version, in case if you want to test with different version to provide performance stats.
- Target OS (linux)  --->
- Source of linux (Released tarball)  ---> 
- Version of linux (6.6.101)  --->
- [*] Build shared libraries

### Binary Utilities
- Binutils (binutils)  --->  
- Version of binutils (2.45)  ---> 

### C-library (system API)
- C library (glibc)  --->
- Version of glibc (2.42)  --->

### C Compiler
- Compiler (gcc)  ---> 
- Version of gcc (15.2.0)  --->  
And with needs, include various options like C++, Fortran, and also check in the internet there is wikipedia page that mentions various worked out example of 
the various successful compilation with C Compiler, Binary Utilities and C-library (not all enumerated version combination works).

And save them with default name (.config in the current directory)

```bash
cp .config "~/dev/<sbc_name>/config/ctng-rpi3bp.config"
ct-ng build

export CROSS_COMPILE="~/dev/<sbc_name>/x-tools/aarch64-rpi3-linux-gnu/bin/aarch64-rpi3-linux-gnu-"
${CROSS_COMPILE}gcc -v
${CROSS_COMPILE}ld --version
```