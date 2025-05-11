idf_component_register(SRCS
                         "src/i2c.cpp"
                       INCLUDE_DIRS
                         "include"
                       REQUIRES
                         esp_driver_i2c
                         logger
                         constants
)

# Suppress -Wmissing-field-initializers for this component
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
