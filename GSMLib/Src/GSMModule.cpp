/**
  ******************************************************************************
  * @file     lib.c
  * @author   Auto-generated by STM32CubeIDE
  * @version  V1.0
  * @date     02/11/2024 18:42:21
  * @brief    Default under dev library file.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "GSMModule.hpp"
#include "stm32f3xx_hal.h"
#include "usart.h"
#include <cstring>

GSM_Module::GSM_Module(const Parameters& parameters){
	this->parameters = parameters;
}

int GSM_Module::send_sms(const char* message){
	return 0;
}

void GSM_Module::make_call(){
	// make call
}

bool GSM_Module::send_AT(){
	// private send "AT" command, return true if accepted good, return false if otherwise
	const char* check = "AT\r\n";
	HAL_UART_Transmit(this->parameters.uart_handle, (uint8_t*)check, strlen(check), HAL_MAX_DELAY);

	char answer[16];
	HAL_StatusTypeDef result = HAL_UART_Receive(this->parameters.uart_handle, (uint8_t*)answer, sizeof(answer), HAL_MAX_DELAY);
	if (result == HAL_OK){
		return true;
	}
	return false;
}

int GSM_Module::send_at_command(const char* command){
	return 0;
}

Parameters load_parameters(){
	Parameters parameters;
	parameters.uart_handle = &huart1;
	parameters.rx_pin = RX_FROM_GSM_Pin;
	parameters.rx_port = RX_FROM_GSM_GPIO_Port;
	parameters.td_pin = TX_TO_GSM_Pin;
	parameters.td_port = TX_TO_GSM_GPIO_Port;
	return parameters;
}