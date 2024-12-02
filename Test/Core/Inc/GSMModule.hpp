/*
 * GSM_MODULE.hpp
 *
 *  Created on: Nov 2, 2024
 *      Author: gllekk
 */

#ifndef INC_GSM_MODULE_HPP_
#define INC_GSM_MODULE_HPP_

extern "C"{
	#include "main.h"
}

#include <string>


struct Parameters{
	UART_HandleTypeDef *uart_handle;

	uint16_t rx_pin;
	GPIO_TypeDef *rx_port;

	uint16_t tx_pin;
	GPIO_TypeDef *tx_port;
};

class GSM_Module{

enum State{
	IDLE,
	CALLING,
	RECEIVE_CALL,
	RINGING,
	HANG_UP,
	SEND_SMS,
	RECEIVE_SMS,
	UNKNOWN
};
public:
	GSM_Module(const Parameters& parameters);

	void send_sms(const char* number, const char* message);
	void make_call(const char* number);
	void hang_up();

	void receive_call();
	void receive_sms();

	bool transmit(const char* data, size_t size);
	bool receive(char* buffer, size_t size);
	void start_receiving();

	State prev_state = IDLE;
	State current_state = IDLE;

private:
	uint16_t rx_index = 0;
	uint8_t rx_buffer[256];
	Parameters parameters;

	bool send_at_command(const char* command);
	void read_sms(int index);

	void handle_interruption();
    friend void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

};

void c_print(const char* str);

Parameters load_parameters();

#endif /* INC_GSM_MODULE_HPP_ */
