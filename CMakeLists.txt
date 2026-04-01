cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(mgl849-lab2)

find_library(I2C_LIBRARY NAMES i2c)
find_path(I2C_INCLUDE_DIR NAMES i2c/smbus.h)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS src/*.c)
add_executable(${PROJECT_NAME} ${sources})

target_compile_features(${PROJECT_NAME} PRIVATE c_std_11)

target_compile_options(${PROJECT_NAME} PRIVATE $<IF:$<CXX_COMPILER_ID:MSVC>,/W3
                                               /WX,-Wall -Wextra -Wpedantic>)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
          ${CMAKE_CURRENT_LIST_DIR}/include/control
          ${CMAKE_CURRENT_LIST_DIR}/include/display
          ${CMAKE_CURRENT_LIST_DIR}/include/metrics
          ${CMAKE_CURRENT_LIST_DIR}/include/network
          ${I2C_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE ${I2C_LIBRARY})
