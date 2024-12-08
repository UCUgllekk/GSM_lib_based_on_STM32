/*
 * main.cpp
 *
 *  Created on: Nov 2, 2024
 *      Author: gllekk
 */


#include "GSMModule.hpp"
#include <string>
#include <iostream>

int main(){
	Parameters param = load_parameters();
	GSM_Module gsm = GSM_Module(param);
	std::cout << gsm.send_AT();
}


