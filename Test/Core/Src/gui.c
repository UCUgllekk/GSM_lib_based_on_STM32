#include "gui.h"
#include "stm32f3xx_hal.h"
#include <stdint.h>
#include <stdio.h>

#define ROW_1_Pin GPIO_PIN_0
#define ROW_2_Pin GPIO_PIN_1
#define ROW_3_Pin GPIO_PIN_2
#define ROW_4_Pin GPIO_PIN_3
#define COL_1_Pin GPIO_PIN_4
#define COL_2_Pin GPIO_PIN_5
#define COL_3_Pin GPIO_PIN_6
#define COL_4_Pin GPIO_PIN_7

GPIO_TypeDef* ROW_1_Port = GPIOA;
GPIO_TypeDef* ROW_2_Port = GPIOA;
GPIO_TypeDef* ROW_3_Port = GPIOA;
GPIO_TypeDef* ROW_4_Port = GPIOA;
GPIO_TypeDef* COL_1_Port = GPIOB;
GPIO_TypeDef* COL_2_Port = GPIOB;
GPIO_TypeDef* COL_3_Port = GPIOB;
GPIO_TypeDef* COL_4_Port = GPIOB;

LCD5110_display lcd;

void keypad_init(void)
{
  // Configure GPIO pins for keypad matrix
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = ROW_1_Pin | ROW_2_Pin | ROW_3_Pin | ROW_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(ROW_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_3_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(ROW_4_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = COL_1_Pin | COL_2_Pin | COL_3_Pin | COL_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(COL_1_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_2_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_3_Port, &GPIO_InitStruct);
  HAL_GPIO_Init(COL_4_Port, &GPIO_InitStruct);
}


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
    HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
    break;

    case 1:
    HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
    break;

    case 2:
    HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_SET);
    break;

    case 3:
    HAL_GPIO_WritePin(COL_1_Port, COL_1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_2_Port, COL_2_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_3_Port, COL_3_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(COL_4_Port, COL_4_Pin, GPIO_PIN_RESET);
    break;
  }
  // Read current rows
  if(HAL_GPIO_ReadPin(ROW_1_Port, ROW_1_Pin) == GPIO_PIN_RESET)
    return keys[0][i];
  if(HAL_GPIO_ReadPin(ROW_2_Port, ROW_2_Pin) == GPIO_PIN_RESET)
    return keys[1][i];
  if(HAL_GPIO_ReadPin(ROW_3_Port, ROW_3_Pin) == GPIO_PIN_RESET)
    return keys[2][i];
  if(HAL_GPIO_ReadPin(ROW_4_Port, ROW_4_Pin) == GPIO_PIN_RESET)
    return keys[3][i];
  }
  return 0; // No key pressed
}

void display_main_screen(LCD5110_display* lcd, int menu) {
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

  int y_position = 35; // Y position for the menu button
  int rect_height = 8; // Height of the rectangle
  int rect_width = 8; // Full width of the screen (Nokia 5110)

  if (menu == 1) {
    // Draw a black rectangle
    for (int x = 0; x < rect_width; x++) {
      for (int y = y_position; y < y_position + rect_height; y++) {
        LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
      }
    }



    // Print white text on black rectangle
    LCD5110_set_cursor(2, y_position, lcd); // Adjust X cursor (2) for slight padding
    LCD5110_print("Menu", WHITE, lcd);
  } else {
    // Print text in black (no rectangle)
    LCD5110_set_cursor(2, y_position, lcd);
    LCD5110_print("Menu", BLACK, lcd);
  }

  for (int x = 0; x < 26; x++) {
          LCD5110_putpix(x, 42, BLACK, &lcd->hw_conf);
   }


  LCD5110_set_cursor(53, 13, lcd);
  LCD5110_print("19:45", BLACK, lcd);

  LCD5110_set_cursor(30, 25, lcd);
  LCD5110_print("Thu/09.05", BLACK, lcd);


  LCD5110_refresh(lcd);
}


void display_main_menu(LCD5110_display* lcd, int selected_num) {
    // Clear the screen
    LCD5110_clear_scr(lcd);

    LCD5110_set_cursor(31, 0, lcd);
    LCD5110_print("Menu", BLACK, lcd);

    for (int x = 0; x < 84; x++) {
        LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
    }

    const char* menu_items[] = {"> Call", "> Messages", "> Snake", "> Music"};
    const int y_positions[] = {12, 20, 28, 36};
    const int num_items = 4;

    for (int i = 0; i < num_items; i++) {
        if (selected_num == i + 1) {
            // Draw a filled rectangle to highlight the selected item
            for (int x = 0; x < 84; x++) {
                for (int y = y_positions[i]; y < y_positions[i] + 8; y++) {
                    LCD5110_putpix(x, y, BLACK, &lcd->hw_conf);
                }
            }
            LCD5110_set_cursor(0, y_positions[i], lcd);
            LCD5110_print(menu_items[i], WHITE, lcd);
        } else {
            // Print the item in black color
            LCD5110_set_cursor(0, y_positions[i], lcd);
            LCD5110_print(menu_items[i], BLACK, lcd);
        }
    }
    LCD5110_refresh(lcd);
}


void display_messages_screen(LCD5110_display* lcd, int selected_num) {
    LCD5110_clear_scr(lcd); // CLEAR THE SCREEN

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
            // Print the item in black color
            LCD5110_set_cursor(0, y_positions[i], lcd);
            LCD5110_print(menu_items[i], BLACK, lcd);
        }
    }
    LCD5110_refresh(lcd);
}


void display_call_screen(LCD5110_display* lcd){
  LCD5110_clear_scr(lcd); // CLEAR THE SCREEN

  LCD5110_set_cursor(31, 0, lcd);
  LCD5110_print("Call", BLACK, lcd);

  for (int x = 0; x < 84; x++) {
    LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
  }

  LCD5110_set_cursor(3, 14, lcd);
  LCD5110_print("Enter number:", BLACK, lcd);

  LCD5110_set_cursor(1, 35, lcd);
  LCD5110_print("+380", BLACK, lcd);

  int x_start = 26;
  int x_end = 83;
  int y_start = 33;
  int y_end = 41;

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

}


void display_call_process(LCD5110_display* lcd){
  LCD5110_clear_scr(lcd);
  LCD5110_set_cursor(15, 0, lcd);
  LCD5110_print("Calling...", BLACK, lcd);

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


void display_send_sms(LCD5110_display* lcd){
  LCD5110_clear_scr(lcd);

  LCD5110_set_cursor(21, 0, lcd);
  LCD5110_print("Send SMS", BLACK, lcd);

  for (int x = 0; x < 84; x++) {
    LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
  }

  LCD5110_set_cursor(3, 14, lcd);
  LCD5110_print("Enter number:", BLACK, lcd);

  LCD5110_set_cursor(1, 35, lcd);
  LCD5110_print("+380", BLACK, lcd);

  int x_start = 26;
  int x_end = 83;
  int y_start = 33;
  int y_end = 41;

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

}


void display_send_sms_input(LCD5110_display* lcd){
  LCD5110_clear_scr(lcd);

  LCD5110_set_cursor(17, 0, lcd);
  LCD5110_print("Send SMS", BLACK, lcd);

  for (int x = 0; x < 84; x++) {
    LCD5110_putpix(x, 9, BLACK, &lcd->hw_conf);
  }

  LCD5110_set_cursor(3, 14, lcd);
  LCD5110_print("Enter SMS:", BLACK, lcd);

  int x_start = 0;
  int x_end = 84;
  int y_start = 25;
  int y_end = 41;

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
}


typedef enum {
    STATE_MAIN_SCREEN,
    STATE_MENU,
    STATE_CALL_SCREEN,
    STATE_MESSAGES_SCREEN,
    STATE_SNAKE_SCREEN,
    STATE_MUSIC_SCREEN
} SystemState;

int MAX_OPTIONS = 4;

SystemState current_state = STATE_MAIN_SCREEN;
int current_option = 0;

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
        case 3:
            display_main_menu(&lcd, 4);
            break;
    }
}

void enter_main_screen() {
    display_main_screen(&lcd, 0);
    current_state = STATE_MAIN_SCREEN;
}

void enter_menu() {
    display_main_menu(&lcd, 0);
    update_display_for_option(current_option);
    current_state = STATE_MENU;
}

void enter_call_screen() {
    display_call_screen(&lcd);
    current_state = STATE_CALL_SCREEN;
}

void enter_messages_screen() {
    display_messages_screen(&lcd, 0);
    current_state = STATE_MESSAGES_SCREEN;
}

void enter_snake_screen() {
    current_state = STATE_SNAKE_SCREEN;
}

void enter_music_screen() {
    current_state = STATE_MUSIC_SCREEN;
}

void handle_key_press(char key_pressed) {
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
                    } else if (key_pressed == 'B') {
                        current_option = (current_option + 1) % MAX_OPTIONS;
                        update_display_for_option(current_option);
                    } else if (key_pressed == '0') {
                        switch (current_option) {
                            case 0: enter_call_screen(); break;
                            case 1: enter_messages_screen(); break;
                            case 2: enter_snake_screen(); break;
                            case 3: enter_music_screen(); break;
                        }
                    } else if (key_pressed == 'D') {
                        enter_main_screen();
                    }
                    break;

                case STATE_CALL_SCREEN:
                case STATE_MESSAGES_SCREEN:
                case STATE_SNAKE_SCREEN:
                case STATE_MUSIC_SCREEN:
                    if (key_pressed == 'D') {
                        enter_menu();
                    }
                    break;

                default:
                    break;
            }
        }

// Main function
int main(void) {
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_SPI1_Init();
    MX_USB_PCD_Init();
    MX_SPI2_Init();

    lcd.hw_conf.spi_handle = &hspi2;
    lcd.hw_conf.spi_cs_pin = LCD1_CS_Pin;
    lcd.hw_conf.spi_cs_port = LCD1_CS_GPIO_Port;
    lcd.hw_conf.rst_pin = LCD1_RST_Pin;
    lcd.hw_conf.rst_port = LCD1_RST_GPIO_Port;
    lcd.hw_conf.dc_pin = LCD1_DC_Pin;
    lcd.hw_conf.dc_port = LCD1_DC_GPIO_Port;
    lcd.def_scr = lcd5110_def_scr;

    LCD5110_init(&lcd.hw_conf, LCD5110_NORMAL_MODE, 0x40, 2, 3);

    keypad_init();

    enter_main_screen();

    char key_pressed = 0;

    while (1) {
        key_pressed = keypad_scan();
        if (key_pressed != 0) {
          HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_SET);
            handle_key_press(key_pressed);
            HAL_Delay(2000);
          HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, GPIO_PIN_RESET);
        }
    }

    return 0;
}



/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
//    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
//
//    /** Initializes the RCC Oscillators according to the specified parameters
//    * in the RCC_OscInitTypeDef structure.
//    */
//    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
//    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
//    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
//    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
//    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
//        Error_Handler();
//    }
//
//    /** Initializes the CPU, AHB and APB buses clocks
//    */
//    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
//    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
//        Error_Handler();
//    }
//    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_I2C1;
//    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
//    PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
//    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
//        Error_Handler();
//    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
    // Print debug info if needed
}
#endif /* USE_FULL_ASSERT */
