# These options can be set with command line: e.g cmake . -DTESTS=OFF
# or by UI (e.g ccmake)

# Check tests before actual build
option(TESTS "Set to ON to build and run tests" ON)

# Build everything for host, useful to skip long hardware synthesis but testing executable compilation
# (assuming host means x86)
option(HOSTBUILD "Set to ON to build everything natively, without hardware" OFF)

# Build everything as usual, but without hardware function to skip long hardware synthesis but testing executable compilation
# (Use platform with prebuilt bitstream for fast build time)
option(NOHW "Set to ON to build everything natively, without hardware" OFF)

# Clocking wizard input clock (AXI clock)
set(INPUT_CLK 100.f CACHE STRING "Clocking wizard iput clock (MHz)")
set(SAFE_CLK 100.f CACHE STRING "Static safe frequency (MHz) - only used for speedup measurements")

# Datamover and accelerator clock IDs
set(DMCLKID 0 CACHE STRING "Data mover clock ID")
set(CLKID 1 CACHE STRING "Accelerator clock ID")

# Version of SDx for headers/libraries
set(SDXVERSION "2018.2" CACHE STRING "SDx version for headers/libraries (you still need to source settings64.sh)")
set(XILINXPATH "/opt/Xilinx" CACHE STRING "Path to Xilinx's tool installation directory")

# Target SDSoC platform, either name for a standard one, or an path
# (in which case it will be resolved to an absolute path)
# set(PLATFORM "zc706" CACHE STRING "SDx platform (path or standard name)") # TODO do not commit
set(PLATFORM "zc706oc/output/zc706oc/export/zc706oc" CACHE STRING "SDx platform (path or standard name)")
set(SDSARGS "--remote_ip_cache ~/ip_cache -sds-sys-config linux -sds-proc linux" CACHE STRING "sds compiler-specific arguments")

# Option
set(DATA_T "float" CACHE STRING "Data type for accelerator I/O")
set(N 128 CACHE STRING "Array size for accelerator I/O")
option(NODSP "Do not use DSP for dot product" OFF)

# Operation choice
set(OPERATION "OPADD" CACHE STRING "Operation: must be either OPADD, OPEWMUL, OPMAC or OPMUL")
