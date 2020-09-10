#include "main.h"

static void print_page_one(t_data *db)
{
    char str[17];
    db->is_page_two = false;
    clear_oled(&db->oled);
    sprintf(str, "Temperature %dC", db->tem);
    str_to_oled(&db->oled, str);
    display_pixels(&db->oled);
}

static void print_page_two(t_data *db)
{
    char str[15];
    db->is_page_two = true;
    clear_oled(&db->oled);
    sprintf(str, "Humidity %d%%", db->hum);
    str_to_oled(&db->oled, str);
    display_pixels(&db->oled);
}

void check_buttons(void *db)
{
    _Bool on = true;
    t_data *d = (t_data*)db;
    while(on)
    {
        d->btn1.is_press = !gpio_get_level(BUTTON_1);
        d->btn2.is_press = !gpio_get_level(BUTTON_2);
        toggle_button(&d->btn1);
        toggle_button(&d->btn2);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void check_dht11(void *db)
{
    uint8_t tem = 0;
    uint8_t hum = 0;
    _Bool on = true;
    t_data *d = (t_data*)db;
    while(on)
    {
        get_value_dht11(&tem, &hum);
        if ((tem != d->tem) || (hum != d->hum))
        {
            if ((tem != d->tem) && (tem != 0))
            {
                d->is_updt_tem = true;
                d->tem = tem;
                printf("---TEMP---\n");
            }
            else
                d->is_updt_tem = false;

            if ((hum != d->hum) && (hum != 0))
            {
                d->is_updt_hum = true;
                d->hum = hum;
                printf("---HUM---\n");
            }
            else
                d->is_updt_hum = false;
        }
        printf("tem: %d, hum: %d\n", d->tem, d->hum);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void check_print_oled(void *db)
{
    t_data *d = (t_data*)db;
    _Bool on = true;
    while(on)
    {
        if (d->btn1.is_on)
        {
            if (d->is_page_two)
                print_page_one(db);
            make_bip();
            d->btn1.is_on = false;
        }
        if (d->btn2.is_on)
        {
            if (!d->is_page_two)
                print_page_two(db);
            make_bip();
            d->btn2.is_on = false;
        }
        if (d->is_updt_tem || d->is_updt_hum)
        {
            if (d->is_updt_tem && !d->is_page_two)
                print_page_one(db);
            else if (d->is_updt_hum && d->is_page_two)
                print_page_two(db);
            d->is_updt_tem = false;
            d->is_updt_hum = false;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

//void check_accelerometr(void *db)
//{
//    t_data *d = (t_data*)db;
//    _Bool on = true;
//    _Bool is_up = false;
//    while(on)
//    {
//
//    }
//}

