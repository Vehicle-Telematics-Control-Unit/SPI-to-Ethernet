/*
 * w5500_interface.h
 *
 *  Created on: Jan 26, 2023
 *      Author: BASILYJOK3R
 */

#ifndef INC_W5500_INTERFACE_H_
#define INC_W5500_INTERFACE_H_

#include <stdint.h>

void configureMCU();
void initializeW5500(uint8_t mac[], uint8_t ip[], uint8_t sn[], uint8_t gw[]);
void Socket_sendUDP(uint8_t socNum,uint8_t message[], uint8_t dstip[], uint16_t portNum);

#endif /* INC_W5500_INTERFACE_H_ */
