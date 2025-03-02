idf_component_register(SRCS "main.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES arduino-esp32 esp8266-oled-ssd1306
                    PRIV_REQUIRES cxx)
