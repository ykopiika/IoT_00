idf_component_register(
        SRCS "main.c"
        SRCS "tasks_accel_buttons.c"
        SRCS "tasks_dht_oled.c"
        INCLUDE_DIRS "."
        REQUIRES accelerometer
        REQUIRES buttons
        REQUIRES dht11
        REQUIRES oled
        REQUIRES speaker
        REQUIRES custom_error_functions
)