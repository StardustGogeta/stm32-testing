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
#include "nxd_dhcp_client.h"
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
#define     DEMO_STACK_SIZE         4096

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
NX_IP              rndis_ip;
NX_PACKET_POOL     net_packet_pool;
//NX_DHCP_SERVER     dhcp_server;
TX_THREAD               my_thread;
NX_DHCP                 my_dhcp;
NX_IP                   my_ip;

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
void    my_thread_entry(ULONG thread_input);
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

//  /* Allocate the stack for my_thread_entry. */
//  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
//		  DEMO_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
//  {
//    return TX_POOL_ERROR;
//  }
//
//  /* Create “my_thread�?.  */
//  tx_thread_create(&my_thread, "my thread", my_thread_entry, 0,
//					pointer, DEMO_STACK_SIZE, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

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

void    my_thread_entry(ULONG thread_input)
 {
//	UINT status;
//	ULONG actual_status;
//	NX_PACKET *my_packet;
//	int error_counter = 0;
//	/* Wait for the link to come up.  */
//	do
//	{
//	/* Get the link status.  */
//		status =  nx_ip_status_check(&rndis_ip, NX_IP_LINK_ENABLED,
//									 &actual_status, 100);
//	} while (status != NX_SUCCESS);
//
//	/* Create a DHCP instance.  */
//	status =  nx_dhcp_create(&my_dhcp, &rndis_ip, "My DHCP");
//
//	/* Check for DHCP create error.  */
//	if (status)
//		error_counter++;
//
//	/* Start DHCP.  */
//	nx_dhcp_start(&my_dhcp);
//
//	/* Check for DHCP start error.  */
//	if (status)
//		error_counter++;
//
//	/* Wait for IP address to be resolved through DHCP.  */
//	nx_ip_status_check(&rndis_ip, NX_IP_ADDRESS_RESOLVED,
//					   (ULONG *) &status, 100000);
//
//	/* Check to see if we have a valid IP address.  */
//	if (status != NX_IP_ADDRESS_RESOLVED)
//	{
//	  error_counter++;
//	  return;
//	}
//	else
//	{
//
//	/* Yes, a valid IP address is now on lease…  All NetX Duo
//		services are available. */
//	}
}

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
