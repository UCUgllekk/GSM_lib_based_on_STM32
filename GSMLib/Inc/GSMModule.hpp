/*
 * GSM_MODULE.hpp
 *
 *  Created on: Nov 2, 2024
 *      Author: gllekk
 */

#ifndef INC_GSM_MODULE_HPP_
#define INC_GSM_MODULE_HPP_

#include <stm32f303xc.h>
//#include <string>
#include "main.h"

struct Parameters{
	UART_HandleTypeDef *uart_handle;

	uint16_t rx_pin;
	GPIO_TypeDef *rx_port;

	uint16_t td_pin;
	GPIO_TypeDef *td_port;

};

class GSM_Module{
public:
	GSM_Module(const Parameters& parameters);
	int send_sms(const char* message);
	void make_call();
	bool send_AT();
private:
	int send_at_command(const char* command);
	Parameters parameters;
};

Parameters load_parameters();

#endif /* INC_GSM_MODULE_HPP_ */
