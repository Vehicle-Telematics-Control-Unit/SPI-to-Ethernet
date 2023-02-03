#include "stm32f1xx_hal.h"
#include "w5500_interface.h"
#include "socket.h"

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/**
 * @brief function to set cs pin low to start transmission
 */
void cs_sel() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //CS LOW
}

/**
 * @brief function to set cs pin high to end transmission
 */
void cs_desel() {
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
 * @brief function that configures the MCU (STM32)
 */
void configureMCU()
{
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
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

/**
 * @brief this function configures the system clock of the Microcontroller (STM32)
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief this function initializes the SPI peripheral of the Microcontroller (STM32) to enable communication through SPI
 */
void MX_SPI1_Init(void)
{
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

}

/**
 * @brief this function initializes the GPIO peripheral of the Microcontroller (STM32). note: we configured portA pin 4 as output which will be used to start and end transmission for SPI
 */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
