# freezer
Temperature controller relay for a freezer with broken thermostat.

It keeps power on while temperature is above TEMP_MAX (-17&deg;C) and turns it off when temperature reaches TEMP_MIN (-20&deg;C).
I use the &deg; sign of the display as "power on" indicator.

# Hardware used

* Arduino Pro Mini clone
* Dallas [DS18B20](https://www.maximintegrated.com/en/products/analog/sensors-and-sensor-interface/DS18B20.html) 1-wire thermometer.
* Cheap USB charger as AC/DC power supply
* A TM1637-based 4-digit 7-segment LED display
* 5VDC control / 240VAC switching relay

# Schematic

This is how the DC parts are connected. The power supply goes on the lower +/- leads, and the AC bits obviously connect to the relay.

![Image of tinytemp module](https://github.com/zagor/freezer/blob/master/freezer.png)
