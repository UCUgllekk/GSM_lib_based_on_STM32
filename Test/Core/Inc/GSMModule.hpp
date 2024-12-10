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

	const char END_OF_MSG = 0x1A;

	GSM_Module(const Parameters& parameters);

	void send_sms(const char* number, const char* message);
	void make_call(const char* number);
	void hang_up();

	void receive_call();
	void receive_sms();

	bool transmit(const char* data, size_t size);
	bool receive(char* buffer, size_t size);
	void start_receiving();

	int get_signal_strength();
	std::pair<std::string, std::string> GSM_Module::get_date();
//	char* receive_gps_data();

	State prev_state = IDLE;
	State current_state = IDLE;

	void receive_signal_strength(std::string buffer);
	void receive_date_and_time(std::string buffer);

private:

	static constexpr const char* MSG = "AT+CMGS=";
	static constexpr const char* MSG_TEXT_MODE = "AT+CMGF=1\r\n";

	static constexpr const char* CALL = "ATD+";

	static constexpr const char* AT = "AT\r\n";

	static constexpr const char* GPS_ON = "AT+GPS=1\r\n";
	static constexpr const char* GPS_OFF = "AT+GPS=0\r\n";
	static constexpr const char* GET_TIME = "AT+CCLK?\r\n";
	static constexpr const char* GET_SIGNAL = "AT+CSQ\r\n";

	uint16_t rx_index = 0;
	uint8_t rx_buffer[256];
	Parameters parameters;

	bool send_at_command(const char* command);
	void read_sms(int index);

	void handle_interruption();
    friend void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

    int signal = 0;
    std::string time;
    std::string date;

};

[[maybe_unused]] static GSM_Module* gsm = nullptr;

Parameters load_parameters();

#endif /* INC_GSM_MODULE_HPP_ */
