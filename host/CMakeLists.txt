# Use host compiler
set(CMAKE_CXX_COMPILER g++)

remove_definitions(-DENABLE_CLKWIZ)

# Clocking wizard simulation
add_executable(clkwizsim
  clkwizsim.cpp
  ../src/clkwiz.cpp
)
target_include_directories(
  clkwizsim PRIVATE
  "${XILINXPATH}/SDx/${SDXVERSION}/target/aarch32-linux/include/"
  "${XILINXPATH}/Vivado/${SDXVERSION}/include/"
)

# Random floating-point simulation
add_executable(randsim
  randsim.cpp
  ../src/io.cpp
)
target_include_directories(
  randsim PRIVATE
  "${XILINXPATH}/SDx/${SDXVERSION}/target/aarch32-linux/include/"
  "${XILINXPATH}/Vivado/${SDXVERSION}/include/"
)
