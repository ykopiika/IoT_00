#include "oled.h"

void app_main() {
    t_oled  oled;
    init_oled(&oled);
    str_to_oled(&oled,"Hello, World!");
}
