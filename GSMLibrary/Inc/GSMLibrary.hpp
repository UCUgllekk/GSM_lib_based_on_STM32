/*
 * GSMLibrary.hpp
 *
 *  Created on: Dec 11, 2024
 *      Author: gllekk
 */

#ifndef INC_GSMLIBRARY_HPP_
#define INC_GSMLIBRARY_HPP_
/* Includes ------------------------------------------------------------------*/
#ifdef __MAIN_H
extern "C"{
	#include "main.h"
}
#endif
#include <string>
/** Functions ----------------------------------------------------------------*/
struct Parameters{
	UART_HandleTypeDef *uart_handle;

	uint16_t rx_pin;
	GPIO_TypeDef *rx_port;

	uint16_t tx_pin;
	GPIO_TypeDef *tx_port;
};

class GSM_Module{

public:

	const char END_OF_MSG = 0x1A;

	enum State{
			IDLE,
			CALLING_PROCESS,
			CONVERSATION,
			INCOMING_CALL
		};

	State prev_state = IDLE;
	State current_state = IDLE;

	GSM_Module(const Parameters& parameters);

	void send_sms(const char* number, const char* message);
	std::pair<std::string, std::string> read_sms(int index);

	void make_call(const char* number);
	void hang_up();

	void receive_call();

	bool transmit(const char* data, size_t size);
	bool receive(char* buffer, size_t size, size_t timeout = 100);


	int get_signal_strength();
	std::pair<std::string, std::string> get_date_and_time();

private:

	static constexpr const char* MSG = "AT+CMGS=";
	static constexpr const char* MSG_TEXT_MODE = "AT+CMGF=1\r\n";

	static constexpr const char* CALL = "ATD+";

	static constexpr const char* AT = "AT\r\n";

	static constexpr const char* GPS_ON = "AT+GPS=1\r\n";
	static constexpr const char* GPS_OFF = "AT+GPS=0\r\n";
	static constexpr const char* GET_TIME = "AT+CCLK?\r\n";
	static constexpr const char* GET_SIGNAL = "AT+CSQ\r\n";

	Parameters parameters;
	uint16_t rx_index = 0;
	uint8_t rx_buffer[256];

	int signal = 0;
	std::string time;
	std::string date;
	std::string sms_sender;
	std::string sms_content;
	std::string gps_data;

	bool send_at_command(const char* command);
	void handle_interruption();
	void start_receiving();
	void receive_signal(std::string buffer);
	void receive_date_and_time(std::string buffer);
	void receive_sms(std::string buffer);
    friend void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart);

};

[[maybe_unused]] static GSM_Module* gsm = nullptr;

void load_parameters(Parameters& parameters, GPIO_TypeDef* rx_port, uint16_t rx_pin,
                     GPIO_TypeDef* tx_port, uint16_t tx_pin, UART_HandleTypeDef* uart_handle);
#endif /* INC_GSMLIBRARY_HPP_ */
