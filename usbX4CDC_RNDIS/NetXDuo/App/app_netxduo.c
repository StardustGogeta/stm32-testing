/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
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
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RNDIS_IP_ADDRESS        IP_ADDRESS(192,168,0,10)
#define RNDIS_NETMASK_ADDR      IP_ADDRESS(255,255,255,0)
#define RNDIS_GATEWAY_ADDR      IP_ADDRESS(192,168,0,10)

/* IP address range assigned by the DHCP server */
#define START_IP_ADDRESS_LIST  IP_ADDRESS(192, 168, 0, 10)
#define END_IP_ADDRESS_LIST    IP_ADDRESS(192, 168, 0, 20)

/* Network Configuration */
#define NX_DHCP_SERVER_IP_ADDRESS  IP_ADDRESS(192,168,0,2)
#define NX_DHCP_ROUTER_IP_ADDRESS  IP_ADDRESS(192,168,0,2)
#define NX_DHCP_DNS_IP_ADDRESS     IP_ADDRESS(192,168,0,2)

#define LED_ON     "led=LED_ON"
#define LED_OFF    "led=LED_OFF"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
NX_IP              rndis_ip;
NX_PACKET_POOL     net_packet_pool;
NX_DHCP_SERVER     dhcp_server;

ALIGN_32BYTES(uint32_t DataBuffer[512]);

/* Set nx_server_pool start address to ".UsbxAppSection" */
#if defined ( __ICCARM__ ) /* IAR Compiler */
#pragma location = ".UsbxAppSection"
#elif defined ( __CC_ARM ) || defined(__ARMCC_VERSION) /* ARM Compiler 5/6 */
__attribute__((section(".UsbxAppSection")))
#elif defined ( __GNUC__ ) /* GNU Compiler */
__attribute__((section(".UsbxAppSection")))
#endif

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */
/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */

  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN MX_NetXDuo_Init */
  UCHAR *pointer;
  UINT addresses_added;

	/* Initialize the NetX system */
	nx_system_initialize();

  /* Allocate stack for the packet pool */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, NX_PACKET_POOL_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create the DHCP Server packet pool */
  if (nx_packet_pool_create(&net_packet_pool, "NetX Main Packet Pool",
                            PACKET_PAYLOAD_SIZE, pointer, NX_PACKET_POOL_SIZE) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Allocate stack for the IP instance */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 2048, TX_NO_WAIT) != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Creates an RNDIS Internet Protocol instance */
  if (nx_ip_create(&rndis_ip, "RNDIS IP Instance", RNDIS_IP_ADDRESS, RNDIS_NETMASK_ADDR,
                   &net_packet_pool, _ux_network_driver_entry, pointer,
                   2048, 3) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Allocate stack for ARP */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 2048, TX_NO_WAIT) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable ARP and supply ARP cache memory for IP Instance RNDIS */
  if (nx_arp_enable(&rndis_ip, pointer, 2048) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable TCP traffic */
  if (nx_tcp_enable(&rndis_ip) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable UDP traffic */
  if (nx_udp_enable(&rndis_ip) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable ICMP to enable the ping utility */
  if (nx_icmp_enable(&rndis_ip) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Enable fragment */
  if (nx_ip_fragment_enable(&rndis_ip) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Setup the Gateway address for previously created IP Instance RNDIS */
  if (nx_ip_gateway_address_set(&rndis_ip, RNDIS_GATEWAY_ADDR) != NX_SUCCESS)
  {
    Error_Handler();
  }

  /* Set the IP address and network mask to RNDIS_IP_ADDRESS and RNDIS_NETMASK_ADDR
     for the previously created IP Instance RNDIS. */
  nx_ip_address_set(&rndis_ip, RNDIS_IP_ADDRESS, RNDIS_NETMASK_ADDR);

//  /* Allocate stack for the DHCP */
//  if (tx_byte_allocate(byte_pool, (VOID **) &pointer, 2048, TX_NO_WAIT) != TX_SUCCESS)
//  {
//    Error_Handler();
//  }
//
//  /* Create the DHCP Server instance */
//  if (nx_dhcp_server_create(&dhcp_server, &rndis_ip, pointer, 2048,
//                            "RNDIS DHCP Server", &net_packet_pool) != NX_SUCCESS)
//  {
//    Error_Handler();
//  }
//
//  /* Create a IP address pool */
//  if (nx_dhcp_create_server_ip_address_list(&dhcp_server, 0, START_IP_ADDRESS_LIST,
//                                            END_IP_ADDRESS_LIST,
//                                            &addresses_added) != NX_SUCCESS)
//  {
//    Error_Handler();
//  }
//
//  /* Set network parameters for DHCP options */
//  if (nx_dhcp_set_interface_network_parameters(&dhcp_server, 0,
//                                               NX_DHCP_SUBNET_MASK,
//                                               NX_DHCP_SERVER_IP_ADDRESS,
//                                               NX_DHCP_DNS_IP_ADDRESS) != NX_SUCCESS)
//  {
//    Error_Handler();
//  }

  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */

/**
  * @brief  nx_server_thread_entry
  *         Application thread for HTTP web server
  * @param  thread_input: not used
  * @retval none
  */
VOID nx_server_thread_entry(ULONG thread_input)
{
  NX_PARAMETER_NOT_USED(thread_input);

//  /* Start DHCP Server processing */
//  if (nx_dhcp_server_start(&dhcp_server) != NX_SUCCESS)
//  {
//    Error_Handler();
//  }
}
/* USER CODE END 1 */
