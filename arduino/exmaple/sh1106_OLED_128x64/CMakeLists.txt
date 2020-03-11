# Set a variable for commands below
set(PROJECT_NAME example_sh1106_OLED_128x64)
#set(ARDUINO_DEBUG Ture)

# Define your project and language
project(${PROJECT_NAME} C CXX)
set(${PROJECT_NAME}_SRCS src/main.cpp)

set(${PROJECT_NAME}_BOARD nano)
set(${PROJECT_NAME}_BOARD_CPU atmega328old)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT COM7)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
