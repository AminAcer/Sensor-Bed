idf_component_register(SRCS
                         "src/bno055.cpp"
                         "src/bme280.cpp"
                       INCLUDE_DIRS
                         "include"
                       REQUIRES
                         i2c
                         logger
                         constants
)

# Suppress -Wmissing-field-initializers for this component
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
