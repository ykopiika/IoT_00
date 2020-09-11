#include "main.h"

static void print_page(t_data *db, _Bool is_page_one)
{
    char str[17];
    db->is_page_two = !is_page_one;
    clear_pixels(&db->oled);
    if (is_page_one)
        sprintf(str, "Temperature %dC", db->tem);
    else
        sprintf(str, "Humidity %d%%", db->hum);
    str_to_oled(&db->oled, str);
}

static void write_value(t_data* d, uint8_t tem, uint8_t hum)
{
    if ((tem != d->tem) && (tem != 0))
    {
        d->is_updt_tem = true;
        d->tem = tem;
        printf(T_SLV"\t\t\t Temperature changed\n"R);
    }
    else
        d->is_updt_tem = false;

    if ((hum != d->hum) && (hum != 0))
    {
        d->is_updt_hum = true;
        d->hum = hum;
        printf(T_SLV"\t\t\t Humidity changed\n"R);
    }
    else
        d->is_updt_hum = false;
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
            write_value(d, tem, hum);
        printf("tem: %dC, hum: %d%%\n", d->tem, d->hum);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void check_print_oled(void *db) //todo 29 lines
{
    t_data *d = (t_data*)db;
    _Bool on = true;
    while(on)
    {
        if (d->btn1.is_on)
        {
            if (d->is_page_two)
                print_page(db,1);
            make_bip();
            d->btn1.is_on = false;
        }
        if (d->btn2.is_on)
        {
            if (!d->is_page_two)
                print_page(db,0);
            make_bip();
            d->btn2.is_on = false;
        }
        if (d->is_updt_tem || d->is_updt_hum)
        {
            if (d->is_updt_tem && !d->is_page_two)
                print_page(db,1);
            else if (d->is_updt_hum && d->is_page_two)
                print_page(db,0);
            d->is_updt_tem = false;
            d->is_updt_hum = false;
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

