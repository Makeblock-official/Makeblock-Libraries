/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeUSBHost
 * \brief   Driver for Me USB Host module.
 * @file    MeUSBHost.h
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/01/20
 * @brief   Header for MeUSBHost.cpp module
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2016, MakeBlock. Use is subject to license \n
 * conditions. The main licensing options available are GPL V2 or Commercial: \n
 *
 * \par Open Source Licensing GPL V2
 * This is the appropriate option if you want to share the source code of your \n
 * application with everyone you distribute it to, and you also want to give them \n
 * the right to share who uses it. If you wish to use this software under Open \n
 * Source Licensing, you must contribute all your source code to the open source \n
 * community in accordance with the GPL Version 2 when your application is \n
 * distributed. See http://www.gnu.org/copyleft/gpl.html
 *
 * \par Description
 * This file is a drive for Me USB Host device, The USB Host inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *
 *    1. void MeUSBHost::init(int8_t type);
 *    2. int16_t MeUSBHost::initHIDDevice();
 *    3. int16_t MeUSBHost::probeDevice();
 *    4. void MeUSBHost::resetBus();
 *    5. uint8_t MeUSBHost::host_recv();
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * forfish         2015/11/10     1.0.0            Add description
 * Mark Yan        2016/01/20     1.0.1            Support hardware serial automatic Identification
 * </pre>
 *
 * @example TestUSBHsot.ino
 */

#ifndef MeUSBHost_H
#define MeUSBHost_H

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "MeConfig.h"
#include "MePort.h"
#include "MeSerial.h"
#define USB2_0 1
#define USB1_0 0

#define	CH375_MAX_DATA_LEN	0x40			/* max package length */

	/* ********************************************************************************************************************* */
	/* Commands */

#define	CMD_RESET_ALL		0x05			/* Reset Device */

#define	CMD_CHECK_EXIST		0x06			/* Chip check */
	/* Input: any (ep:0x12)*/
	/* Output: output inverse of input (ep:0x21) */

#define	CMD_SET_USB_ID		0x12			/* Device Mode: Setup the VID and PID in device mode */
	/* Input: VID lower byte, VID higher byte, PID lower byte, PID higher byte */

#define	CMD_SET_USB_ADDR	0x13			/* Setup the usb address */
	/* Input: address value */

#define	CMD_SET_USB_MODE	0x15			/* Setup the module work mode, with enable or disable state */
	/* Input: mode */
	/*		 00H=Device mode disabled, 01H=Device mode enabled with external firmware, 02H=Device mode enabled with internal firmware, 03H=Device mode enabled with internal firmware and interrupt endpoint */
	/*		 04H=Host Mode disabled, 05H=Host Mode enabled, 06H=Host Mode enabled with SOF package, 07H=Host Mode enabled and reset USB bus */
	/* Output: status( CMD_RET_SUCCESS or CMD_RET_ABORT, otherswise unfinished ) */

#define	CMD_SET_ENDP2		0x18			/* Device Mode: setup endpoint 0 receiver */
	/* Input: Workmode */
	/*			 Bit 7~6: 1x set sync trigger flag to x, 0x don't change the sync flag */
	/*			 Bit3~0 token ack type:  0000-Ready ACK, 1110-Busy NAK, 1111-Error STALL */

#define	CMD_SET_ENDP3		0x19			/* Device Mode: setup endpoint 0 transmitter */

#define	CMD_SET_ENDP4		0x1A			/* Device Mode: setup endpoint 1 receiver */

#define	CMD_SET_ENDP5		0x1B			/* Device Mode: setup endpoint 1 transmitter */

#define	CMD_SET_ENDP6		0x1C			/* Device Mode: setup endpoint 2 receiver */

#define	CMD_SET_ENDP7		0x1D			/* Device Mode: setup endpoint 2 transmitter */


#define	CMD_GET_TOGGLE		0x0A			/* get the out token sync state */
	/* Input: 0x1A */
	/* Output: sync state */
	/*			 bit4: 1:OUT token synced, 0:OUT token unsync */

#define	CMD_GET_STATUS		0x22			/* Get interrupt state */
	/* Output: interrupt state */

#define	CMD_UNLOCK_USB		0x23			/* Device Mode: release buffer */

#define	CMD_RD_USB_DATA		0x28			/* block read from usb device, and release buffer */
	/* Output: length, data stream */

#define	CMD_WR_USB_DATA3	0x29			/* Device Mode: write to usb endpoint 0 */
	/* Input: length, data stream */

#define	CMD_WR_USB_DATA5	0x2A			/* Device Mode: write to usb endpoint 1 */
	/* Input: length, data stream */

#define	CMD_WR_USB_DATA7	0x2B			/* write to usb endpoint 2 */
	/* Input: length, data stream */

	/* ************************************************************************** */
	/* Host Mode, only for CH375 */

#define	CMD_SET_BAUDRATE	0x02			/* Serial Mode: setup serial baudrate */
	/* Input: baudrate pll divider */
	/* Output: State( CMD_RET_SUCCESS or CMD_RET_ABORT, otherswise unfinished ) */

#define	CMD_ABORT_NAK		0x17			/* Host Mode: abort nak retry */

#define	CMD_SET_RETRY		0x0B			/* Host Mode: setup usb token retry times */
	/* Input: 0x25, setup retry times */
	/*					  bit7=1 for infinite retry, bit3~0 retry times*/

#define	CMD_ISSUE_TOKEN		0x4F			/* Host Mode: issue the token */
	/* Input: token */
	/*			 bit3~0:token type, bit7~4:endpoint */
	/* Output interrupt state */

#define	CMD_CLR_STALL		0x41			/* Host Mode: clear the endpoint Error */
	/* Input: endpoint index */
	/* Output interrupt state */

#define	CMD_SET_ADDRESS		0x45			/* Host Mode: Control-setup usb address */
	/* Input: address */
	/* Output interrupt state */

#define	CMD_GET_DESCR		0x46			/* Host Mode: Control-get descriptor */
	/* Input: descriptor type 0:config 1:device */
	/* Output interrupt state */

#define	CMD_SET_CONFIG		0x49			/* Host Mode: Control-config and enable the device */
	/* Input: config */
	/* Output interrupt state */

#define	CMD_DISK_INIT		0x51			/* Host Mode: init usb storage device */
	/* Output interrupt state */

#define	CMD_DISK_RESET		0x52			/* Host Mode: reset usb storage device */
	/* Output interrupt state */

#define	CMD_DISK_SIZE		0x53			/* Host Mode: get the capacity of usb storage device */
	/* Output interrupt state */

#define	CMD_DISK_READ		0x54			/* Host Mode: read from usb storage device(512byte) */
	/* Input: LBA sector address(32bit, LSB), Sector(01H~FFH) */
	/* Output interrupt state */

#define	CMD_DISK_RD_GO		0x55			/* Host Mode: continus read */
	/* Output interrupt state */

#define	CMD_DISK_WRITE		0x56			/* Host Mode: write to usb storage device(512byte) */
	/* Input: LBA sector address(32bit, LSB), Sector(01H~FFH) */
	/* Output interrupt state */

#define	CMD_DISK_WR_GO		0x57			/* Host Mode: continus write */
	/* Output interrupt state */

	/* ************************************************************************** */
	/* only for CH372A/CH375A */

#define	CMD_GET_IC_VER		0x01			/* get chip version */
	/* Output: Version( bit7:1, bit6:0, bit5~0:Version ) */
	/*			 CH375:5FH(invalid), CH375A:0A2H */

#define	CMD_ENTER_SLEEP		0x03			/* into sleep mode */

#define	CMD_RD_USB_DATA0	0x27			/* read from usb endpoint */
	/* Output: length, data stream */

#define	CMD_DELAY_100US		0x0F			/* Parallel : delay 100uS */
	/* Output: 0 during delay, none zero after delay */

#define	CMD_CHK_SUSPEND		0x0B			/* Device Mode: check usb bus hung */
	/* Input: 10H, TYPE */
	/*		   TYPE:00H=don't check usb hung, 04H=check hung every 50ms, 05H=check hung every 10ms */

#define	CMD_SET_SYS_FREQ	0x04			/* setup system working frequency */
	/* Input: frequency */
	/*			 00H=12MHz, 01H=1.5MHz */

	/* ************************************************************************** */
	/*	only for CH375A */

#define	CMD_TEST_CONNECT	0x16			/* Host Mode: check usb device connection */
	/* Output: state( USB_INT_CONNECT of USB_INT_DISCONNECT, otherswise unfinished ) */

#define	CMD_AUTO_SETUP		0x4D			/* Host Mode: auto setup usb device */
	/* Output interrupt state */

#define	CMD_ISSUE_TKN_X		0x4E			/* Host Mode: issue sync token */
	/* Input: sync flag, token */
	/*			 sync flag: bit7:IN endpoint sync flag, OUT endpoint sync flag, bit5~0:should be zero */
	/*			 token:bit3~0:token type, bit7~4:endpoint */
	/* Output interrupt state */


	/* ********************************************************************************************************************* */
	/* operate mode */

#define	CMD_RET_SUCCESS		0x51			/* success */
#define	CMD_RET_ABORT		0x5F			/* fail */

	/* ********************************************************************************************************************* */
	/* USB interrupt state */

	/* special interrupt state, only for CH372A/CH375A */
#define	USB_INT_USB_SUSPEND	0x05			/* usb bus susspend */
#define	USB_INT_WAKE_UP		0x06			/* wake up */

	/* 0XH for USBDevice Mode */
#define	USB_INT_EP0_SETUP	0x0C			/* USB ep0 SETUP */
#define	USB_INT_EP0_OUT		0x00			/* USB ep0 OUT */
#define	USB_INT_EP0_IN		0x08			/* USB ep0 IN */
#define	USB_INT_EP1_OUT		0x01			/* USB ep1 OUT */
#define	USB_INT_EP1_IN		0x09			/* USB ep1 IN */
#define	USB_INT_EP2_OUT		0x02			/* USB ep2 OUT */
#define	USB_INT_EP2_IN		0x0A			/* USB ep2 IN */
	/* USB_INT_BUS_RESET	0x0000XX11B */		/* USB BUS RESET */
#define	USB_INT_BUS_RESET1	0x03			/* USB BUS RESET */
#define	USB_INT_BUS_RESET2	0x07			/* USB BUS RESET */
#define	USB_INT_BUS_RESET3	0x0B			/* USB BUS RESET */
#define	USB_INT_BUS_RESET4	0x0F			/* USB BUS RESET */

	/* 2XH-3XH fail state response, only for CH375/CH375A */
	/*	 bit7~6 00 */
	/*	 bit5 1 */
	/*	 bit4 if package sync */
	/*	 bit3~0 fail response from usb device: 0010=ACK, 1010=NAK, 1110=STALL, 0011=DATA0, 1011=DATA1, XX00=TimeOut */
	/* USB_INT_RET_ACK	0x001X0010B */			/* Error:Ack to IN token */
	/* USB_INT_RET_NAK	0x001X1010B */			/* Error:NAK */
	/* USB_INT_RET_STALL	0x001X1110B */		/* Error:STALL */
	/* USB_INT_RET_DATA0	0x001X0011B */		/* Error:DATA0 to OUT/SETUP*/
	/* USB_INT_RET_DATA1	0x001X1011B */		/* Error:DATA1 to OUT/SETUP */
	/* USB_INT_RET_TOUT 0x001XXX00B */			/* Error:TimeOut */
	/* USB_INT_RET_TOGX 0x0010X011B */			/* Error:Unsync to IN */
	/* USB_INT_RET_PID	0x001XXXXXB */			/* Error:refer to PID */

	/* 0x1X control state, only for CH375/CH375A */

#define	USB_INT_SUCCESS		0x14			/* token successful sent */
#define	USB_INT_CONNECT		0x15			/* detect usb device plug in */
#define	USB_INT_DISCONNECT	0x16			/* detect usb device plug out */
#define	USB_INT_BUF_OVER	0x17			/* USB control buffer overflow */
#define	USB_INT_DISK_READ	0x1D			/* USB storage, read request */
#define	USB_INT_DISK_WRITE	0x1E			/* USB storage, write request */
#define	USB_INT_DISK_ERR	0x1F			/* USB storage, fail */

	/* ********************************************************************************************************************* */
	/* USB package identifer PID, for host mode */
#define	DEF_USB_PID_NULL	0x00			/* reserve PID, not defined */
#define	DEF_USB_PID_SOF		0x05
#define	DEF_USB_PID_SETUP	0x0D
#define	DEF_USB_PID_IN		0x09
#define	DEF_USB_PID_OUT		0x01
#define	DEF_USB_PID_ACK		0x02
#define	DEF_USB_PID_NAK		0x0A
#define	DEF_USB_PID_STALL	0x0E
#define	DEF_USB_PID_DATA0	0x03
#define	DEF_USB_PID_DATA1	0x0B
#define	DEF_USB_PID_PRE		0x0C

	/* USB request type, for external firmware mode */
#define	DEF_USB_REQ_READ	0x80			/* control read request */
#define	DEF_USB_REQ_WRITE	0x00			/* control write request */
#define	DEF_USB_REQ_TYPE	0x60			/* control type request */
#define	DEF_USB_REQ_STAND	0x00			/* standard request */
#define	DEF_USB_REQ_CLASS	0x20			/* device class request */
#define	DEF_USB_REQ_VENDOR	0x40			/* vendor request */
#define	DEF_USB_REQ_RESERVE	0x60			/* reserved request */

	/* USB standard device request, RequestType bit6~5=00(Standard), for external firmware mode */
#define	DEF_USB_CLR_FEATURE	0x01
#define	DEF_USB_SET_FEATURE	0x03
#define	DEF_USB_GET_STATUS	0x00
#define	DEF_USB_SET_ADDRESS	0x05
#define	DEF_USB_GET_DESCR	0x06
#define	DEF_USB_SET_DESCR	0x07
#define	DEF_USB_GET_CONFIG	0x08
#define	DEF_USB_SET_CONFIG	0x09
#define	DEF_USB_GET_INTERF	0x0A
#define	DEF_USB_SET_INTERF	0x0B
#define	DEF_USB_SYNC_FRAME	0x0C

//Define struct
/* ********************************************************************************************************************* */
typedef struct _USB_DEVICE_DEscriptOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    unsigned short bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    unsigned short idVendor;
    unsigned short idProduct;
    unsigned short bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} USB_DEV_DESCR, *PUSB_DEV_DESCR;

typedef struct _USB_CONFIG_DEscriptOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    unsigned short wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationvalue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t MaxPower;
} USB_CFG_DESCR, *PUSB_CFG_DESCR;

typedef struct _USB_INTERF_DEscriptOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} USB_ITF_DESCR, *PUSB_ITF_DESCR;

typedef struct _USB_ENDPOINT_DEscriptOR {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    unsigned short wMaxPacketSize;
    uint8_t bInterval;
} USB_ENDP_DESCR, *PUSB_ENDP_DESCR;

typedef struct _USB_CONFIG_DEscriptOR_LONG {
    USB_CFG_DESCR cfg_descr;
    USB_ITF_DESCR itf_descr;
    USB_ENDP_DESCR endp_descr[4];
} USB_CFG_DESCR_LONG, *PUSB_CFG_DESCR_LONG;

/**
 * Class: MeUSBHost
 * \par Description
 * Declaration of Class MeUSBHost.
 */
class MeUSBHost : public MePort
{
public:
  bool ch375_online;
  bool device_online;
  bool device_ready;
  uint8_t RECV_BUFFER[ CH375_MAX_DATA_LEN ];

/**
 * Alternate Constructor which can call your own function to map the USB Host to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
  MeUSBHost();

  MeUSBHost(uint8_t s1, uint8_t s2);

/**
 *  Alternate Constructor which can call your own function to map the USB Host to arduino port, \n
 *  the slot2 pin will be used for key pin.
 * \param[in]
 *    port - RJ25 port from PORT_1 to M2
 */
  MeUSBHost(uint8_t port);

/**
 * \par Function
 *    init
 * \par Description
 *    Init the data Bus.
 * \param[in]
 *    type - The type of data Bus.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void init(int8_t type);

/**
 * \par Function
 *    initHIDDevice
 * \par Description
 *    Init the HID Device.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    The result of initHIDDevice's action.
 * \par Others
 *    None
 */
  int16_t initHIDDevice();

/**
 * \par Function
 *    probeDevice
 * \par Description
 *    Prode of USB Host Device.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    The result of device's probe.
 * \par Others
 *    None
 */
  int16_t probeDevice();

/**
 * \par Function
 *    resetBus
 * \par Description
 *    Reset the data Bus.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void resetBus();

/**
 * \par Function
 *    host_recv
 * \par Description
 *    The USB Host receive data.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    Return the length of data.
 * \par Others
 *    None
 */
  uint8_t host_recv();

private:
  MeSerial *HSerial;
  int16_t stallCount;
  int8_t usbtype;

/**
 * \par Function
 *    CH375_RD
 * \par Description
 *    Read data from USB.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    Return 0;
 * \par Others
 *    None
 */
  uint8_t CH375_RD();

/**
 * \par Function
 *    CH375_WR
 * \par Description
 *    Write data to USB device.
 * \param[in]
 *    c - The bytes that wrote to device. 
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void CH375_WR(uint8_t c);

/**
 * \par Function
 *    set_usb_mode
 * \par Description
 *    Set the work mode of USB.
 * \param[in]
 *    mode - The USB's work mode. 
 * \par Output
 *    None
 * \return
 *    Return the data that CH375_RD()'s return.
 * \par Others
 *    None
 */
  int16_t set_usb_mode(int16_t mode);

/**
 * \par Function
 *    getIrq
 * \par Description
 *    Get the Interrupt Request of USB.
 * \param[in]
 *    None 
 * \par Output
 *    None
 * \return
 *    Return the data that CH375_RD()'s return.
 * \par Others
 *    None
 */
  uint8_t getIrq();

/**
 * \par Function
 *    toggle_send
 * \par Description
 *    The toggle used to send data.
 * \param[in]
 *    None 
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void toggle_send();

/**
 * \par Function
 *    toggle_recv
 * \par Description
 *    The toggle used to receive data.
 * \param[in]
 *    None 
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void toggle_recv();

/**
 * \par Function
 *    issue_token
 * \par Description
 *    USB Host make a token and perform transactions.
 * \param[in]
 *    endp_and_pid - The token that USB Host used.
 * \par Output
 *    None
 * \return
 *    Return the Interrupt Request.
 * \par Others
 *    None
 */
  uint8_t issue_token( uint8_t endp_and_pid );

/**
 * \par Function
 *    wr_usb_data
 * \par Description
 *    Write data to USB Host.
 * \param[in]
 *    len - The data's length.
  * \param[in]
 *    buf - Data in buffer.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void wr_usb_data( uint8_t len, uint8_t *buf );

/**
 * \par Function
 *    rd_usb_data
 * \par Description
 *    Read data from USB Host.
 * \param[in]
 *    len - The data's length.
 * \param[in]
 *    buf - Data in buffer.
 * \par Output
 *    None
 * \return
 *    Return the length of read data.
 * \par Others
 *    None
 */
  uint8_t rd_usb_data( uint8_t *buf );

/**
 * \par Function
 *    get_version
 * \par Description
 *    Get version of USB Host.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    Return the data that CH375_RD()'s return.
 * \par Others
 *    None
 */
  int16_t get_version();

/**
 * \par Function
 *    set_freq
 * \par Description
 *    Set frequency of USB Host.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
  void set_freq(void);

/**
 * \par Function
 *    set_addr
 * \par Description
 *    Set address of USB Host.
 * \param[in]
 *    addr - The address of USB Host.
 * \par Output
 *    None
 * \return
 *    Return the number of Interrupt Request.
 * \par Others
 *    None
 */
  uint8_t set_addr( uint8_t addr );

/**
 * \par Function
 *    set_config
 * \par Description
 *    Set config of USB Host.
 * \param[in]
 *    cfg - The config file of USB Host.
 * \par Output
 *    None
 * \return
 *    Return the number of Interrupt Request.
 * \par Others
 *    None
 */
  uint8_t set_config(uint8_t cfg);

/**
 * \par Function
 *    clr_stall6
 * \par Description
 *    Clear all stall in USB Host.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \return
 *    Return the number of Interrupt Request.
 * \par Others
 *    None
 */
  uint8_t clr_stall6(void);

/**
 * \par Function
 *    get_desr
 * \par Description
 *    Get description of USB Host.
 * \param[in]
 *    type - The type of description.
 * \par Output
 *    None
 * \return
 *    Return the number of Interrupt Request.
 * \par Others
 *    None
 */
  uint8_t get_desr(uint8_t type);
};
#endif

