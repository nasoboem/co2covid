# Monitoring Covid 19 Aerosol through CO2 Concentration


## Hardware:
    
- ESP32
- MH-Z19 CO2 sensor
- 6x 10mm LEDs
- 6x 68 Ohm resistors
- PCB or perfboard

![assembled PCB](https://github.com/brouwerb/co2covid/blob/master/hardware/picture.jpg?raw=true)


## Code

####    onlyled

- show the CO2 level on the six leds

## Schematic

![Schematic](https://github.com/brouwerb/co2covid/blob/master/hardware/schematic.png?raw=true)

## PCB

look at my EasyEDA project:
https://easyeda.com/koning.unauwen/co2measure_copy

## flashing firmware to ESP32

- download Arduino IDE
- install ESP32 core
- upload program

## TODO

- data to database (influxdb) via WiFi
- display data with grafana

## contribute

If something is wrong, not clear, doesn't work or could be designed better, feel free to create an issue.