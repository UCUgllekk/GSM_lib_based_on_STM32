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


GSM_Module::GSM_Module(const Parameters& parameters){
	gsm = this;
	this->parameters = parameters;
	if(!send_at_command(AT) && !send_at_command(MSG_TEXT_MODE) && !send_at_command(GPS_ON)){
		Error_Handler();
	}
	start_receiving();
}

void GSM_Module::make_call(const char* number){
	if (this->current_state != IDLE) {
	    return;
	}

	char command[32];
	snprintf(command, sizeof(command), "%s%s\r\n", CALL, number);
	transmit(command, strlen(command));

	this->prev_state = this->current_state;
	this->current_state = this->CALLING;
}

void GSM_Module::receive_call() {
	transmit("ATA\r\n", 5);
}

void GSM_Module::hang_up(){
	if (this->current_state != CALLING) {
	    return;
	}

	transmit("ATH\r\n", 5);
	this->current_state = this->prev_state;
	this->prev_state = IDLE;
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

    if (buffer_to_str.find("RING") != std::string::npos) {

    	this->prev_state = this->current_state;
    	this->current_state = RINGING;

    } else if (buffer_to_str.find("+CTM:") != std::string::npos) {

//    	receive_sms();

    } else {

    	this->prev_state = this->current_state;
    	this->current_state = this->UNKNOWN;

    }
    std::fill(std::begin(rx_buffer), std::end(rx_buffer), 0);
    rx_index = 0;
}

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (gsm && huart == gsm->parameters.uart_handle){

        if (gsm->rx_buffer[gsm->rx_index] == '\n'){
            gsm->rx_buffer[gsm->rx_index] = '\0';

        	gsm->handle_interruption();
        }
        else{
            gsm->rx_index = (gsm->rx_index + 1) % 256;
        }

        gsm->start_receiving();
	}
}

void GSM_Module::send_sms(const char* number, const char* message){
	char command[32];
	snprintf(command, sizeof(command), "%s\"%s\"\r", MSG, number);
	if(!transmit(command, strlen(command))){
		make_call("");
	}
	HAL_Delay(1000);
    transmit(message, strlen(message));
    transmit(&END_OF_MSG, 1);
    HAL_Delay(1000);
}

char* GSM_Module::receive_gps_data() {
  char answer[256];
  transmit(GET_TIME, strlen(GET_TIME));
  receive(answer, 256);

  return answer;

}

char* GSM_Module::get_date() {

  char* buffer = receive_gps_data();

//    const char* cclk_start = strstr(buffer, "+CCLK: ");
//    if (!cclk_start) {
//        return NULL;
//    }
//
//    char date_string[30];
//    if (sscanf(cclk_start, "+CCLK: \"%[^\"]\"", date_string) != 1) {
//        return NULL;
//    }

    int year, month, day;
    if (sscanf(buffer, "%2d/%2d/%2d,", &year, &month, &day) != 3) {
        return NULL;
    }

    make_call("380986629200");

    year += 2000;

    char* result = (char*)malloc(13 * sizeof(char));
    if (!result) {
        return NULL;
    }

    snprintf(result, 13, "%04d-%02d-%02d\r\n", year, month, day);

    return result;
}

//char* GSM_Module::get_time() {
//
//  char buffer[256] = {0};
//
//  receive_gps_data(buffer);
//
//    const char* cclk_start = strstr(buffer, "+CCLK: ");
//    if (!cclk_start) {
//        return NULL;
//    }
//
//    char time_string[30] = {0};
//    if (sscanf(cclk_start, "+CCLK: \"%[^\"]\"", time_string) != 1) {
//        return NULL;
//    }
//
//    int hh, mm, ss, tz;
//    if (sscanf(time_string, ",%2d:%2d:%2d+%2d", &hh, &mm, &ss, &tz) != 4) {
//        return NULL;
//    }
//
//    char* result = (char*)malloc(13 * sizeof(char));
//    if (!result) {
//        return NULL;
//    }
//
//    snprintf(result, 13, "%2d:%2d:%2d+%2d", hh, mm, ss, tz);
//
//    return result;
//}
//

int GSM_Module::get_signal_strength() {
  if(!transmit(GET_SIGNAL, strlen(GET_SIGNAL))){
    make_call("380986629200");
  }
  HAL_Delay(500);
    char response[256];
//    make_call("380963809782");
    if (!receive(response, sizeof(response))) {
        make_call("380963809782");
        return -1;
    }

    char* pos = strstr(response, "+CSQ:");
    make_call("380986629200");
    if (pos != nullptr) {
        int rssi, ber;

        if (sscanf(pos + 6, "%d,%d", &rssi, &ber) == 2) {
            make_call("380986629200");

            return rssi;
        }
        make_call("380683809782");
    }

    return -1;
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

Parameters load_parameters(){
	Parameters parameters;
	parameters.uart_handle = &huart2;
	parameters.rx_pin = USART_RX_Pin;
	parameters.rx_port = USART_RX_GPIO_Port;
	parameters.tx_pin = USART_TX_Pin;
	parameters.tx_port = USART_TX_GPIO_Port;
	return parameters;
}
