/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "app_usbx_device.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TX_APP_STACK_SIZE 512
#define TX_APP_THREAD_PRIO 30
#define TX_APP_SECONDARY_THREAD_PRIO 40
#define TX_APP_THREAD_PREEMPTION_THRESHOLD TX_APP_THREAD_PRIO
#define TX_APP_SECONDARY_THREAD_PREEMPTION_THRESHOLD TX_APP_SECONDARY_THREAD_PRIO
#define TX_APP_THREAD_TIME_SLICE TX_NO_TIME_SLICE
#define TX_APP_THREAD_AUTO_START TX_AUTO_START
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
	TX_THREAD tx_app_thread, tx_app2_thread;
	TX_SEMAPHORE tx_app_semaphore;
	extern TX_QUEUE     ux_app_MsgQueue;
	#if defined ( __ICCARM__ ) /* IAR Compiler */
	  #pragma data_alignment=4
	#endif /* defined ( __ICCARM__ ) */
	__ALIGN_BEGIN static USB_MODE_STATE USB_Device_State_Msg;
	static int txSent = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
	void App_Delay(unsigned long ticks);
	void MainThread_Entry(unsigned long thread_input);
	void SecondaryThread_Entry(unsigned long thread_input);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  char* pointer;
  if (tx_byte_allocate(byte_pool, (void**)&pointer, TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
	  return TX_POOL_ERROR;
  }
  if (tx_thread_create(&tx_app_thread, "Main Thread", MainThread_Entry, 0, pointer, TX_APP_STACK_SIZE,
		  TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD, TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
	  return TX_THREAD_ERROR;
  }
  /*if (tx_byte_allocate(byte_pool, (void**)&pointer, TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
	  return TX_POOL_ERROR;
  }
  if (tx_thread_create(&tx_app2_thread, "Secondary Thread", SecondaryThread_Entry, 0, pointer, TX_APP_STACK_SIZE,
		  TX_APP_SECONDARY_THREAD_PRIO, TX_APP_SECONDARY_THREAD_PREEMPTION_THRESHOLD, TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
	  return TX_THREAD_ERROR;
  }*/
  if (tx_semaphore_create(&tx_app_semaphore, "Semaphore", 0) != TX_SUCCESS)
  {
	  return TX_SEMAPHORE_ERROR;
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

void App_Delay(unsigned long ticks)
{
	tx_thread_sleep(ticks);
}

void MainThread_Entry(unsigned long thread_input)
{
	while (1)
	{
		//char buf[100] = {};
		//sprintf(buf, "Button: %d\n\r", HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13));
		//HAL_UART_Transmit(&huart2, (unsigned char*)buf, sizeof(buf), 1000);
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
		//if (1)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

			USB_Device_State_Msg = START_USB_DEVICE;

			/* Send message to start device */
			if (!txSent)
			{
				if (tx_queue_send(&ux_app_MsgQueue, &USB_Device_State_Msg, TX_WAIT_FOREVER) != TX_SUCCESS)
				{
					Error_Handler();
				}
				txSent = 1;
			}
			//if ()
		}
		App_Delay(20);
	}
}
/*
void SecondaryThread_Entry(unsigned long thread_input)
{
	unsigned char buf[128] = {};
	while (1)
	{
		HAL_UART_Receive_IT(&huart1, buf, 1);
		HAL_UART_Transmit_IT(&huart1, buf, 1);
	}
}
*/
/* USER CODE END 1 */
