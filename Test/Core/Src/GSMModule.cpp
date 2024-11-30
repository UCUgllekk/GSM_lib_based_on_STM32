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
extern "C"{
	#include "usart.h"
	#include "usb.h"
}
#include <cstring>
#include <iostream>
#include <string>

const char* MSG = "AT+CMGS=";
const char* MSG_MODE_1 = "AT+CMGF=1\r\n";
const char* MSG_MODE_0 = "AT+CMGF=0\r\n";

const char* CALL = "ATD+";

const char* AT = "AT\r\n";

static GSM_Module* gsm = nullptr;


GSM_Module::GSM_Module(const Parameters& parameters){
	gsm = this;
	this->parameters = parameters;
	if(!send_at_command(AT)){
		Error_Handler();
	}
	start_receiving();
}

void GSM_Module::make_call(const char* number){
	char command[32];
	snprintf(command, sizeof(command), "%s%s\r\n", CALL, number);
	transmit(command, strlen(command));
}

void GSM_Module::receive_call() {
	transmit("ATA\r\n", 5);
}

void GSM_Module::hang_up(){
	transmit("ATH\r\n", 5);
}

bool GSM_Module::send_at_command(const char* command){
	if (!transmit(command, strlen(command))){
		return false;
	}
	char answer[256];
	bool result = receive(answer, 100);
	if (result != HAL_OK or !strstr(answer, "OK")){
		return false;
	}
	return true;
}

void GSM_Module::handle_interruption(){

	std::string buffer_to_str(reinterpret_cast<char*>(rx_buffer), rx_index);

    if (buffer_to_str.find("RING")) {

        receive_call();

    } else if (buffer_to_str.find("")) {

//    	receive_sms();

    }
    rx_index = 0;
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (gsm && huart == gsm->parameters.uart_handle){

        if (gsm->rx_buffer[gsm->rx_index] == '\n'){
        	gsm->handle_interruption();
        }

        gsm->rx_index = (gsm->rx_index + 1) % 256;

        gsm->start_receiving();
	}
}

void GSM_Module::send_sms(const char* number, const char* message){

	transmit(MSG_MODE_1, strlen(MSG_MODE_1) != HAL_OK);

	HAL_Delay(1000);

	char command[32];
	snprintf(command, sizeof(command), "%s\"%s\"", MSG, number);
	transmit(command, strlen(command));
	HAL_Delay(1000);

	char msg[256];

    snprintf(msg, sizeof(msg), "%s\r\n0x1A", message);
    transmit(msg, strlen(msg));
    HAL_Delay(1000);

    transmit(MSG_MODE_0, strlen(MSG_MODE_0));
}

void c_print(const char* str){
	size_t command_len = strlen(str) + 2;
	char command[command_len];
	snprintf(command, command_len, "%s\r\n", str);
	gsm->transmit(command, strlen(command));
}

Parameters load_parameters(){
	Parameters parameters;
	parameters.uart_handle = &huart2;
	parameters.rx_pin = USART_RX_Pin;
	parameters.rx_port = USART_RX_GPIO_Port;
	parameters.tx_pin = USART_TX_Pin;
	parameters.tx_port = USART_TX_GPIO_Port;
	return parameters;
}

void GSM_Module::start_receiving() {
    HAL_UART_Receive_IT(parameters.uart_handle, &rx_buffer[rx_index], 1);
}

bool GSM_Module::transmit(const char* data, size_t size) {
    return HAL_UART_Transmit(parameters.uart_handle, (uint8_t*)data, size, 100) == HAL_OK;
}

bool GSM_Module::receive(char* buffer, size_t size) {
    return HAL_UART_Receive(parameters.uart_handle, (uint8_t*)buffer, size, 100) == HAL_OK;
}

//	char command[32];
//	int index = 0;
//
//	while (true) {
//		uint8_t received_char;
//
//		HAL_UART_Receive(this->parameters.uart_handle, &received_char, 1, 100);
//
//		if (received_char) {
//			command[index] = received_char;
//
//			index++;
//
//			std::string result = command;
//
//			if (result.find("RING") <= sizeof(command) - 1) {
//				HAL_UART_Transmit(this->parameters.uart_handle, (uint8_t*)"ATA\r\n", 5, 100);
//			}
//		}
//
//		if (index >= sizeof(command) - 1) {
//			memset(command, 0, sizeof(command));
//			index = 0;
//		}


