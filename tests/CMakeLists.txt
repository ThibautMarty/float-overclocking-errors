# These tests use google tests framework.
# If you don't know it (or similar framework), learn and use it:
# https://github.com/google/googletest/blob/master/googletest/docs/Primer.md

find_package(GTest REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

# Use host compiler
set(CMAKE_CXX_COMPILER g++)

remove_definitions(-DENABLE_CLKWIZ)

# Enable test-specific code (scoped here, will not affect real hardware)
add_definitions(-DTESTS)

add_executable(runTests
  tests.cpp             # a tests file
  tools.cpp             # provided tests for tools
  stubs.cpp             # stubs for sds_lib
  ../hw/accelerator.cpp # accelerator code
  ../hw/wrapper.cpp     # accelerator wrapper code
  ../src/golden.cpp     # some golden functions to compare with
  ../src/io.cpp
)

target_link_libraries(runTests
  ${GTEST_BOTH_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

# Include simulation headers (e.g ap_int.h)
target_include_directories(
  runTests PRIVATE
  "${XILINXPATH}/SDx/${SDXVERSION}/target/x86/include/"
  "${XILINXPATH}/Vivado/${SDXVERSION}/include/"
)

# target to run tests
add_custom_target(check
  COMMAND ./runTests
  DEPENDS runTests
)

# Build and run tests before building accelerator
add_dependencies(
  accelerator
  check
)
