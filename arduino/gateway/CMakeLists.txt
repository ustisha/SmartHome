# Set a variable for commands below
set(PROJECT_NAME gateway)

# Define your project and language
project(${PROJECT_NAME} C CXX)

set(${PROJECT_NAME}_BOARD uno)

# Define the source code
set(${PROJECT_NAME}_SRCS src/main.cpp)

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT COM3)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
