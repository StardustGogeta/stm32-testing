/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_device.c
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
#include "app_usbx_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "ux_api.h"
#include "ux_system.h"
#include "ux_utility.h"
#include "ux_device_stack.h"
#include "ux_dcd_stm32.h"
#include "ux_device_descriptors.h"
//#include "ux_device_mouse.h"
//#include "ux_device_cdc_acm.h"
#include "ux_device_rndis.h"
#include "ux_device_cdc_ecm.h"
#include "app_azure_rtos_config.h"
#include "usb_drd_fs.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USBX_APP_STACK_SIZE     (2 * 1024)
#define USBX_MEMORY_SIZE        (25 * 1024)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
TX_THREAD                       ux_app_thread;


static ULONG rndis_interface_number;
static ULONG rndis_configuration_number;
static UCHAR rndis_local_nodeid[UX_DEVICE_CLASS_RNDIS_NODE_ID_LENGTH];
static UCHAR rndis_remote_nodeid[UX_DEVICE_CLASS_RNDIS_NODE_ID_LENGTH];
static UX_SLAVE_CLASS_RNDIS_PARAMETER rndis_parameter;
/*
static ULONG cdc_ecm_interface_number;
static ULONG cdc_ecm_configuration_number;
static UCHAR cdc_ecm_local_nodeid[UX_DEVICE_CLASS_CDC_ECM_NODE_ID_LENGTH];
static UCHAR cdc_ecm_remote_nodeid[UX_DEVICE_CLASS_CDC_ECM_NODE_ID_LENGTH];
*/
/*
TX_THREAD                       ux_hid_thread;
UX_SLAVE_CLASS_HID_EVENT        mouse_hid_event;
UX_SLAVE_CLASS_HID_PARAMETER    hid_parameter;
*/
//TX_THREAD                          ux_cdc_read_thread;
//TX_THREAD                          ux_cdc_write_thread;
//TX_THREAD                          ux_cdc_ecm_read_thread;
//TX_THREAD                          ux_cdc_ecm_write_thread;
TX_THREAD                          ux_cdc_rndis_read_thread;
TX_EVENT_FLAGS_GROUP               EventFlag;
//UX_SLAVE_CLASS_CDC_ACM_PARAMETER   cdc_acm_parameter;
UX_SLAVE_CLASS_CDC_ECM_PARAMETER   cdc_ecm_parameter;
/* ux app msg queue */
TX_QUEUE     ux_app_MsgQueue;
extern PCD_HandleTypeDef        hpcd_USB_DRD_FS;
#if defined ( __ICCARM__ ) /* IAR Compiler */
  #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN USB_MODE_STATE                            Event_Msg;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void usbx_app_thread_entry(ULONG arg);
/* USER CODE END PFP */
/**
  * @brief  Application USBX Device Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT MX_USBX_Device_Init(VOID *memory_ptr)
{
  UINT ret = UX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_USBX_Device_MEM_POOL */
  /* USER CODE END MX_USBX_Device_MEM_POOL */

  /* USER CODE BEGIN MX_USBX_Device_Init */
  UCHAR *pointer;
  /* Device framework FS length*/
  ULONG device_framework_fs_length;
  /* Device String framework length*/
  ULONG string_framework_length;
  /* Device language id framework length*/
  ULONG languge_id_framework_length;
  /* Device Framework Full Speed */
  UCHAR *device_framework_full_speed;
  /* String Framework*/
  UCHAR *string_framework;
  /* Language_Id_Framework*/
  UCHAR *language_id_framework;

  /* Allocate the stack for thread 0.  */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_MEMORY_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Initialize USBX Memory */
  if (ux_system_initialize(pointer, USBX_MEMORY_SIZE, UX_NULL, 0) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Get_Device_Framework_Full_Speed and get the length */
  device_framework_full_speed = USBD_Get_Device_Framework_Speed(USBD_FULL_SPEED,
                                &device_framework_fs_length);

  /* Get_String_Framework and get the length */
  string_framework = USBD_Get_String_Framework(&string_framework_length);

  /* Get_Language_Id_Framework and get the length */
  language_id_framework = USBD_Get_Language_Id_Framework(&languge_id_framework_length);

//  unsigned char device_framework_full_speed2[] = {
//
//      /* Device descriptor 18 bytes
//      0x02 bDeviceClass: CDC_ECM class code
//      0x06 bDeviceSubclass: CDC_ECM class sub code
//      0x00 bDeviceProtocol: CDC_ECM Device protocol
//      idVendor & idProduct - https://www.linux-usb.org/usb.ids
//      0x3939 idVendor: Azure RTOS test.
//      */
//
//      0x12, 0x01, 0x10, 0x01,
//      0x02, 0x00, 0x00, 0x08,
//      0x39, 0x39, 0x08, 0x08, 0x00, 0x01, 0x01, 0x02, 03,0x01,
//
//      /* Configuration 1 descriptor 9 bytes. */
//      0x09, 0x02, 0x58, 0x00,0x02, 0x01, 0x00,0x40, 0x00,
//
//      /* Interface association descriptor. 8 bytes. */
//
//      0x08, 0x0b, 0x00, 0x02, 0x02, 0x06, 0x00, 0x00,
//
//      /* Communication Class Interface Descriptor Requirement 9 bytes */
//      0x09, 0x04, 0x00, 0x00,0x01,0x02, 0x06, 0x00, 0x00,
//
//      /* Header Functional Descriptor 5 bytes */
//      0x05, 0x24, 0x00, 0x10, 0x01,
//
//      /* ECM Functional Descriptor 13 bytes */
//      0x0D, 0x24, 0x0F, 0x04,0x00, 0x00, 0x00, 0x00, 0xEA, 0x05, 0x00,
//      0x00,0x00,
//
//      /* Union Functional Descriptor 5 bytes */
//      0x05, 0x24, 0x06, 0x00,0x01,
//
//      /* Endpoint descriptor (Interrupt) */
//      0x07, 0x05, 0x83, 0x03, 0x08, 0x00, 0x08,
//
//      /* Data Class Interface Descriptor Alternate Setting 0, 0 endpoints. 9 bytes */
//      0x09, 0x04, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00,
//
//      /* Data Class Interface Descriptor Alternate Setting 1, 2 endpoints. 9 bytes */
//      0x09, 0x04, 0x01, 0x01, 0x02, 0x0A, 0x00, 0x00,0x00,
//
//      /* First alternate setting Endpoint 1 descriptor 7 bytes */
//      0x07, 0x05, 0x02, 0x02, 0x40, 0x00, 0x00,
//
//      /* Endpoint 2 descriptor 7 bytes */
//      0x07, 0x05, 0x81, 0x02, 0x40, 0x00,0x00
//
//  };
//
//  unsigned char string_framework2[] = {
//      /* Manufacturer string descriptor: Index 1 - "Azure RTOS" */
//      0x09, 0x04, 0x01, 0x0c,
//      0x45, 0x78, 0x70, 0x72, 0x65, 0x73, 0x20, 0x4c,
//      0x6f, 0x67, 0x69, 0x63,
//
//      /* Product string descriptor: Index 2 - "EL CDCECM Device" */
//      0x09, 0x04, 0x02, 0x10,
//      0x45, 0x4c, 0x20, 0x43, 0x44, 0x43, 0x45, 0x43,
//      0x4d, 0x20, 0x44, 0x65, 0x76, 0x69, 0x63, 0x64,
//
//      /* Serial Number string descriptor: Index 3 - "0001" */
//      0x09, 0x04, 0x03, 0x04,
//      0x30, 0x30, 0x30, 0x31,
//
//      /* MAC Address string descriptor: Index 4 - "001E5841B879" */
//      0x09, 0x04, 0x04, 0x0C,
//      0x30, 0x30, 0x31, 0x45, 0x35, 0x38,
//      0x34, 0x31, 0x42, 0x38, 0x37, 0x39
//
//  };
//
//  unsigned char language_id_framework2[] = {
//		    /* English. */
//		    0x09, 0x04
//		};

  /* The code below is required for installing the device portion of USBX.
     In this application */
  if (ux_device_stack_initialize(NULL,
                                 0U,
                                 device_framework_full_speed,
                                 //sizeof(device_framework_full_speed2),
								 device_framework_fs_length,
                                 string_framework,
                                 //sizeof(string_framework2),
                                 string_framework_length,
                                 language_id_framework,
								 //sizeof(language_id_framework),
                                 languge_id_framework_length,
                                 UX_NULL) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Initialize the cdc class parameters for the device. */
    //cdc_acm_parameter.ux_slave_class_cdc_acm_instance_activate = CDC_Init_FS;

    /* Deinitialize the cdc class parameters for the device. */
    //cdc_acm_parameter.ux_slave_class_cdc_acm_instance_deactivate = CDC_DeInit_FS;

    /* Manage the CDC class requests */
    //cdc_acm_parameter.ux_slave_class_cdc_acm_parameter_change = ux_app_parameters_change;

    /* Registers a slave class to the slave stack. The class is connected with
       interface 0 */
    /*if (ux_device_stack_class_register(_ux_system_slave_class_cdc_acm_name,
                                       ux_device_class_cdc_acm_entry, 1, 0,
                                       (VOID *)&cdc_acm_parameter) != UX_SUCCESS)
    {
      return UX_ERROR;
    }*/

/*
    // Initialize the cdc class parameters for the device.
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_instance_activate = USBD_CDC_ECM_Activate;

	// Deinitialize the cdc class parameters for the device.
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_instance_deactivate = USBD_CDC_ECM_Deactivate;

	// Get CDC ECM local MAC address
	USBD_CDC_ECM_GetMacAdd((uint8_t *)CDC_ECM_MAC_STR_DESC, cdc_ecm_local_nodeid);

	// Define CDC ECM local node id
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[0] = cdc_ecm_local_nodeid[0];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[1] = cdc_ecm_local_nodeid[1];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[2] = cdc_ecm_local_nodeid[2];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[3] = cdc_ecm_local_nodeid[3];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[4] = cdc_ecm_local_nodeid[4];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[5] = cdc_ecm_local_nodeid[5];

	// Get CDC ECM remote MAC address
	USBD_CDC_ECM_GetMacAdd((uint8_t *)CDC_ECM_MAC_STR_DESC, cdc_ecm_remote_nodeid);

	// Define CDC ECM remote node id
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[0] = cdc_ecm_remote_nodeid[0];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[1] = cdc_ecm_remote_nodeid[1];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[2] = cdc_ecm_remote_nodeid[2];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[3] = cdc_ecm_remote_nodeid[3];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[4] = cdc_ecm_remote_nodeid[4];
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[5] = cdc_ecm_remote_nodeid[5];

	 //cdc_ecm_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_CDC_ECM, 0);

	// Find cdc ecm interface number
	//cdc_ecm_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_CDC_ECM, 0);

	// Manage the CDC class requests
	//cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_change = ux_ecm_app_parameters_change;

	// Registers a slave class to the slave stack. The class is connected with
	// interface 0
	if (ux_device_stack_class_register(_ux_system_slave_class_cdc_ecm_name,
									 ux_device_class_cdc_ecm_entry, 1, 0,
									 (VOID *)&cdc_ecm_parameter) != UX_SUCCESS)
	{
		return UX_ERROR;
	}
	*/
/*
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_instance_activate = UX_NULL;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_instance_deactivate = UX_NULL;

	// Define a NODE ID
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[0] = 0x00;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[1] = 0x1e;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[2] = 0x58;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[3] = 0x41;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[4] = 0xb8;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_local_node_id[5] = 0x78;

	// Define a remote NODE ID.
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[0] = 0x00;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[1] = 0x1e;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[2] = 0x58;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[3] = 0x41;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[4] = 0xb8;
	cdc_ecm_parameter.ux_slave_class_cdc_ecm_parameter_remote_node_id[5] = 0x79;
	*/


  	// Initialize the rndis class parameters for the device
    rndis_parameter.ux_slave_class_rndis_instance_activate   = USBD_RNDIS_Activate;
    rndis_parameter.ux_slave_class_rndis_instance_deactivate = USBD_RNDIS_Deactivate;

    // Get RNDIS local MAC address
    USBD_RNDIS_GetMacAdd((uint8_t *)RNDIS_LOCAL_MAC_STR_DESC, rndis_local_nodeid);

// For reference from device descriptor file:
//#define RNDIS_LOCAL_MAC_STR_DESC                      (uint8_t *)"000202030000"
//#define RNDIS_REMOTE_MAC_STR_DESC                     (uint8_t *)"000202030001"

    // Define RNDIS local node id
    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[0] = rndis_local_nodeid[0];
    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[1] = rndis_local_nodeid[1];
    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[2] = rndis_local_nodeid[2];
    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[3] = rndis_local_nodeid[3];
    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[4] = rndis_local_nodeid[4];
    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[5] = rndis_local_nodeid[5];
//    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[0] = 0;
//    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[1] = 2;
//    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[2] = 2;
//    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[3] = 3;
//    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[4] = 0;
//    rndis_parameter.ux_slave_class_rndis_parameter_local_node_id[5] = 0;

    // Get RNDIS local MAC address
    USBD_RNDIS_GetMacAdd((uint8_t *)RNDIS_REMOTE_MAC_STR_DESC, rndis_remote_nodeid);

    // Define RNDIS remote node id
    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[0] = rndis_remote_nodeid[0];
    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[1] = rndis_remote_nodeid[1];
    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[2] = rndis_remote_nodeid[2];
    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[3] = rndis_remote_nodeid[3];
    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[4] = rndis_remote_nodeid[4];
    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[5] = rndis_remote_nodeid[5];
//    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[0] = 0;
//    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[1] = 2;
//    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[2] = 2;
//    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[3] = 3;
//    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[4] = 0;
//    rndis_parameter.ux_slave_class_rndis_parameter_remote_node_id[5] = 1;

    rndis_parameter.ux_slave_class_rndis_parameter_vendor_id      = USBD_VID;
    rndis_parameter.ux_slave_class_rndis_parameter_driver_version = USBD_RNDIS_DRIVER_VERSION;

    ux_utility_memory_copy(rndis_parameter.ux_slave_class_rndis_parameter_vendor_description,
                           USBD_PRODUCT_STRING, sizeof(USBD_PRODUCT_STRING));

    // Get rndis configuration number
    rndis_configuration_number = USBD_Get_Configuration_Number(CLASS_TYPE_RNDIS, 0);

    // Find rndis interface number
    rndis_interface_number = USBD_Get_Interface_Number(CLASS_TYPE_RNDIS, 0);

    // Initialize the device RNDIS
    if (ux_device_stack_class_register(_ux_system_slave_class_rndis_name,
                                       ux_device_class_rndis_entry,
                                       rndis_configuration_number,
                                       rndis_interface_number,
                                       &rndis_parameter) != UX_SUCCESS)
    {
      return UX_ERROR;
    }

	/*int status = 0;
	// Initialize the device cdc_ecm class.
	if ( (status = ux_device_stack_class_register(_ux_system_slave_class_cdc_ecm_name,
		    ux_device_class_cdc_ecm_entry, 1,0,&cdc_ecm_parameter)) != UX_SUCCESS )
	{
		return status;
	}*/
	//nx_system_initialize();
	ux_network_driver_init();

  /* Initialize the hid class parameters for the device. */
  /*hid_parameter.ux_device_class_hid_parameter_report_address = USBD_Get_Device_HID_MOUSE_ReportDesc();

  hid_parameter.ux_device_class_hid_parameter_report_length = USBD_HID_MOUSE_REPORT_DESC_SIZE;

  hid_parameter.ux_device_class_hid_parameter_report_id = UX_TRUE;

  hid_parameter.ux_device_class_hid_parameter_callback = app_usbx_device_thread_hid_callback;*/

  /* Initialize the device hid class. The class is connected with interface 0 */
  /*if (ux_device_stack_class_register(_ux_system_slave_class_hid_name,
                                     ux_device_class_hid_entry, 1, 0,
                                     (VOID *)&hid_parameter) != UX_SUCCESS)
  {
    return UX_ERROR;
  }*/

  /* Allocate the stack for main_usbx_app_thread_entry. */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the main thread. */
  /*if (tx_thread_create(&ux_app_thread, "main_usbx_app_thread_entry",
                       usbx_app_thread_entry, 0, pointer, USBX_APP_STACK_SIZE,
                       20, 20, 1, TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }*/
  
  /* Create the main thread. */
  if (tx_thread_create(&ux_app_thread, "main_usbx_app_thread_entry",
                       usbx_app_thread_entry, 0, pointer, USBX_APP_STACK_SIZE,
                       20, 20, TX_NO_TIME_SLICE, TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Allocate the stack for usbx_hid_thread_entry.  */
  /*if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }*/

  /* Create the usbx_hid_thread_entry thread. */
  /*if (tx_thread_create(&ux_hid_thread, "hid_usbx_app_thread_entry",
                       usbx_hid_thread_entry, 0,
                       pointer, USBX_APP_STACK_SIZE, 20, 20,
                       1, TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }*/

  // Allocate the stack for usbx_cdc_acm_read_thread_entry.
  /*if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  // Create the usbx_cdc_acm_thread_entry thread.
  if (tx_thread_create(&ux_cdc_read_thread, "cdc_acm_read_usbx_app_thread_entry",
                       usbx_cdc_acm_read_thread_entry, 1, pointer,
                       USBX_APP_STACK_SIZE, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  // Allocate the stack for usbx_cdc_acm_write_thread_entry.
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  // Create the usbx_cdc_acm_thread_entry thread.
  if (tx_thread_create(&ux_cdc_write_thread, "cdc_acm_write_usbx_app_thread_entry",
                       usbx_cdc_acm_write_thread_entry, 1, pointer,
                       USBX_APP_STACK_SIZE, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }*/

  // Allocate the stack for usbx_cdc_ecm_read_thread_entry.
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       USBX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }


  // Create the usbx_cdc_ecm_thread_entry thread.

  /*if (tx_thread_create(&ux_cdc_ecm_read_thread, "cdc_ecm_read_usbx_app_thread_entry",
                       usbx_cdc_ecm_read_thread_entry, 1, pointer,
                       USBX_APP_STACK_SIZE, 20, 20, TX_NO_TIME_SLICE,
                       TX_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }*/

  /*if (tx_thread_create(&ux_cdc_rndis_read_thread, "cdc_rndis_read_usbx_app_thread_entry",
                         usbx_cdc_rndis_read_thread_entry, 1, pointer,
                         USBX_APP_STACK_SIZE, 20, 20, TX_NO_TIME_SLICE,
                         TX_AUTO_START) != TX_SUCCESS)
    {
      return TX_THREAD_ERROR;
    }*/


  /* Create the event flags group. */
  if (tx_event_flags_create(&EventFlag, "Event Flag") != TX_SUCCESS)
  {
    return TX_GROUP_ERROR;
  }

  /* Allocate Memory for the Queue */
  if (tx_byte_allocate(byte_pool, (VOID **) &pointer,
                       sizeof(APP_QUEUE_SIZE*sizeof(ULONG)),
                       TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }

  /* Create the MsgQueue */
  if (tx_queue_create(&ux_app_MsgQueue, "Message Queue app",
                      TX_1_ULONG, pointer,
                      APP_QUEUE_SIZE*sizeof(ULONG)) != TX_SUCCESS)
  {
    return TX_QUEUE_ERROR;
  }
  /* USER CODE END MX_USBX_Device_Init */

  return ret;
}

/* USER CODE BEGIN 1 */
/**
  * @brief  Function implementing usbx_app_thread_entry.
  * @param arg: Not used
  * @retval None
  */
void usbx_app_thread_entry(ULONG arg)
{
  /* Initialization of USB device */
  MX_USB_Device_Init();
}

/**
  * @brief MX_USB_Device_Init
  *        Initialization of USB device.
  * Init USB device Library, add supported class and start the library
  * @retval None
  */
void MX_USB_Device_Init(void)
{
  /* USER CODE BEGIN USB_Device_Init_PreTreatment_0 */
  /* USER CODE END USB_Device_Init_PreTreatment_0 */

  MX_USB_DRD_FS_PCD_Init();

  /* USER CODE BEGIN USB_Device_Init_PreTreatment_1 */
  
  // This was the original block
//  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x00, PCD_SNG_BUF, 0x0C);
//  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x80, PCD_SNG_BUF, 0x4C);
//  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x81, PCD_SNG_BUF, 0x8C);
  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x00, PCD_SNG_BUF, 0x14);
  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x80, PCD_SNG_BUF, 0x54);
  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x81, PCD_SNG_BUF, 0x94);
  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x01, PCD_SNG_BUF, 0xD4);
  HAL_PCDEx_PMAConfig(&hpcd_USB_DRD_FS, 0x82, PCD_SNG_BUF, 0x114);
  /* USER CODE END USB_Device_Init_PreTreatment_1 */
//
//  HAL_PCDEx_SetRxFiFo(&hpcd_USB_DRD_FS, 0x200);
//  HAL_PCDEx_SetTxFiFo(&hpcd_USB_DRD_FS, 0, 0x10);
//  HAL_PCDEx_SetTxFiFo(&hpcd_USB_DRD_FS, 1, 0x80);
//  HAL_PCDEx_SetTxFiFo(&hpcd_USB_DRD_FS, 2, 0x20);

  /* initialize the device controller driver*/
  _ux_dcd_stm32_initialize((ULONG)USB_DRD_FS, (ULONG)&hpcd_USB_DRD_FS);

  /* USER CODE BEGIN USB_Device_Init_PostTreatment */
  HAL_PCD_Start(&hpcd_USB_DRD_FS);
  /* USER CODE END USB_Device_Init_PostTreatment */
}

/* USER CODE END 1 */
