# Sandbox

## How to deploy onto an esp32s3 board
- `git submodule update --init --recursive   # Get deps`
  - In `components/esp8266-oled-ssd1306/src/OLEDDisplay.h`, add `using Print::write;` in `class OLEDDisplay` (like on line 163 above `public:`)
- `export IDF_TARGET=esp32s3                 # Set your target processor`
- `idf.py build                              # Build your program`
- `idf.py -p /dev/ttyUSB0 flash monitor      # Flash board with program and monitor logs`
    - To exit watching the logs, use `Ctrl+]`
