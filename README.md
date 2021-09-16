This set of accelerator generators can be used to collect errors in Xilinx floating point operators on FPGA under overclocked environment.

It must be used with an [overclocking-enabled SDSoC platform](https://github.com/ThibautMarty/sdsoc-overclocking-pfm). See [this repository](https://github.com/ThibautMarty/conv-hls-overclocking) for more information.

## Directory structure:

```
  project root            → initially call CMake with path to this directory as argument
  ├── CMakeLists.txt      → this file
  ├── options.cmake       → file with configuration options set through CMake (tile size, etc)
  ├── hw                  → hardware code directory, will be compiled with sds++ (see its CMakeLists.txt)
  │   ├── accelerator.cpp → hardware toplevel function file
  │   └── wrapper.cpp     → wrapper around accelerator - library entry point
  ├── inc                 → header files and configuration files directory
  │   ├── options.h.in    → options header processed by CMake
  │   └── options.sw.h.in → options header processed by CMake (options not used in hardware code)
  ├── src                 → non-hardware code, will be compiled with g++
  │   ├── main1.cpp       → an entry-point
  │   └── *.cpp           → other functions
  ├── sim                 → simulation code directory
  │   └── main.cpp        → code for simulation, should test accelerator but be fast enough
  ├── tests               → tests directory, will be compiled with host g++ (see its CMakeLists.txt)
  └── host                → other executables, will be compiled with host g++ (see its CMakeLists.txt)
```

## Basic usage

```
# create an out-source building directory
mkdir build
cd build

# call CMake with options
cmake .. -DNOHW=OFF -DPLATFORM=path/to/pfm -DDATA_T=float -DN=128 -DOPERATION=OPADD

# build everything
make

# launch tests without building accelerator
make check
```

### Run HLS synthesis

To run HLS synthesis without going through the full synthesis, run:
```
make -C hw accelerator.o
```

### Run simulations

To run simulations, run (after HLS synthesis):
```
make csim
make cosim
```

To get performance but avoid generating bitstream and save time, add `-mno-bitstream` to `SDSARGS` CMake option.

## Basic options

* `TESTS`: whether to run tests or not
* `HOSTBUILD`: whether build everything for host, without hardware
* `NOHW`: whether to build evreything as usual, but without hardware
* `NODSP`: do not use DSP blocks for main operators
* `DMCLKID`: clock ID for data mover
* `CLKID`: clock ID for accelerator
* `SDXVERSION`: SDx version (e.g 2018.2)
* `XILINXPATH`: Root path of Xilinx tools (e.g /opt/Xilinx)
* `PLATFORM`: SDSoC platform's name (or path)
* `SDSARGS`: flags given to sds compiler

Edit `options.cmake` to change defaults.

To get the clock ID of the platform and their corresponding frequency, run for e.g zybo:
```
sdscc -sds-pf-info zybo
```

## Log and report files

Important files are (in `hw/_sds` directory in the build directory):
* `reports/sds_accelerator.rpt`: HLS report (performance estimates, utilization estimates)
* `reports/sds_accelerator.log`: `sds++` and HLS log (warnings, pipelining, unrolling…)
* `reports/sds.log`: `sds++` log
* `reports/sds.rpt`: main report file, with timing and utilization report
* `reports/data_motion.html`: data motion report
* `vhls/hw_toplevel/solution/syn/report/*.rpt`: all HLS reports

## SD card generation

To create a complete SD card, use these files in the build directory:
- hw/sd_card/*
- hw/libaccelerator.so.bit
- main1 (entry-points)

On the board (zybo at least), with theses files in /boot, run:
```
cd /boot
cat libaccelerator.so.bit > /dev/xdevcfg
export LD_LIBRARY_PATH=.
./main1
```

## Author

* Author: Thibaut Marty
* Year: 2018
* License: MIT

## TODO

* The clocking wizard code is a bit old, I have a better version. Upgrade this one.
* Idem for randf() function
* Document randf() function
