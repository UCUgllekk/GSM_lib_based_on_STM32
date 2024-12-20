/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
extern "C"{
	#include "main.h"
	#include "i2c.h"
	#include "spi.h"
	#include "usart.h"
	#include "usb.h"
	#include "gpio.h"
	#include "lcd5110.h"
}
#include "GSMLibrary.hpp"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <cstring>
#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <stack>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
LCD5110_display lcd;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//void handle_state_machine(GSM_Module& gsm);
//GSMState parse_command(const std::string& command, GSM_Module gsm);
//std::string read_keyboard_input();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char keypad_scan(void)
{
  char keys[4][4] = {{'1', '2', '3', 'A'},
           {'4', '5', '6', 'B'},
           {'7', '8', '9', 'C'},
           {'*', '0', '#', 'D'}};

  for(int i = 0; i < 4; i++)
  {
  // Set current column as output and low
  switch(i)
  {
    case 0:
    HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_SET);
    break;



    case 1:
    HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_SET);
    break;

    case 2:
    HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_SET);
    break;

    case 3:
    HAL_GPIO_WritePin(COL_1_GPIO_Port, COL_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_2_GPIO_Port, COL_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_3_GPIO_Port, COL_3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_4_GPIO_Port, COL_4_Pin, GPIO_PIN_RESET);
    break;
  }
  // Read current rows
  if(HAL_GPIO_ReadPin(ROW_1_GPIO_Port, ROW_1_Pin) == GPIO_PIN_RESET)
    return keys[0][i];
  if(HAL_GPIO_ReadPin(ROW_2_GPIO_Port, ROW_2_Pin) == GPIO_PIN_RESET)
    return keys[1][i];
  if(HAL_GPIO_ReadPin(ROW_3_GPIO_Port, ROW_3_Pin) == GPIO_PIN_RESET)
    return keys[2][i];
  if(HAL_GPIO_ReadPin(ROW_4_GPIO_Port, ROW_4_Pin) == GPIO_PIN_RESET)
    return keys[3][i];
  }
  return 0;
}

std::string entered_number = "";

std::string read_input(LCD5110_display* lcd) {
    char key;
    int cursor_x = 21;
    unsigned long last_key_time = 0;
    unsigned long debounce_delay = 200;

    while (true) {
        key = keypad_scan();
        if (key != 0 && (HAL_GetTick() - last_key_time > debounce_delay)) {
            last_key_time = HAL_GetTick();

            if (key == '#') {
                return entered_number;
            }
            if (key == '*') {
                if (!entered_number.empty()) {
                    entered_number.pop_back();
                    cursor_x = std::max(21, cursor_x - 6);
                    LCD5110_set_cursor(cursor_x, 34, lcd);
                    LCD5110_print(" ", BLACK, lcd);
                }
            } else if (key >= '0' && key <= '9') {
                if (entered_number.length() < 10) {
                    entered_number.push_back(key);

                    LCD5110_set_cursor(cursor_x, 34, lcd);
                    char str[2] = {key, '\0'};
                    LCD5110_print(str, BLACK, lcd);
                    cursor_x += 6;
                }
            } else {
                LCD5110_set_cursor(0, 40, lcd);
                LCD5110_print("Invalid input", BLACK, lcd);
                HAL_Delay(1000);
                LCD5110_set_cursor(0, 40, lcd);
                LCD5110_print("              ", BLACK, lcd);
            }

            HAL_Delay(150);
            LCD5110_refresh(lcd);
        }
    }
}

std::string entered_message = "";
void read_input_message(LCD5110_display* lcd) {
    int cursor_x = 3;
    int cursor_y = 34;

    LCD5110_refresh(lcd);
    LCD5110_set_cursor(3, 34, lcd);
    for (int i = 0; i < 12; i++) {
        LCD5110_print(".", BLACK, lcd);
    }
    LCD5110_refresh(lcd);

    HAL_Delay(500);
    for (int i = 0; i < 12; i++) {
        LCD5110_set_cursor(cursor_x, cursor_y, lcd);

        LCD5110_print("0", BLACK, lcd);
        LCD5110_refresh(lcd);
        cursor_x += 6;

        HAL_Delay(500);
    }
}

void display_main_screen(LCD5110_display* lcd, int menu, GSM_Module gsm) {
  LCD5110_clear_scr(lcd);

// Kitty

  LCD5110_putpix(3, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(4, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(5, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(19, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(20, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 3, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(6, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(7, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(17, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(18, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 4, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(8, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(9, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(10, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(11, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(12, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(13, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(14, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(15, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(16, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 5, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(6, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(7, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(17, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(18, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 6, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(5, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(19, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 7, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(4, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(20, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 8, BLACK, &lcd->hw_conf);



  LCD5110_putpix(2, 9, BLACK, &lcd->hw_conf);
  LCD5110_putpix(3, 9, BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 9, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 9, BLACK, &lcd->hw_conf);

  LCD5110_putpix(3, 10, BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 10, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 11, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 11, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 12, BLACK, &lcd->hw_conf);
  LCD5110_putpix(6, 12, BLACK, &lcd->hw_conf);
  LCD5110_putpix(7, 12, BLACK, &lcd->hw_conf);
  LCD5110_putpix(17, 12, BLACK, &lcd->hw_conf);
  LCD5110_putpix(18, 12, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 12, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(6, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(7, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(10, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(12, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(14, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(17, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(18, 13, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 13, BLACK, &lcd->hw_conf);

  LCD5110_putpix(2, 14, BLACK, &lcd->hw_conf);
  LCD5110_putpix(11, 14, BLACK, &lcd->hw_conf);
  LCD5110_putpix(13, 14, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 14, BLACK, &lcd->hw_conf);


  LCD5110_putpix(2, 15, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 15, BLACK, &lcd->hw_conf);

  LCD5110_putpix(3, 16, BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 16, BLACK, &lcd->hw_conf);

  LCD5110_putpix(3, 17, BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 17, BLACK, &lcd->hw_conf);

  LCD5110_putpix(4, 18, BLACK, &lcd->hw_conf);
  LCD5110_putpix(5, 18, BLACK, &lcd->hw_conf);
  LCD5110_putpix(19, 18, BLACK, &lcd->hw_conf);
  LCD5110_putpix(20, 18, BLACK, &lcd->hw_conf);

  LCD5110_putpix(5, 19, BLACK, &lcd->hw_conf);
  LCD5110_putpix(19, 19, BLACK, &lcd->hw_conf);
  LCD5110_putpix(23, 19, BLACK, &lcd->hw_conf);
  LCD5110_putpix(24, 19, BLACK, &lcd->hw_conf);

  LCD5110_putpix(4, 20, BLACK, &lcd->hw_conf);
  LCD5110_putpix(20, 20, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 20, BLACK, &lcd->hw_conf);
  LCD5110_putpix(25, 20, BLACK, &lcd->hw_conf);

  LCD5110_putpix(4, 21, BLACK, &lcd->hw_conf);
  LCD5110_putpix(20, 21, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 21, BLACK, &lcd->hw_conf);
  LCD5110_putpix(25, 21, BLACK, &lcd->hw_conf);

  LCD5110_putpix(3, 22,  BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 22,  BLACK, &lcd->hw_conf);
  LCD5110_putpix(24, 22, BLACK, &lcd->hw_conf);

  LCD5110_putpix(3, 23, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 23, BLACK, &lcd->hw_conf);
  LCD5110_putpix(23, 23, BLACK, &lcd->hw_conf);

  LCD5110_putpix(3, 24, BLACK, &lcd->hw_conf);
  LCD5110_putpix(21, 24, BLACK, &lcd->hw_conf);
  LCD5110_putpix(22, 24, BLACK, &lcd->hw_conf);

  LCD5110_putpix(4, 25, BLACK, &lcd->hw_conf);
  LCD5110_putpix(20, 25, BLACK, &lcd->hw_conf);

  LCD5110_putpix(5, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(6, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(7, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(8, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(9, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(10, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(11, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(12, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(13, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(14, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(15, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(16, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(17, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(18, 26, BLACK, &lcd->hw_conf);
  LCD5110_putpix(19, 26, BLACK, &lcd->hw_conf);

///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

  LCD5110_putpix(81, 0, BLACK, &lcd->hw_conf);
  LCD5110_putpix(82, 0, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 0, BLACK, &lcd->hw_conf);

  LCD5110_putpix(81, 1, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 1, BLACK, &lcd->hw_conf);


  LCD5110_putpix(77, 2, BLACK, &lcd->hw_conf);
  LCD5110_putpix(78, 2, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 2, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 2, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 2, BLACK, &lcd->hw_conf);

  LCD5110_putpix(77, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 3, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 3, BLACK, &lcd->hw_conf);

  LCD5110_putpix(73, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(74, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(75, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(77, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 4, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 4, BLACK, &lcd->hw_conf);

  LCD5110_putpix(73, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(75, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(77, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 5, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 5, BLACK, &lcd->hw_conf);

  LCD5110_putpix(69, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(70, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(71, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(73, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(75, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(77, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 6, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 6, BLACK, &lcd->hw_conf);

  LCD5110_putpix(69, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(71, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(73, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(75, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(77, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 7, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 7, BLACK, &lcd->hw_conf);

  LCD5110_putpix(69, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(70, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(71, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(73, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(74, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(75, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(77, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(78, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(79, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(81, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(82, 8, BLACK, &lcd->hw_conf);
  LCD5110_putpix(83, 8, BLACK, &lcd->hw_conf);

  int signal = gsm.get_signal_strength();

	  if (signal >= 0) {
		LCD5110_putpix(70, 7, BLACK, &lcd->hw_conf);
	      }

		  if (signal >= 9) {
			LCD5110_putpix(74, 7, BLACK, &lcd->hw_conf);
			LCD5110_putpix(74, 6, BLACK, &lcd->hw_conf);
			LCD5110_putpix(74, 5, BLACK, &lcd->hw_conf);
		  }

		  if (signal >= 14) {
			LCD5110_putpix(78, 7, BLACK, &lcd->hw_conf);
			LCD5110_putpix(78, 6, BLACK, &lcd->hw_conf);
			LCD5110_putpix(78, 5, BLACK, &lcd->hw_conf);
			LCD5110_putpix(78, 4, BLACK, &lcd->hw_conf);
			LCD5110_putpix(78, 3, BLACK, &lcd->hw_conf);
		  }

		  if (signal >= 19) {
			LCD5110_putpix(82, 7, BLACK, &lcd->hw_conf);
			LCD5110_putpix(82, 6, BLACK, &lcd->hw_conf);
			LCD5110_putpix(82, 5, BLACK, &lcd->hw_conf);
			LCD5110_putpix(82, 4, BLACK, &lcd->hw_conf);
			LCD5110_putpix(82, 3, BLACK, &lcd->hw_conf);
			LCD5110_putpix(82, 2, BLACK, &lcd->hw_conf);
			LCD5110_putpix(82, 1, BLACK, &lcd->hw_conf);
		  }

	int y_position = 35;
	int rect_height = 8;
	int rect_width = 8;

  if (menu == 1) {
    for (int x = 0; x < rect_width; x++) {
      for (int y = y_position; y < y_position + rect_height; y++) {
        LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
      }
    }


    LCD5110_set_cursor(2, y_position, lcd);
    LCD5110_print("Menu", WHITE, lcd);
  } else {
    LCD5110_set_cursor(2, y_position, lcd);
    LCD5110_print("Menu", BLACK, lcd);
  }

  for (int x = 0; x < 26; x++) {
          LCD5110_putpix(x, 42, BLACK, &lcd->hw_conf);
   }

  std::pair<std::string, std::string> date_and_time = gsm.get_date_and_time();

  LCD5110_set_cursor(53, 13, lcd);
  LCD5110_print(((date_and_time.second).substr(0, 5)).c_str(), BLACK, lcd);

  LCD5110_set_cursor(36, 25, lcd);
  LCD5110_print(date_and_time.first.c_str(), BLACK, lcd);


  LCD5110_refresh(lcd);
}


void display_main_menu(LCD5110_display* lcd, int selected_num) {
    LCD5110_clear_scr(lcd);

    LCD5110_set_cursor(31, 0, lcd);
    LCD5110_print("Menu", BLACK, lcd);

    for (int x = 0; x < 84; x++) {
        LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
    }

    const char* menu_items[] = {"> Call", "> Messages", "> Snake"};
    const int y_positions[] = {12, 20, 28};
    const int num_items = 3;


    for (int i = 0; i < num_items; i++) {
        if (selected_num == i + 1) {
            for (int x = 0; x < 84; x++) {
                for (int y = y_positions[i]; y < y_positions[i] + 8; y++) {
                    LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
                }
            }
            LCD5110_set_cursor(0, y_positions[i], lcd);
            LCD5110_print(menu_items[i], WHITE, lcd);
        } else {
            LCD5110_set_cursor(0, y_positions[i], lcd);
            LCD5110_print(menu_items[i], BLACK, lcd);
        }
    }
    LCD5110_refresh(lcd);
}


void display_messages_screen(LCD5110_display* lcd, int selected_num) {
    LCD5110_clear_scr(lcd);

    LCD5110_set_cursor(21, 0, lcd);
    LCD5110_print("Messages", BLACK, lcd);

    for (int x = 0; x < 84; x++) {
        LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
    }

    const char* menu_items[] = {"> Send SMS", "> Received SMS"};
    const int y_positions[] = {12, 20};
    const int num_items = 2;

    for (int i = 0; i < num_items; i++){
      if (selected_num == i + 1){
        for (int x = 0; x < 84; x++){
          for (int y = y_positions[i]; y < y_positions[i] + 8; y++){
              LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
          }
        }
        LCD5110_set_cursor(0, y_positions[i], lcd);
        LCD5110_print(menu_items[i], WHITE, lcd);
      } else {
            LCD5110_set_cursor(0, y_positions[i], lcd);
            LCD5110_print(menu_items[i], BLACK, lcd);
        }
    }
    LCD5110_refresh(lcd);
}


void display_call_screen(LCD5110_display* lcd) {
    LCD5110_clear_scr(lcd);

    LCD5110_set_cursor(31, 0, lcd);
    LCD5110_print("Call", BLACK, lcd);

    for (int x = 0; x < 84; x++) {
        LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
    }

    LCD5110_set_cursor(3, 14, lcd);
    LCD5110_print("Enter number:", BLACK, lcd);

    LCD5110_set_cursor(1, 34, lcd);
    LCD5110_print("+38", BLACK, lcd);


    int x_start = 19;
    int x_end = 83;
    int y_start = 32;
    int y_end = 42;

    for (int x = x_start; x < x_end; x++) {
        LCD5110_putpix(x, y_start, BLACK, &lcd->hw_conf);
    }

    for (int x = x_start; x < x_end; x++) {
        LCD5110_putpix(x, y_end, BLACK, &lcd->hw_conf);
    }

    for (int y = y_start; y <= y_end; y++) {
        LCD5110_putpix(x_start, y, BLACK, &lcd->hw_conf);
    }

    for (int y = y_start; y <= y_end; y++) {
        LCD5110_putpix(x_end - 1, y, BLACK, &lcd->hw_conf);
    }

    LCD5110_refresh(lcd);

    std::string phone_number = read_input(lcd);

}


void display_call_process(LCD5110_display* lcd) {
    LCD5110_clear_scr(lcd);
    LCD5110_set_cursor(15, 0, lcd);
    LCD5110_print("Calling...", BLACK, lcd);

    int x_start = 0;
    int y_start = 18;

    if (entered_number.empty()) {
       LCD5110_set_cursor(x_start, y_start, lcd);
       LCD5110_print("Empty", BLACK, lcd);
    } else{

    LCD5110_set_cursor(x_start, y_start, lcd);
    LCD5110_print(entered_number.c_str(), BLACK, lcd);
    }
    LCD5110_set_cursor(0, 35, lcd);
    LCD5110_print("Press * to end", BLACK, lcd);

    LCD5110_refresh(lcd);
}



void display_hang_up_call_process(LCD5110_display* lcd){
  LCD5110_clear_scr(lcd);
  LCD5110_set_cursor(28, 0, lcd);
  LCD5110_print("00:00", BLACK, lcd);
  for (int x = 0; x < 84; x++) {
    LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
  }

  int x_start = 0;
  int y_start = 18;
  int text_width = 72;
  int text_height = 8;

  for (int x = x_start; x < x_start + text_width; x++) {
    for (int y = y_start; y < y_start + text_height; y++) {
      LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
    }
  }

  LCD5110_set_cursor(x_start, y_start, lcd);
  LCD5110_print("+380.........", WHITE, lcd);


  LCD5110_set_cursor(0, 35, lcd);
  LCD5110_print("Press * to end", BLACK, lcd);

  LCD5110_refresh(lcd);
}


void display_send_sms(LCD5110_display* lcd) {
    LCD5110_clear_scr(lcd);

    LCD5110_set_cursor(21, 0, lcd);
    LCD5110_print("Send SMS", BLACK, lcd);

    for (int x = 0; x < 84; x++) {
        LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
    }

    LCD5110_set_cursor(3, 14, lcd);
    LCD5110_print("Enter number:", BLACK, lcd);

    LCD5110_set_cursor(1, 34, lcd);
    LCD5110_print("+38", BLACK, lcd);

    int x_start = 19;
	int x_end = 83;
	int y_start = 32;
	int y_end = 42;

	for (int x = x_start; x < x_end; x++) {
		LCD5110_putpix(x, y_start, BLACK, &lcd->hw_conf);
	}

	for (int x = x_start; x < x_end; x++) {
		LCD5110_putpix(x, y_end, BLACK, &lcd->hw_conf);
	}

	for (int y = y_start; y <= y_end; y++) {
		LCD5110_putpix(x_start, y, BLACK, &lcd->hw_conf);
	}

	for (int y = y_start; y <= y_end; y++) {
		LCD5110_putpix(x_end - 1, y, BLACK, &lcd->hw_conf);
	}

    std::string phone_number = read_input(lcd);

    LCD5110_clear_scr(lcd);
    LCD5110_set_cursor(15, 0, lcd);
    LCD5110_print("Sending SMS to:", BLACK, lcd);

    LCD5110_set_cursor(0, 20, lcd);
    LCD5110_print(("+38" + phone_number).c_str(), BLACK, lcd);

    LCD5110_refresh(lcd);

}

void display_send_sms_input(LCD5110_display* lcd, GSM_Module gsm) {
    LCD5110_clear_scr(lcd);

    LCD5110_set_cursor(25, 0, lcd);
    LCD5110_print("Send SMS", BLACK, lcd);

    for (int x = 0; x < 84; x++) {
        LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
    }

    LCD5110_set_cursor(3, 14, lcd);
    LCD5110_print("Enter SMS:", BLACK, lcd);

    int x_start = 1;
    int x_end = 83;
    int y_start = 32;
    int y_end = 42;

    for (int x = x_start; x < x_end; x++) {
        LCD5110_putpix(x, y_start, BLACK, &lcd->hw_conf);
    }

    for (int x = x_start; x < x_end; x++) {
        LCD5110_putpix(x, y_end, BLACK, &lcd->hw_conf);
    }

    for (int y = y_start; y <= y_end; y++) {
        LCD5110_putpix(x_start, y, BLACK, &lcd->hw_conf);
    }
    for (int y = y_start; y <= y_end; y++) {
        LCD5110_putpix(x_end - 1, y, BLACK, &lcd->hw_conf);
    }

    LCD5110_refresh(lcd);

    read_input_message(lcd);
}


void display_sent_sms(LCD5110_display* lcd){
  LCD5110_clear_scr(lcd);
  LCD5110_set_cursor(0, 20, lcd);
  LCD5110_print("Message was sent", BLACK, lcd);


  LCD5110_refresh(lcd);
}

void display_calling(LCD5110_display* lcd) {
    LCD5110_clear_scr(lcd);
    LCD5110_set_cursor(4, 5, lcd);
    LCD5110_print("+380.........", BLACK, lcd);

    LCD5110_set_cursor(18, 20, lcd);
    LCD5110_print("*", BLACK, lcd);

    LCD5110_set_cursor(60, 18, lcd);
    LCD5110_print("#", BLACK, lcd);

    int claimTextColor = BLACK;
    int claimRectColor = WHITE;
    int cancelTextColor = BLACK;
    int cancelRectColor = WHITE;

    for (int y = 30; y <= 40; y++) {
        for (int x = 0; x < 40; x++) {
            LCD5110_putpix(x, y, claimRectColor, &lcd->hw_conf);
        }
    }
    LCD5110_set_cursor(4, 32, lcd);
    LCD5110_print("OK", claimTextColor, lcd);

    for (int y = 30; y <= 40; y++) {
        for (int x = 43; x < 82; x++) {
            LCD5110_putpix(x, y, cancelRectColor, &lcd->hw_conf);
        }
    }
    LCD5110_set_cursor(45, 32, lcd);
    LCD5110_print("Cancel", cancelTextColor, lcd);

    LCD5110_refresh(lcd);
}

const int MAX_VISIBLE_MESSAGES = 5;

void display_messages_list(LCD5110_display* lcd, GSM_Module gsm) {
    int selected_message = 0;
    int scroll_offset = 0;

    while (true) {
        LCD5110_clear_scr(lcd);

        LCD5110_set_cursor(10, 0, lcd);
        LCD5110_print("Received SMS", BLACK, lcd);

        for (int x = 0; x < 84; x++) {
            LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
        }

        for (int i = 0; i < MAX_VISIBLE_MESSAGES; ++i) {
            int msg_index = scroll_offset + i;
//            if (msg_index >= messages.size()) {
//                break; // No more messages
//            }

            int y_position = 12 + i * 8;

            std::pair<std::string, std::string> message = gsm.read_sms(msg_index);

            if (msg_index == selected_message) {
                for (int x = 0; x < 84; ++x) {
                    for (int y = y_position; y < y_position + 8; ++y) {
                        LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
                    }
                }
                LCD5110_set_cursor(0, y_position, lcd);
                LCD5110_print((("> " + message.second).substr(0, 10)).c_str(), WHITE, lcd);
            } else {
                LCD5110_set_cursor(0, y_position, lcd);
                LCD5110_print((("> " + message.second).substr(0, 10)).c_str(), BLACK, lcd);
            }
        }

        LCD5110_refresh(lcd);

        char key = keypad_scan();

        if (key == '2') {
            if (selected_message > 0) {
                --selected_message;
                if (selected_message < scroll_offset) {
                    --scroll_offset;
                }
            }
        } else if (key == '8') {
//            if (selected_message < messages.size() - 1) {
                ++selected_message;
                if (selected_message >= scroll_offset + MAX_VISIBLE_MESSAGES) {
                    ++scroll_offset;
                }
//            }
        } else if (key == '#') {
            return;
        }

        HAL_Delay(150);

    }
}


typedef enum {
    STATE_MAIN_SCREEN,
    STATE_MENU,
    STATE_CALL_INPUT,
    STATE_CALLING,
    STATE_CALL,
    STATE_INCOMING_CALL,
    STATE_MESSAGES,
    STATE_MESSAGE_NUMBER,
    STATE_MESSAGE_TEXT,
	STATE_RECIEVED,
    STATE_SNAKE_SCREEN
} SystemState;

void update_display_for_option(int option);
void update_display_for_message_option(int option);
void enter_main_screen(GSM_Module gsm);
void enter_menu();
void enter_call();
void enter_call_input();
void enter_calling();
void enter_messages();
void enter_message_number();
void enter_message_text(GSM_Module gsm);
char keypad_scan();
void handle_key_press(char key_pressed);
void enter_incomming_call();

int MAX_OPTIONS = 3;

SystemState current_state = STATE_MAIN_SCREEN;
std::stack<SystemState> state_stack;
int current_option = 0;
int current_message_option = 0;

std::vector<std::string> messages = {
    "Hello!",
    "How're you?",
    "ASAP.",
    "Don't forget the meeting.",
    "Urgent: System update required.",
    "Reminder: Code review tomorrow.",
    "Lunch at 12:00?"
};


void update_display_for_option(int option) {
    switch (option) {
        case 0:
            display_main_menu(&lcd, 1);
            break;
        case 1:
            display_main_menu(&lcd, 2);
            break;
        case 2:
            display_main_menu(&lcd, 3);
            break;
    }
}

void update_display_for_message_option(int option) {
    switch (option) {
        case 0:
            display_messages_screen(&lcd, 1);
            break;
        case 1:
            display_messages_screen(&lcd, 2);
            break;
    }
}

void push_state(SystemState state) {
    state_stack.push(state);
}

void pop_state(GSM_Module gsm) {
    if (!state_stack.empty()) {
        current_state = state_stack.top();
        state_stack.pop();

        // Update the display based on the restored state
        switch (current_state) {
            case STATE_MAIN_SCREEN:
            	display_main_screen(&lcd, 0, gsm);
                break;
            case STATE_MENU:
                display_main_menu(&lcd, 0);
                update_display_for_option(current_option);
                break;
            case STATE_CALL_INPUT:
                display_call_screen(&lcd);
                read_input(&lcd);
                break;
            case STATE_CALLING:
                display_call_process(&lcd);
                break;
            case STATE_CALL:
                display_hang_up_call_process(&lcd);
                break;
            case STATE_MESSAGES:
                display_messages_screen(&lcd, 0);
                update_display_for_message_option(current_message_option);
                break;
            case STATE_MESSAGE_NUMBER:
                display_send_sms(&lcd);
                break;
            case STATE_MESSAGE_TEXT:
                display_send_sms_input(&lcd, gsm);
                break;
            case STATE_RECIEVED:
            	display_messages_list(&lcd, gsm);
            	break;
            default:
                break;
        }
    }
}


void enter_main_screen(GSM_Module gsm) {
    display_main_screen(&lcd, 0, gsm);
    entered_number.clear();
    entered_message.clear();
    push_state(current_state);
    current_state = STATE_MAIN_SCREEN;
}

void enter_menu() {
    display_main_menu(&lcd, 0);
    update_display_for_option(current_option);
    push_state(current_state);
    current_state = STATE_MENU;
}

void enter_call_input() {
    display_call_screen(&lcd);
    read_input(&lcd);
    push_state(current_state);
    current_state = STATE_CALL_INPUT;
}

void enter_calling() {
    display_call_process(&lcd);
    push_state(current_state);
    current_state = STATE_CALLING;
}

void enter_call() {
    display_hang_up_call_process(&lcd);
    push_state(current_state);
    current_state = STATE_CALL;
}

void enter_messages() {
    display_messages_screen(&lcd, 0);
    current_message_option = 0;
    push_state(current_state);
    current_state = STATE_MESSAGES;
}

void enter_message_number() {
    display_send_sms(&lcd);
    read_input(&lcd);
    push_state(current_state);
    current_state = STATE_MESSAGE_NUMBER;
}

void enter_message_text(GSM_Module gsm) {
    display_send_sms_input(&lcd, gsm);
    read_input(&lcd);
    push_state(current_state);
    current_state = STATE_MESSAGE_TEXT;
}

void enter_recieved(GSM_Module gsm) {
	display_messages_list(&lcd, gsm);
	push_state(current_state);
	current_state = STATE_RECIEVED;

}

void enter_incomming_call() {
	display_calling(&lcd);
	push_state(current_state);
	current_state = STATE_INCOMING_CALL;
}

void handle_key_press(char key_pressed, GSM_Module gsm) {
    if (gsm.current_state == gsm.IDLE) {
        switch (current_state) {
            case STATE_MAIN_SCREEN:
                if (key_pressed == '0') {
                    enter_menu();
                }
                break;
            case STATE_MENU:
                if (key_pressed == 'A') {
                    current_option = (current_option - 1 + MAX_OPTIONS) % MAX_OPTIONS;
                    update_display_for_option(current_option);
                }
                if (key_pressed == 'B') {
                    current_option = (current_option + 1) % MAX_OPTIONS;
                    update_display_for_option(current_option);
                }
                if (key_pressed == '0') {
                    switch (current_option) {
                        case 0: enter_call_input(); break;
                        case 1: enter_messages(); break;
                    }
                }
                if (key_pressed == 'D') {
                    enter_main_screen(gsm);
                }
                if (key_pressed == 'C') {
                    pop_state(gsm);
                }
                break;
            case STATE_CALL_INPUT:
                if (key_pressed == '#') {
                	gsm.make_call(("38" + entered_number).c_str());
                    enter_calling();
                }
                if (key_pressed == 'D') {
                    enter_main_screen(gsm);
                }
                if (key_pressed == 'C') {
                    pop_state(gsm);
                }
                break;
            case STATE_CALLING:
                if (key_pressed == '#') {
                    enter_call();
                }
                if (key_pressed == '*') {
                    enter_main_screen(gsm);
                }
                break;
            case STATE_MESSAGES:
                if (key_pressed == 'A') {
                    current_message_option = (current_message_option - 1 + 2) % 2;
                    update_display_for_message_option(current_message_option);
                }
                if (key_pressed == 'B') {
                    current_message_option = (current_message_option + 1) % 2;
                    update_display_for_message_option(current_message_option);
                }
                if (key_pressed == '0') {
                	switch (current_message_option) {
				case 0: enter_message_number(); break;
				case 1: enter_recieved(gsm);
			}
                }
                if (key_pressed == 'D') {
                    enter_main_screen(gsm);
                }
                if (key_pressed == 'C') {
                    pop_state(gsm);
                }
                break;
            case STATE_MESSAGE_NUMBER:
                if (key_pressed == '#') {
                    enter_message_text(gsm);
                }
                if (key_pressed == 'C') {
                    pop_state(gsm);
                }
                if (key_pressed == 'D') {
                	enter_main_screen(gsm);
                }
                break;
            case STATE_MESSAGE_TEXT:
                if (key_pressed == '#') {
                    display_sent_sms(&lcd);
                    HAL_Delay(2000);
                    enter_main_screen(gsm);
                }
                if (key_pressed == 'C') {
                    pop_state(gsm);
                }
                if (key_pressed == 'D') {
                	enter_main_screen(gsm);
                }
                break;
            case STATE_RECIEVED:
            	display_messages_list(&lcd, gsm);
            	if (key_pressed == 'C') {
					pop_state(gsm);
				}
				if (key_pressed == 'D') {
					enter_main_screen(gsm);
				}
            default:
                enter_main_screen(gsm);
        }
    } else {
        switch (current_state) {
            case STATE_CALL:
                if (key_pressed == '*') {
                    gsm.hang_up();
                    enter_main_screen(gsm);
                }
                break;
            case STATE_INCOMING_CALL:
                if (key_pressed == '*') {
                    gsm.receive_call();
                    enter_call();
                }
                if (key_pressed == '#') {
                    gsm.hang_up();
                    enter_main_screen(gsm);
                }
                break;
        }
    }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void){
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  lcd.hw_conf.spi_handle = &hspi2;
  lcd.hw_conf.spi_cs_pin = LCD1_CS_Pin;
  lcd.hw_conf.spi_cs_port = LCD1_CS_GPIO_Port;
  lcd.hw_conf.rst_pin = LCD1_RST_Pin;
  lcd.hw_conf.rst_port = LCD1_RST_GPIO_Port;
  lcd.hw_conf.dc_pin = LCD1_DC_Pin;
  lcd.hw_conf.dc_port = LCD1_DC_GPIO_Port;
  lcd.def_scr = lcd5110_def_scr;

  char key_pressed = 0;

  Parameters parameters;
  load_parameters(parameters, USART_RX_GPIO_Port, USART_RX_Pin, USART_TX_GPIO_Port, USART_TX_Pin, &huart2);
  GSM_Module gsm(parameters);

  LCD5110_init(&lcd.hw_conf, LCD5110_NORMAL_MODE, 0x40, 2, 3);

  enter_main_screen(gsm);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  key_pressed = keypad_scan();
	  if (key_pressed != 0) {
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
		  handle_key_press(key_pressed, gsm);
		  HAL_Delay(2000);
		  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
	  }

	  if (gsm.current_state == gsm.INCOMING_CALL) {
		  enter_incomming_call();
	  } else if (gsm.current_state == gsm.CONVERSATION) {
		  enter_call();
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART1
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief USB Initialization Function
  * @param None
  * @retval None
  */
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
