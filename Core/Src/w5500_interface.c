#include "stm32f1xx_hal.h"
#include "w5500_interface.h"
#include "socket.h"

/* Private variables ---------------------------------------------------------*/
extern SPI_HandleTypeDef hspi1;

/**
 * @brief function to set cs pin low to start transmission
 */
static void cs_sel(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //CS LOW
}

/**
 * @brief function to set cs pin high to end transmission
 */
static void cs_desel(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //CS HIGH
}

/**
 * @brief function to read byte through spi
 */
uint8_t spi_rb(void) {
	uint8_t rbuf;
	HAL_SPI_Receive(&hspi1, &rbuf, 1, 0xFFFFFFFF);
	return rbuf;
}

/**
 * @brief function to write byte through spi
 */
void spi_wb(uint8_t b) {
	HAL_SPI_Transmit(&hspi1, &b, 1, 0xFFFFFFFF);
}

/**
 * @brief this function initializes the w5500
 *
 * @param mac is the node's MAC address
 * @param ip is the node's IP address
 * @param sn is the node's subnet mask
 * @param gw is the node's gateway address
 */
void initializeW5500(uint8_t mac[], uint8_t ip[], uint8_t sn[], uint8_t gw[])
{
	uint8_t bufSize[] = { 2, 2, 2, 2, 2, 2, 2, 2};//size of each socket number in kilobytes

	reg_wizchip_cs_cbfunc(cs_sel, cs_desel);//passes call back function for pin high and low which needs to be called during the SPI transfer
	reg_wizchip_spi_cbfunc(spi_rb, spi_wb);//passes the call back function for read byte and write byte which needs to be called during the SPI transfer

	wizchip_init(bufSize, bufSize);//initializes the transmit and receive buffer sizes for each socket
	wiz_NetInfo netInfo = {};

	memcpy(&netInfo.mac,mac,6);// Mac address
	memcpy(&netInfo.ip,ip,4);// IP address
	memcpy(&netInfo.sn,sn,4);// Subnet mask
	memcpy(&netInfo.gw,gw,4);// Gateway address

	wizchip_setnetinfo(&netInfo);//sets the configuration of the MAC address, IP address, subnet mask and gateway
	//wizchip_getnetinfo(&netInfo);
}

/**
 * @brief this function is used to send a message through UDP
 *
 * @param socNum is the socket number
 * @param message is the message that needs to be sent
 * @param dstip is the destination IP address
 * @param portNum is the destination port number
 */
void Socket_sendUDP(uint8_t socNum,uint8_t message[],uint8_t dstip[], uint16_t portNum)
{
	while(sendto(socNum, message, strlen(message), dstip, portNum)!=strlen(message));//sends message to a destination ip address and port
}

