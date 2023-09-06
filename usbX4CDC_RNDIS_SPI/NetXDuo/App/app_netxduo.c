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
#define 	DEFAULT_PORT			6000
#define		LINK_PRIORITY			11
#define 	NX_APP_CABLE_CONNECTION_CHECK_PERIOD  (NX_IP_PERIODIC_RATE)
#define 	NX_APP_THREAD_STACK_SIZE	2 * 1024
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
NX_IP              rndis_ip;
NX_PACKET_POOL     net_packet_pool;
//NX_DHCP_SERVER     dhcp_server;
TX_THREAD               AppLinkThread;
TX_THREAD               my_thread;
NX_DHCP                 my_dhcp;
NX_IP                   my_ip;
int shouldGo = 0;

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
static VOID App_Link_Thread_Entry(ULONG thread_input);
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

  /* Allocate the stack for my_thread_entry. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
		  DEMO_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create “my_thread�?.  */
  tx_thread_create(&my_thread, "my thread", my_thread_entry, 0,
					pointer, DEMO_STACK_SIZE, 2, 2, TX_NO_TIME_SLICE, TX_AUTO_START);

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

  /* Allocate the memory for Link thread   */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,NX_APP_THREAD_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* create the Link thread */
  ret = tx_thread_create(&AppLinkThread, "App Link Thread", App_Link_Thread_Entry, 0, pointer, NX_APP_THREAD_STACK_SIZE,
                         LINK_PRIORITY, LINK_PRIORITY, TX_NO_TIME_SLICE, TX_AUTO_START);

  if (ret != TX_SUCCESS)
  {
    return NX_NOT_ENABLED;
  }

  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */

void    my_thread_entry(ULONG thread_input)
 {
	// Assumes PC IP address is set to 192.168.0.1
	// This device has IP address 192.168.0.10 by default
	ULONG svr_addr = IP_ADDRESS(192,168,0,1);
	NX_PACKET *my_packet;
	NX_UDP_SOCKET udp_socket;
	int status;

	if (nx_udp_socket_create(&rndis_ip, &udp_socket, "UDP Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 1) != NX_SUCCESS)
	{
	  Error_Handler();
	}
	int stat;
	if ((stat = nx_udp_socket_bind(&udp_socket, DEFAULT_PORT, NX_WAIT_FOREVER)) != NX_SUCCESS)
	{
	  Error_Handler();
	}

	int i = 0;
	char buf[16] = {};
	while (1)
	{
		if (shouldGo)
		{
			sprintf(buf, "Hello world %d", (i++) % 10);
			status = nx_packet_allocate(&net_packet_pool, &my_packet, NX_UDP_PACKET, NX_WAIT_FOREVER);
			status = nx_packet_data_append(my_packet, buf, strlen(buf), &net_packet_pool, NX_WAIT_FOREVER);
			status = nx_udp_socket_send(&udp_socket, my_packet, svr_addr, DEFAULT_PORT);
			status = nx_packet_release(my_packet);
		}
		tx_thread_sleep(20);
	}
}

/**
* @brief  Link thread entry
* @param thread_input: ULONG thread parameter
* @retval none
*/
static VOID App_Link_Thread_Entry(ULONG thread_input)
{
  ULONG actual_status;
  UINT linkdown = 0, status;

  while(1)
  {
    /* Get Physical Link status. */
    status = nx_ip_interface_status_check(&rndis_ip, 0, NX_IP_LINK_ENABLED,
                                      &actual_status, 10);

    if(status == NX_SUCCESS)
    {
      if(linkdown == 1)
      {
        linkdown = 0;
        status = nx_ip_interface_status_check(&rndis_ip, 0, NX_IP_ADDRESS_RESOLVED,
                                      &actual_status, 10);
        if(status == NX_SUCCESS)
        {
          /* The network cable is connected again. */
          printf("The network cable is connected again.\n");
          /* Print UDP Echo Server is available again. */
          printf("UDP Echo Server is available again.\n");
        }
        else
        {
          /* The network cable is connected. */
          printf("The network cable is connected.\n");
          /* Send command to Enable Nx driver. */
          nx_ip_driver_direct_command(&rndis_ip, NX_LINK_ENABLE,
                                      &actual_status);
          /* Restart DHCP Client. */
//          nx_dhcp_stop(&DHCPClient);
//          nx_dhcp_start(&DHCPClient);
        }
      }
    }
    else
    {
      if(0 == linkdown)
      {
        linkdown = 1;
        /* The network cable is not connected. */
        printf("The network cable is not connected.\n");
      }
    }
    shouldGo = !linkdown;

    tx_thread_sleep(NX_APP_CABLE_CONNECTION_CHECK_PERIOD);
  }
}
/* USER CODE END 1 */
