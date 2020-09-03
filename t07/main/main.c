#include "oled.h"

void app_main() {
    uint8_t buffer[LCDWIDTH * LCDHEIGHT];
    init_oled(buffer);


    memset(&buffer, 0, sizeof(buffer));
    str_to_oled(buffer,
                "All the greatest technological inventions created by man - the plane, the car, the computer - are more indicative of his laziness than of high intelligence.");
//    write_lcd(buffer);
//    for (int i = 0; i < 64; ++i) {
//        for (int j = 0; j < 124; ++j) {
//            draw_pixel(j,i,1);
//            write_lcd(buffer);
//        }
//    }
}












