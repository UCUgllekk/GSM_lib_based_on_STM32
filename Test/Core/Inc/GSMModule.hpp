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
public:
	GSM_Module(const Parameters& parameters);

	void send_sms(const char* number, const char* message);
	void make_call(const char* number);
	void hang_up();

	void receive_call();
	void receive_sms();

	bool transmit(const uint8_t* data, size_t size);
	bool receive(uint8_t* buffer, size_t size, uint32_t timeout);
	void start_receiving();

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
