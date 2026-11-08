# Smart Irrigation System (DHT22, Soil Sensor, Buzzer and LCD)

An Arduino-based automatic irrigation prototype that monitors soil moisture and ambient temperature/humidity. When the soil reading indicates dry conditions, it activates a water pump through a relay for a fixed period and provides buzzer and LCD feedback.

## Features

- Reads soil moisture through an analog input.
- Reads temperature and relative humidity using a DHT22.
- Displays sensor readings and system status on a 16x2 I2C LCD.
- Switches a pump through a relay when the soil reading is above the dry threshold.
- Uses a buzzer for startup loading feedback and while watering.
- Includes a 15-second minimum interval between completed pump cycles.

## Hardware

- Arduino-compatible board
- DHT22 temperature and humidity sensor
- Analog soil-moisture sensor
- 16x2 I2C LCD (configured for address `0x27`)
- Relay module suitable for the pump
- Buzzer
- Water pump and suitable external power supply
- Jumper wires

## Pin connections

| Component | Arduino pin / setting |
|---|---|
| DHT22 data | D2 |
| Soil sensor analog output | A0 |
| Buzzer | D3 |
| Relay module input | D4 |
| I2C LCD SDA/SCL | Board's I2C SDA/SCL pins |
| LCD address | `0x27` |

The sketch uses the `DHT` and `LiquidCrystal_I2C` libraries. Install compatible versions through the Arduino IDE Library Manager.

## Soil thresholds and pump behavior

The current sketch uses raw analog readings and these conditions:

- Above `700`: soil is treated as dry. If the pump protection interval has elapsed, the pump runs for 9 seconds.
- Below `400`: soil is treated as wet and the relay is switched off.
- From `400` through `700`: soil is treated as good and the relay is switched off.

The pump protection interval is configured as 15,000 ms. The pump output is configured active-HIGH (`HIGH` turns the relay on and `LOW` turns it off). Confirm that this matches your relay module before connecting a pump.

## Setup

1. Open `Smart_irri_dht_buzzer_lcd.ino` in the Arduino IDE.
2. Install the `DHT sensor library` and a compatible `LiquidCrystal_I2C` library.
3. Select your Arduino board and serial port.
4. Check the wiring and the LCD I2C address.
5. Upload the sketch.
6. Open Serial Monitor at 9600 baud to view soil readings and DHT errors.

## Important safety notes

- Do not power a pump directly from an Arduino GPIO pin.
- Use a correctly rated relay or driver and a separate power supply suitable for the pump.
- Keep water away from the Arduino and exposed electrical connections.
- Verify relay polarity and test the system without the pump connected before operating it.
- Calibrate the `400` and `700` soil thresholds for your particular sensor and soil. Raw analog values vary by sensor, supply voltage, and soil conditions.
- The sketch has not been hardware-tested as part of this repository preparation.

## License

MIT License. See [LICENSE](LICENSE).
