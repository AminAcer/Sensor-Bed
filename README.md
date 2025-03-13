# Sandbox

## Deploy to an esp32s3 board
- Make sure you're using idf.py v5.4
- `git submodule update --init --recursive`
  - In `components/third-party/esp8266-oled-ssd1306/src/OLEDDisplay.h`, add `using Print::write;` in `class OLEDDisplay` (like on line 163 above `public:`)
- `export IDF_TARGET=esp32s3`
- `idf.py build`
- `idf.py -p /dev/ttyUSB0 flash monitor`
    - To exit watching the logs, use `Ctrl+]`
