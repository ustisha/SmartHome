# Set a variable for commands below
set(PROJECT_NAME outside-temp)
#set(ARDUINO_DEBUG Ture)

# Define your project and language
project(${PROJECT_NAME} C CXX)
set(${PROJECT_NAME}_SRCS src/main.cpp)

set(${PROJECT_NAME}_BOARD pro)
set(${PROJECT_NAME}_BOARD_CPU 8MHzatmega328)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT COM4)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
