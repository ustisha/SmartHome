# Set a variable for commands below
set(PROJECT_NAME main-display)
#set(ARDUINO_DEBUG Ture)

# Define your project and language
project(${PROJECT_NAME} C CXX)
set(${PROJECT_NAME}_SRCS src/main.cpp)

set(${PROJECT_NAME}_BOARD mega)
set(${PROJECT_NAME}_BOARD_CPU atmega2560)

generate_arduino_library(mega_Adafruit_I2CDevice
        SRCS ${CMAKE_SOURCE_DIR}/libraries/Adafruit_BusIO/Adafruit_BusIO_Register.cpp
        HDRS ${CMAKE_SOURCE_DIR}/libraries/Adafruit_BusIO/Adafruit_BusIO_Register.h
        BOARD mega)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT COM3)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
