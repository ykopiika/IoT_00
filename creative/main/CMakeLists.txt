idf_component_register(
        SRCS "main.c"
        SRCS "tasks.c"
        INCLUDE_DIRS "."
        REQUIRES accelerometer
        REQUIRES buttons
        REQUIRES dht11
        REQUIRES oled
        REQUIRES speaker
        REQUIRES custom_error_functions
)