# Monitoring Covid 19 Aerosol through CO2 Concentration
-------

## Hardware:
    
- ESP32
- MH-Z19 CO2 sensor
- 6x 10mm LEDs
- 68 Ohm resistors
- PCB or perfboard

Scematic

## Code

####    onlyled

- only show the CO2 level on the six leds

## flashing firmware to ESP32

- download Arduino IDE
- install ESP32 core
- upload program

## TODO

- data to database (influxdb) via WiFi
- display data with grafana