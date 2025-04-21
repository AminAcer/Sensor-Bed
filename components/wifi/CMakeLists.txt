idf_component_register(SRCS
                         "src/wifi.cpp"
                       INCLUDE_DIRS
                         "include"
                       REQUIRES
                         constants
                         esp_wifi
                         display
                         driver
                         lwip
                         nvs_flash
                         esp_event
                         esp_netif
)

# Suppress -Wmissing-field-initializers for this component
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
