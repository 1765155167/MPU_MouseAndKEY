/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Joystick Mouse Demo
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#define uint8_t u8
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t Joystick_DeviceDescriptor[JOYSTICK_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
    0x83,                       /*idVendor (0x0483)*/
    0x04,
    0x10,                       /*idProduct = 0x5710*/
    0x57,
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Joystick_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const u8 Joystick_ConfigDescriptor[JOYSTICK_SIZ_CONFIG_DESC] =
{
 /***************����������***********************/
 0x09,  //bLength�ֶ�
 USB_CONFIGURATION_DESCRIPTOR_TYPE,  //bDescriptorType�ֶ�
 //wTotalLength�ֶ�
 JOYSTICK_SIZ_CONFIG_DESC,
 /* wTotalLength: Bytes returned */
 0x00,
 
 0x02, //bNumInterfaces�ֶ�
 0x01, //bConfiguration�ֶ�
 0x00, //iConfigurationz�ֶ�
 0x80, //bmAttributes�ֶ�
 0x32, //bMaxPower�ֶ�
 
 /*******************��һ���ӿ�������*********************/
 0x09, //bLength�ֶ�
 0x04, //bDescriptorType�ֶ�
 0x00, //bInterfaceNumber�ֶ�
 0x00, //bAlternateSetting�ֶ�
 0x02, //bNumEndpoints�ֶ�
 0x03, //bInterfaceClass�ֶ�
 0x01, //bInterfaceSubClass�ֶ�
 0x01, //bInterfaceProtocol�ֶ�
 0x00, //iConfiguration�ֶ�
 
 /******************HID������************************/
 0x09, //bLength�ֶ�
 0x21, //bDescriptorType�ֶ�
 0x10, //bcdHID�ֶ�
 0x01,
 0x21, //bCountyCode�ֶ�
 0x01, //bNumDescriptors�ֶ�
 0x22, //bDescriptorType�ֶ�
 
 //bDescriptorLength�ֶΡ�
 //�¼��������ĳ��ȡ��¼�������Ϊ���̱�����������
 sizeof(KeyboardReportDescriptor)&0xFF,
 (sizeof(KeyboardReportDescriptor)>>8)&0xFF,
 
 /**********************����˵�������***********************/
 0x07, //bLength�ֶ�
 0x05, //bDescriptorType�ֶ�
 0x81, //bEndpointAddress�ֶ�
 0x03, //bmAttributes�ֶ�
 0x10, //wMaxPacketSize�ֶ�
 0x00,
 0x0A, //bInterval�ֶ�
 
 /**********************����˵�������***********************/
 0x07, //bLength�ֶ�
 0x05, //bDescriptorType�ֶ�
 0x01, //bEndpointAddress�ֶ�
 0x03, //bmAttributes�ֶ�
 0x10, //wMaxPacketSize�ֶ�
 0x00,
 0x0A, //bInterval�ֶ�
 
 /*******************�ڶ����ӿ�������*********************/
 0x09, //bLength�ֶ�
 0x04, //bDescriptorType�ֶ�
 0x01, //bInterfaceNumber�ֶ�
 0x00, //bAlternateSetting�ֶ�
 0x01, //bNumEndpoints�ֶ�
 0x03, //bInterfaceClass�ֶ�
 0x01, //bInterfaceSubClass�ֶ�
 0x02, //bInterfaceProtocol�ֶ�
 0x00, //iConfiguration�ֶ�
 
 /******************HID������************************/
 0x09, //bLength�ֶ�
 0x21, //bDescriptorType�ֶ�
 0x10, //bcdHID�ֶ�
 0x01,
 0x21, //bCountyCode�ֶ�
 0x01, //bNumDescriptors�ֶ�
 0x22, //bDescriptorType�ֶ�
 sizeof(MouseReportDescriptor)&0xFF,  //bDescriptorLength�ֶ�
 (sizeof(MouseReportDescriptor)>>8)&0xFF,
 
 /**********************����˵�������***********************/
 0x07, //bLength�ֶ�
 0x05, //bDescriptorType�ֶ�
 0x82, //bEndpointAddress�ֶ�
 0x03, //bmAttributes�ֶΡ�D1~D0Ϊ�˵㴫������ѡ��
 0x40, //wMaxPacketSize�ֶ�
 0x00,
 0x0A  //bInterval�ֶ�
}; /* MOUSE_ConfigDescriptor */
const uint8_t Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC] =
  {
    0x05,          /*Usage Page(Generic Desktop)*/
    0x01,
    0x09,          /*Usage(Mouse)*/
    0x02,
    0xA1,          /*Collection(Logical)*/
    0x01,
    0x09,          /*Usage(Pointer)*/
    0x01,
    /* 8 */
    0xA1,          /*Collection(Linked)*/
    0x00,
    0x05,          /*Usage Page(Buttons)*/
    0x09,
    0x19,          /*Usage Minimum(1)*/
    0x01,
    0x29,          /*Usage Maximum(3)*/
    0x03,
    /* 16 */
    0x15,          /*Logical Minimum(0)*/
    0x00,
    0x25,          /*Logical Maximum(1)*/
    0x01,
    0x95,          /*Report Count(3)*/
    0x03,
    0x75,          /*Report Size(1)*/
    0x01,
    /* 24 */
    0x81,          /*Input(Variable)*/
    0x02,
    0x95,          /*Report Count(1)*/
    0x01,
    0x75,          /*Report Size(5)*/
    0x05,
    0x81,          /*Input(Constant,Array)*/
    0x01,
    /* 32 */
    0x05,          /*Usage Page(Generic Desktop)*/
    0x01,
    0x09,          /*Usage(X axis)*/
    0x30,
    0x09,          /*Usage(Y axis)*/
    0x31,
    0x09,          /*Usage(Wheel)*/
    0x38,
    /* 40 */
    0x15,          /*Logical Minimum(-127)*/
    0x81,
    0x25,          /*Logical Maximum(127)*/
    0x7F,
    0x75,          /*Report Size(8)*/
    0x08,
    0x95,          /*Report Count(3)*/
    0x03,
    /* 48 */
    0x81,          /*Input(Variable, Relative)*/
    0x06,
    0xC0,          /*End Collection*/
    0x09,
    0x3c,
    0x05,
    0xff,
    0x09,
    /* 56 */
    0x01,
    0x15,
    0x00,
    0x25,
    0x01,
    0x75,
    0x01,
    0x95,
    /* 64 */
    0x02,
    0xb1,
    0x22,
    0x75,
    0x06,
    0x95,
    0x01,
    0xb1,
    /* 72 */
    0x01,
    0xc0
  }
  ; /* Joystick_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t Joystick_StringLangID[JOYSTICK_SIZ_STRING_LANGID] =
  {
    JOYSTICK_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Joystick_StringVendor[JOYSTICK_SIZ_STRING_VENDOR] =
  {
    JOYSTICK_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const uint8_t Joystick_StringProduct[JOYSTICK_SIZ_STRING_PRODUCT] =
{
	JOYSTICK_SIZ_STRING_PRODUCT,          /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
	'A', 0, 'L', 0, 'I', 0, 'E', 0, 'N', 0, 'T', 0, 'E', 0, 'K', 0,//ALIENTEK
	' ', 0, 
	0XE6,0X89,//��
	0XA7,0X63,//��
	'U', 0,
	'S', 0, 
	'B', 0,
	0X20,0X9F,//��
	0X07,0X68,//�� 
};

uint8_t Joystick_StringSerial[JOYSTICK_SIZ_STRING_SERIAL] =
  {
    JOYSTICK_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
//USB���̱����������Ķ���
const u8 KeyboardReportDescriptor[KP_ReportDescriptor_Size]=
{
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop) //63
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x03,                    //   OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                           // END_COLLECTION
//0xc0,
  }; /* Joystick_ReportDescriptor */
///////////////////////////���̱������������////////////////////////////
 
//USB��걨���������Ķ���
const u8 MouseReportDescriptor[Mouse_ReportDescriptor_Size]=
{
 0x05, 0x01, // USAGE_PAGE (Generic Desktop)
 0x09, 0x02, // USAGE (Mouse)
 0xa1, 0x01, // COLLECTION (Application)
 0x85, 0x01, //Report ID (1)
 0x09, 0x01, //   USAGE (Pointer)
 0xa1, 0x00, //   COLLECTION (Physical)
 0x05, 0x09, //     USAGE_PAGE (Button)
 0x19, 0x01, //     USAGE_MINIMUM (Button 1)
 0x29, 0x03, //     USAGE_MAXIMUM (Button 3)
 0x15, 0x00, //     LOGICAL_MINIMUM (0)
 0x25, 0x01, //     LOGICAL_MAXIMUM (1)
 0x95, 0x03, //     REPORT_COUNT (3)
 0x75, 0x01, //     REPORT_SIZE (1)
 0x81, 0x02, //     INPUT (Data,Var,Abs)
 0x95, 0x01, //     REPORT_COUNT (1)
 0x75, 0x05, //     REPORT_SIZE (5)
 0x81, 0x03, //     INPUT (Cnst,Var,Abs)
 0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
 0x09, 0x30, //     USAGE (X)
 0x09, 0x31, //     USAGE (Y)
 0x09, 0x38, //     USAGE (Wheel)
 0x15, 0x81, //     LOGICAL_MINIMUM (-127)
 0x25, 0x7f, //     LOGICAL_MAXIMUM (127)
 0x75, 0x08, //     REPORT_SIZE (8)
 0x95, 0x03, //     REPORT_COUNT (3)
 0x81, 0x06, //     INPUT (Data,Var,Rel)
 0xc0,       //   END_COLLECTION
 0xc0        // END_COLLECTION
};


