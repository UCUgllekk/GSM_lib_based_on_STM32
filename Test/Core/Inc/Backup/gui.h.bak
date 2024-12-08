#include "main.h"
#include "i2c.h"
#include "spi.h"
#include "usb.h"
#include "gpio.h"
#include "lcd5110.h"
#include <stdio.h>
#include "stm32f3xx_hal_conf.h"
#include "stm32f3xx_hal.h"

void SystemClock_Config(void);

void display_main_screen(LCD5110_display* lcd, int rssi);

void display_main_menu(LCD5110_display* lcd, int selected_num);

void display_messages_screen(LCD5110_display* lcd, int selected_num);

void display_call_screen(LCD5110_display* lcd);

void display_call_process(LCD5110_display* lcd);

void display_hang_up_call_process(LCD5110_display* lcd);

void display_send_sms(LCD5110_display* lcd);

void display_send_sms_input(LCD5110_display* lcd);

void SystemClock_Config(void);

void Error_Handler(void);

void MX_GPIO_Init(void);

void get_phone_number();

void keyboard_init(void);

void keyboard_scan(void);

