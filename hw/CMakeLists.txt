# Flags passed to sds++
# -hls-target-flags allow using cosimulation, see:
# https://forums.xilinx.com/t5/Vivado-High-Level-Synthesis-HLS/Unable-to-build-C-RTL-cosimulation-executable-with-SDx-2018-2/m-p/934604/highlight/true#M15521
set(SDS_FLAGS
  "-hls-target-flags \" \" \
  ${SDSARGS} -sds-pf ${PLATFORM}"
)
# optionally limit resource usage with:
# -maxthreads 2 -maxjobs 2 \

# Hardware function(s)
# Use -files between -sds-hw and -sds-end to add more hardware code files.
if(NOT NOHW)
  set(SDS_FLAGS
    "${SDS_FLAGS} \
    -sds-hw hw_toplevel hw/accelerator.cpp -clkid ${CLKID} -sds-end"
  )
endif()

if(NOT HOSTBUILD)
  set(CMAKE_CXX_COMPILER sds++)

  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} ${SDS_FLAGS}"
  )

  set(CMAKE_LD_FLAGS
    "${SDS_FLAGS} -dmclkid ${DMCLKID}"
  )
endif()


# Note: sds compiler needs the file convention %.o <=> %.cpp
# this enables that instead of CMake's default %.cpp.o
# https://cmake.org/pipermail/cmake/2012-August/051638.html
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

# Tell cmake to remove _sds directory on clean
set_directory_properties(PROPERTIES
  ADDITIONAL_MAKE_CLEAN_FILES
  "_sds"
)

# Accelerator is compiled as a shared library
add_library(accelerator SHARED
  accelerator.cpp
  wrapper.cpp
)

set_target_properties(accelerator PROPERTIES
  PUBLIC_HEADER ../inc/wrapper.h
)

if(HOSTBUILD)
  # Note: this is done automatically when using sds++
  target_include_directories(
    accelerator PRIVATE
    "${SDX_PREFIX}/include/"
    "${XILINXPATH}/Vivado/${SDXVERSION}/include/"
  )

  # Note: x86/lib/libsds_lib.a does not work. Build a stubs library
  # Note: a STATIC (without -fPIC) cannot be used in a shared library
  add_library(sds_lib SHARED
    ../tests/stubs.cpp
  )
endif()

target_link_libraries(accelerator
  sds_lib
  ${CMAKE_THREAD_LIBS_INIT}
)

# Simulation targets:
# FIXME: the should depend on accelerator.o (but not on accelerator)
# FIXME: accelerator should depend on these to pass (useless to do full synthesis otherwise)
# FIXME: we could use add_custom_command + PRE_LINK, but then we don't have direct access to the targets
set(sim_command
  ruby ${CMAKE_SOURCE_DIR}/sim/sim.rb
  ${CMAKE_SOURCE_DIR} # absolute path of source directory
  ${SDS_FLAGS}        # flags containing hardware functions
  tests/stubs.cpp     # stubs for sds_lib
  sim/main.cpp        # sources files
  hw/wrapper.cpp
  src/golden.cpp
  src/io.cpp
)
add_custom_target(csim
  COMMAND ${sim_command} csim_design
  WORKING_DIRECTORY ./_sds/vhls/
)
add_custom_target(cosim
  COMMAND ${sim_command} cosim_design
  WORKING_DIRECTORY ./_sds/vhls/
)
