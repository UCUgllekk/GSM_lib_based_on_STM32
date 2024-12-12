/**
  ******************************************************************************
  * @file     lib.c
  * @author   Auto-generated by STM32CubeIDE
  * @version  V1.0
  * @date     11/12/2024 03:35:27
  * @brief    Default under dev library file.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include "stm32f3xx_hal.h"
#include "GSMLibrary.hpp"
extern "C"{
#ifdef __USART_H__
	#include "usart.h"
#endif
}
#include <cstring>
#include <iostream>
#include <string>

/** Functions ----------------------------------------------------------------*/

GSM_Module::GSM_Module(const Parameters& parameters){
	gsm = this;
	this->parameters = parameters;
	if(!send_at_command(AT) || !send_at_command(MSG_TEXT_MODE)){
		return;
	}
	start_receiving();
}


void GSM_Module::make_call(const char* number){
	if (this->current_state != IDLE) {
	    return;
	}

	char command[32];
	snprintf(command, sizeof(command), "%s%s;\r\n", CALL, number);
	transmit(command, strlen(command));

	this->prev_state = this->current_state;
	this->current_state = this->CALLING_PROCESS;
}


void GSM_Module::receive_call() {
	transmit("ATA\r\n", 5);
}


void GSM_Module::hang_up(){
	if (this->current_state != CALLING_PROCESS && this->current_state != INCOMING_CALL && this->current_state != CONVERSATION) {
	    return;
	}

	transmit("ATH\r\n", 5);
	this->prev_state = this->current_state;
	this->current_state = IDLE;
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


std::pair<std::string, std::string> GSM_Module::read_sms(int index) {
    char command[32];
    snprintf(command, sizeof(command), "AT+CMGR=%d\r\n", index);

    if (!transmit(command, strlen(command))) {
        return {"", ""};
    }

    HAL_Delay(1000);
    return {this->sms_sender, this->sms_content};
}


void GSM_Module::receive_sms(std::string buffer){
    size_t sender_start = buffer.find(",\"") + 2;
    size_t sender_end = buffer.find(",", sender_start);
    size_t message_start = buffer.find("\r\n", sender_end) + 2;
    size_t message_end = buffer.find("\r\n", message_start);
    if (sender_start != std::string::npos && sender_end != std::string::npos &&
        message_start != std::string::npos && message_end != std::string::npos) {
        this->sms_sender = buffer.substr(sender_start, sender_end - sender_start);
        this->sms_content = buffer.substr(message_start, message_end - message_start);
    }
}

std::pair<std::string, std::string> GSM_Module::get_date_and_time() {
    if (!transmit(GET_TIME, strlen(GET_TIME))) {
        return {"", ""};
    }
    HAL_Delay(250);
    return {this->date, this->time};
}

void GSM_Module::receive_date_and_time(std::string buffer) {
    size_t cclk_start = buffer.find("+CCLK: ");

    if (cclk_start == std::string::npos) {
        return;
    }

    std::string time_string = buffer.substr(cclk_start + 7);

    if (time_string[0] == '"') {
        time_string = time_string.substr(1, time_string.size() - 2);
    }

    int year, month, day, hh, mm, ss, tz;

    if (sscanf(time_string.c_str(), "%2d/%2d/%2d,%2d:%2d:%2d+%2d", &year, &month, &day, &hh, &mm, &ss, &tz) != 7) {
        return;
    }

    hh += tz;

    if (hh >= 24) {
        day += 1;
        hh = hh % 24;
    }

    char date_buffer[9];
    snprintf(date_buffer, sizeof(date_buffer), "%02d/%02d/%02d", day, month, year);
    date = std::string(date_buffer);

    char time_buffer[9];
    snprintf(time_buffer, sizeof(time_buffer), "%02d:%02d:%02d", hh, mm, ss);
    time = std::string(time_buffer);
}

int GSM_Module::get_signal_strength() {
    if (!transmit(GET_SIGNAL, strlen(GET_SIGNAL))) {
        return -1;
    }
    HAL_Delay(250);
    return signal;
}

void GSM_Module::receive_signal(std::string buffer) {
    size_t start = buffer.find("+CSQ: ") + 6;
    size_t end = buffer.find(",", start);
    std::string signal_str = buffer.substr(start, end - start);
    bool isValid = true;
    for (char c : signal_str) {
        if (!isdigit(c)) {
            isValid = false;
            break;
        }
    }
    if (isValid) {
        int rssi = 0;
        for (char c : signal_str) {
            rssi = rssi * 10 + (c - '0');
        }

        if (rssi >= 0 && rssi <= 31) {
            this->signal = rssi;
        } else {
            this->signal = -1;
        }
    } else {
        this->signal = -2;
    }
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


void GSM_Module::handle_interruption() {

	std::string buffer_to_str(reinterpret_cast<const char*>(rx_buffer), rx_index);

    if (buffer_to_str.find("RING") != std::string::npos) {

        this->prev_state = this->current_state;
        this->current_state = INCOMING_CALL;

    } else if (buffer_to_str.find("+CMGR:") != std::string::npos) {

    	receive_sms(buffer_to_str);

    	this->prev_state = this->current_state;
        this->current_state = IDLE;

    } else if (buffer_to_str.find("+CSQ:") != std::string::npos) {

    	receive_signal(buffer_to_str);

    	this->prev_state = this->current_state;
        this->current_state = IDLE;


    } else if (buffer_to_str.find("+CCLK:") != std::string::npos){

    	receive_date_and_time(buffer_to_str);

    	this->prev_state = this->current_state;
    	this->current_state = IDLE;

	} else if (buffer_to_str.find("+CIEV: \"SOUNDER\",1") != std::string::npos){

		this->current_state = CONVERSATION;

	} else if (buffer_to_str.find("+CIEV: \"SOUNDER\",0") != std::string::npos || buffer_to_str.find("BUSY") != std::string::npos) {

		this->current_state = this->prev_state;

	}

    std::fill(std::begin(rx_buffer), std::end(rx_buffer), 0);
    rx_index = 0;
}


extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if (gsm && huart == gsm->parameters.uart_handle){

        if (gsm->rx_buffer[gsm->rx_index] == '\n'){

        	gsm->handle_interruption();

        }
        else{
            gsm->rx_index = (gsm->rx_index + 1) % 256;
        }

        gsm->start_receiving();
	}
}


void GSM_Module::start_receiving() {
    HAL_UART_Receive_IT(parameters.uart_handle, &rx_buffer[rx_index], 1);
}

bool GSM_Module::transmit(const char* data, size_t size) {
    return HAL_UART_Transmit(parameters.uart_handle, (uint8_t*)data, size, 100) == HAL_OK;
}

bool GSM_Module::receive(char* buffer, size_t size, size_t timeout) {
    return HAL_UART_Receive(parameters.uart_handle, (uint8_t*)buffer, size, timeout) == HAL_OK;
}

void load_parameters(Parameters& parameters, GPIO_TypeDef* rx_port, uint16_t rx_pin,
                     GPIO_TypeDef* tx_port, uint16_t tx_pin, UART_HandleTypeDef* uart_handle) {
    parameters.rx_port = rx_port;
    parameters.rx_pin = rx_pin;
    parameters.tx_port = tx_port;
    parameters.tx_pin = tx_pin;
    parameters.uart_handle = uart_handle;
}

