idf_component_register(SRCS
                         "src/bno055.cpp"
                         "src/bme280.cpp"
                         "src/ada254.cpp"
                       INCLUDE_DIRS
                         "include"
                       REQUIRES
                         protocols
                         logger
                         constants
)

# Suppress -Wmissing-field-initializers for this component
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-missing-field-initializers)
