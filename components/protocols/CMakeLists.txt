idf_component_register(SRCS
                         "src/i2c_interface.cpp"
                         "src/spi_interface.cpp"
                         "src/uart_interface.cpp"
                       INCLUDE_DIRS
                         "include"
                       REQUIRES
                         fatfs
                         esp_driver_i2c
                         esp_driver_spi
                         logger
                         constants
)

# Suppress -Wmissing-field-initializers for this component
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
