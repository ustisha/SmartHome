# Set a variable for commands below
set(PROJECT_NAME outside-temp)
#set(ARDUINO_DEBUG Ture)

# Define your project and language
project(${PROJECT_NAME} C CXX)
set(${PROJECT_NAME}_SRCS src/main.cpp)

set(${PROJECT_NAME}_BOARD mini)
set(${PROJECT_NAME}_BOARD_CPU atmega328)
set(mini.build.mcu "atmega328p")
set(mini.upload.speed "57600")

# Define the port for uploading code to the Arduino
set(${PROJECT_NAME}_PORT COM6)

set(LoRa_RECURSE True)
set(BH1750_RECURSE True)
set(BME280_RECURSE True)
set(DallasTemperature_RECURSE True)
set(OneWire_RECURSE True)
set(OneWireTemperature_RECURSE True)

# Command to generate code arduino firmware (.hex file)
generate_arduino_firmware(${PROJECT_NAME})
