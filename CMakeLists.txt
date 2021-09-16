cmake_minimum_required(VERSION 3.3)

project(mmult-float-overclock-measure CXX C)

# Use this to append search path for CMake's find* module (for e.g local installation of dependencies)
# list(APPEND CMAKE_PREFIX_PATH "~/bin/")

# Configuration options + file generation
include(options.cmake)
configure_file(inc/options.h.in inc/options.h)
configure_file(inc/options.sw.h.in inc/options.sw.h)
include_directories(${CMAKE_BINARY_DIR}/inc)

# Needed by sds compiler
find_package(Threads)

# Ubiquitous compilation option
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS
  "-O3 -Wall \
  -Wno-unknown-warning-option \
  -Wno-unused-label \
  -Wno-unknown-pragmas \
  -Wno-uninitialized \
  -Wno-int-in-bool-context -Wno-mismatched-tags" # <= avoid warnings in Xilinx headers
)

if(HOSTBUILD)
  set(SDX_PREFIX "${XILINXPATH}/SDx/${SDXVERSION}/target/x86")
else()
  set(SDX_PREFIX "${XILINXPATH}/SDx/${SDXVERSION}/target/aarch32-linux")
endif()

# Remove Xilinx warning on hls_fpo.h
add_definitions(-DHLS_NO_XIL_FPO_LIB)

if(NOT HOSTBUILD AND NOT NOHW)
  # Enable real clkwiz for executables, will be disabled in tests
  add_definitions(-DENABLE_CLKWIZ)
endif()

# Headers path
include_directories(inc)

# Hardware code subdirectory
add_subdirectory(hw)

# Tests subdirectory
if(TESTS)
  add_subdirectory(tests)
endif()

# Host code subdirectory
add_subdirectory(host)

if(NOT HOSTBUILD)
  # Use cross-compiler for everything (overwritten in other CMakeLists.txt)
  set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
endif()

# Add link path for sds_lib
link_directories(
  "${SDX_PREFIX}/lib/"
)
# Add include path for sds_lib and Vivado headers
include_directories(
  "${SDX_PREFIX}/include/"
  "${XILINXPATH}/Vivado/${SDXVERSION}/include/"
)

# Overclocking for foat measurement executable
add_executable(main1
  src/main1.cpp
  src/golden.cpp
  src/io.cpp
  src/clkwiz.cpp
)
target_link_libraries(main1
  accelerator
  sds_lib
)

# Same as randsim, but for target
add_executable(randsimt
  host/randsim.cpp
  src/io.cpp
)
target_link_libraries(randsimt
  sds_lib
)
