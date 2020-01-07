set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake) # Arduino Toolchain

set(ARDUINO_C_FLAGS "-std=gnu++14 -g -Os -w -ffunction-sections -fdata-sections -MMD")
set(ARDUINO_CXX_FLAGS "${ARDUINO_C_FLAGS} -fno-exceptions -fno-threadsafe-statics")

cmake_minimum_required(VERSION 3.0)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(SmartHome C CXX)

#print_board_list()
#print_board_settings(mini)

# add libraries to project
link_directories(${ARDUINO_SDK_PATH}/libraries)
link_directories(${CMAKE_SOURCE_DIR}/libraries)
link_directories(C:\\Arduino\\libraries)

# add the project directory into build
add_subdirectory(exmaple)
#add_subdirectory(outside-temp)
#add_subdirectory(greenhouse)
#add_subdirectory(gateway)
