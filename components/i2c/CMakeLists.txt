idf_component_register(SRCS
                         "src/I2C_Interface.cpp"
                       INCLUDE_DIRS
                         "include"
                       REQUIRES
                         esp_driver_i2c
                         logger
                         constants
)

# Suppress -Wmissing-field-initializers for this component
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
