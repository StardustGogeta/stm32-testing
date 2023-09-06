/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    ux_device_cdc_ecm.c
  * @author  MCD Application Team
  * @brief   USBX Device applicative file
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
#include "ux_device_cdc_ecm.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "app_usbx_device.h"
//#include "usart.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define APP_RX_DATA_SIZE                          (2048 * 4)
#define APP_TX_DATA_SIZE                          (2048 * 4)

/* Rx/TX flag */
#define RX_NEW_RECEIVED_DATA                      0x01
#define TX_NEW_TRANSMITTED_DATA                   0x02

/* Data length for vcp */
#define VCP_WORDLENGTH8                           8
#define VCP_WORDLENGTH9                           9

/* the minimum baudrate */
#define MIN_BAUDRATE                              9600
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* Data received over uart are stored in this buffer */
static uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/* Data to send over USB CDC are stored in this buffer   */
static uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];

/* Increment this pointer or roll it back to
start address when data are received over USART */
static uint32_t UserTxBufPtrIn;

/* Increment this pointer or roll it back to
start address when data are sent over USB */
static uint32_t UserTxBufPtrOut;

/* uart3 handler */
//extern UART_HandleTypeDef huart1;
extern TX_EVENT_FLAGS_GROUP EventFlag;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern void Error_Handler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
  * @brief  USBD_CDC_ECM_Activate
  *         This function is called when insertion of a CDC ECM device.
  * @param  cdc_ecm_instance: Pointer to the cdc ecm class instance.
  * @retval none
  */
VOID USBD_CDC_ECM_Activate(VOID *cdc_ecm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ECM_Activate */
  UX_PARAMETER_NOT_USED(cdc_ecm_instance);
  /* USER CODE END USBD_CDC_ECM_Activate */

  return;
}

/**
  * @brief  USBD_CDC_ECM_Deactivate
  *         This function is called when extraction of a CDC ECM device.
  * @param  cdc_ecm_instance: Pointer to the cdc ecm class instance.
  * @retval none
  */
VOID USBD_CDC_ECM_Deactivate(VOID *cdc_ecm_instance)
{
  /* USER CODE BEGIN USBD_CDC_ECM_Deactivate */
  UX_PARAMETER_NOT_USED(cdc_ecm_instance);
  /* USER CODE END USBD_CDC_ECM_Deactivate */

  return;
}

/**
  * @brief  USBD_CDC_ECM_GetMacAdd
  *         This function is called to get cdc ecm node id from MAC Address.
  * @param  mac_add: Pointer string to MAC address.
  * @param  mac_node: Pointer to mac node buffer.
  * @retval none
  */
VOID USBD_CDC_ECM_GetMacAdd(UCHAR *mac_add, UCHAR *mac_node)
{
  /* USER CODE BEGIN USBD_CDC_ECM_GetMacAdd */
  CHAR dummy[2] = {0};
  UCHAR i = 0, j = 0, k = 0;
  UCHAR num = 0;

  for(k = 0; k < 6; k++)
  {
    for(i = 0; i < 2; i++)
    {
      dummy[i] = mac_add[i+j];
    }

    num = strtol(dummy, NULL, 16);

    j=j+2;

    mac_node[k] = num;
  }
  /* USER CODE END USBD_CDC_ECM_GetMacAdd */
}


/**
  * @brief  Function implementing usbx_cdc_acm_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_cdc_ecm_read_thread_entry(ULONG arg)
{
  UX_SLAVE_DEVICE *device;
  UX_SLAVE_INTERFACE *data_interface;
  UX_SLAVE_CLASS_CDC_ECM *cdc_ecm;
  ULONG actual_length;
  ULONG ux_status = UX_SUCCESS;
  ULONG senddataflag = 0;

  /* Get device */
  device = &_ux_system_slave->ux_system_slave_device;

  while (1)
  {
    /* Check if device is configured */
    if (device->ux_slave_device_state == UX_DEVICE_CONFIGURED)
    {
      /* Get Data interface */
      tx_thread_sleep(5);
      data_interface = device->ux_slave_device_first_interface->ux_slave_interface_next_interface;

      /* Compares two memory blocks ux_slave_class_name and _ux_system_slave_class_cdc_acm_name */
      ux_status = ux_utility_memory_compare(data_interface->ux_slave_interface_class->ux_slave_class_name,
                                            _ux_system_slave_class_cdc_acm_name,
                                            ux_utility_string_length_get(_ux_system_slave_class_cdc_acm_name));

      /* Check Compares success */
      if (ux_status == UX_SUCCESS)
      {
        cdc_ecm =  data_interface->ux_slave_interface_class_instance;

        /* Read the received data in blocking mode */
        //ux_device_class_cdc_ecm_read(cdc_ecm, (UCHAR *)UserRxBufferFS, 64, &actual_length);
        if (actual_length != 0)
        {
        	//ux_status = ux_device_class_cdc_ecm_write(cdc_ecm, (UCHAR *)UserRxBufferFS, actual_length, NULL);
        	continue;
        }
      }
      tx_thread_sleep(10);
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
    else
    {
      tx_thread_sleep(1);
    }
  }
}

/**
  * @brief  Function implementing usbx_cdc_acm_write_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_cdc_ecm_write_thread_entry(ULONG arg)
{
  return;
}

/* USER CODE END 0 */

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
