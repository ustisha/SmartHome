set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake) # Arduino Toolchain

cmake_minimum_required(VERSION 2.8)
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
add_subdirectory(outside-temp)
add_subdirectory(gateway)
