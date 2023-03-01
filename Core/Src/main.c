#include "main.h"

/**
 * @brief this is a demo or a sample that illustrates how to configure a node and send a greeting message through UDP. note: there are only 8 socket registers in w5500 please refer to datasheet for more information.
 */
int main(void) {
	configureMCU();//MCU configuration

	uint8_t socNum=0;

	uint8_t mac[]={ 0x00, 0x08, 0xdc, 0xab, 0xcd, 0xef };// Mac address
	uint8_t ip[]={ 192, 168, 1, 15 };// IP address
	uint8_t sn[]={ 255, 255, 255, 0 };// Subnet mask
	uint8_t gw[]={ 192, 168, 1, 1 };// Gateway address

	initializeW5500(mac, ip, sn, gw);//configures the MAC address, IP address, subnet mask and gateway of the device or node.

	uint8_t dstip[4] = { 192, 168, 1, 4 };//destination ip address
	char greetingMSG[] = "Hello User\0";

	/*in this section we will keep sending a greeting message*/

	/*initializes socket with a port number and mode. for instance, a port number of 5000 and udp is used in this sample*/
	if ((socket(socNum, Sn_MR_UDP, 5000, 0)) == socNum) {
		while (1) {
			Socket_sendUDP(socNum, greetingMSG, dstip, 5001);//sends message to a destination ip address and port
			//HAL_Delay(500);
		}
	close(socNum);//closes socket
	}
}
