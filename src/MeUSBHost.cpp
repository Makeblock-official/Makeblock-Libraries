/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeUSBHost
 * \brief   Driver for Me USB Host module.
 * @file    MeUSBHost.cpp
 * @author  MakeBlock
 * @version V1.0.1
 * @date    2016/01/20
 * @brief   Driver for Me USB Host module.
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
 * This file is a drive for Me Bluetooth device, The bluetooth inherited the 
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

#include "MeUSBHost.h"

//#define CH375_DBG

#define p_dev_descr ((PUSB_DEV_DESCR)RECV_BUFFER)
#define p_cfg_descr ((PUSB_CFG_DESCR_LONG)RECV_BUFFER)

uint8_t endp_out_addr;
uint8_t endp_out_size;
uint8_t endp_in_addr;
uint8_t endp6_mode, endp7_mode;

uint8_t *cmd_buf;
uint8_t *ret_buf;
PUSB_ENDP_DESCR tmpEp;

/**
 * Alternate Constructor which can call your own function to map the USB Host to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
 */
MeUSBHost::MeUSBHost() : MePort(0)
{

}

MeUSBHost::MeUSBHost(uint8_t s1, uint8_t s2)
{
  HSerial = new MeSerial(s1,s2);
}

/**
 *  Alternate Constructor which can call your own function to map the USB Host to arduino port, \n
 *  the slot2 pin will be used for key pin.
 * \param[in]
 *    port - RJ25 port from PORT_1 to M2
 */
MeUSBHost::MeUSBHost(uint8_t port) : MePort(port)
{
  HSerial = new MeSerial(port);
}

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
uint8_t MeUSBHost::CH375_RD()
{
  delay(2); // stupid delay, the chip don't got any buffer
  if(HSerial->available()){
    uint8_t c = HSerial->read();
#ifdef CH375_DBG
    Serial.print("<< 0x");
    Serial.print(c,HEX);
    Serial.println();
#endif
    return c;
  }
  return 0;
}

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
void MeUSBHost::CH375_WR(uint8_t c)
{
  HSerial->write(c);
  delay(2);
#ifdef CH375_DBG
  Serial.print(">> 0x");
  Serial.print(c,HEX);
  Serial.println();
#endif
}

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
int16_t MeUSBHost::set_usb_mode(int16_t mode)
{
  CH375_WR(CMD_SET_USB_MODE);
  CH375_WR(mode);
  endp6_mode=endp7_mode=0x80;
  return CH375_RD();
}

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
uint8_t MeUSBHost::getIrq()
{
  CH375_WR(CMD_GET_STATUS);
  delay(20);
  return CH375_RD();
}

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
void MeUSBHost::toggle_send()
{
#ifdef CH375_DBG
  Serial.print("toggle send: 0x");
  Serial.print(endp7_mode,HEX);
  Serial.println();
#endif
  CH375_WR(CMD_SET_ENDP7);
  CH375_WR( endp7_mode );
  endp7_mode^=0x40;
}

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
void MeUSBHost::toggle_recv()
{
  CH375_WR( CMD_SET_ENDP6 );
  CH375_WR( endp6_mode );
#ifdef CH375_DBG
  Serial.print("toggle recv: 0x");
  Serial.print(endp6_mode,HEX);
  Serial.println();
#endif
  endp6_mode^=0x40;
}

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
uint8_t MeUSBHost::issue_token( uint8_t endp_and_pid )
{
  CH375_WR( CMD_ISSUE_TOKEN );
  CH375_WR( endp_and_pid );  /* Bit7~4 for EndPoint No, Bit3~0 for Token PID */
#ifdef CH375_DBG
  Serial.print("issue token: 0x");
  Serial.print(endp_and_pid,HEX);
  Serial.println();
#endif
  delay(2);
  return getIrq();
}

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
void MeUSBHost::wr_usb_data( uint8_t len, uint8_t *buf )
{
#ifdef CH375_DBG
  Serial.print("usb wr: ");
  Serial.print(len,DEC);
  Serial.println();
#endif
  CH375_WR( CMD_WR_USB_DATA7 );
  CH375_WR( len );
  while( len-- ){
    CH375_WR( *buf++ );
  }
}

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
uint8_t MeUSBHost::rd_usb_data( uint8_t *buf )
{
  uint8_t i, len;
  CH375_WR( CMD_RD_USB_DATA );
  len=CH375_RD();
#ifdef CH375_DBG
  Serial.print("usb rd: ");
  Serial.print(len,DEC);
  Serial.println();
#endif
  for ( i=0; i!=len; i++ ) *buf++=CH375_RD();
  return( len );
}

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
int16_t MeUSBHost::get_version(){
  CH375_WR(CMD_GET_IC_VER);
  return CH375_RD();
}

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
void MeUSBHost::set_freq(void)
{
  CH375_WR(0x0b);
  CH375_WR(0x17);
  CH375_WR(0xd8);
}

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
uint8_t MeUSBHost::set_addr( uint8_t addr )
{
  uint8_t irq;
  CH375_WR(CMD_SET_ADDRESS);
  CH375_WR(addr);
  irq = getIrq();
  if(irq==USB_INT_SUCCESS){
    CH375_WR(CMD_SET_USB_ADDR);
    CH375_WR(addr);
  }
  return irq;
}

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
uint8_t MeUSBHost::set_config(uint8_t cfg){
  endp6_mode=endp7_mode=0x80; // reset the sync flags
  CH375_WR(CMD_SET_CONFIG);
  CH375_WR(cfg);
  return getIrq();
}

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
uint8_t MeUSBHost::clr_stall6(void)
{
  CH375_WR( CMD_CLR_STALL );
  CH375_WR( endp_out_addr | 0x80 );
  endp6_mode=0x80;
  return getIrq();
}

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
uint8_t MeUSBHost::get_desr(uint8_t type)
{
  CH375_WR( CMD_GET_DESCR );
  CH375_WR( type );   /* description type, only 1(device) or 2(config) */
  return getIrq();
}

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
uint8_t MeUSBHost::host_recv()
{
  uint8_t irq;
  toggle_recv();
  irq = issue_token( ( endp_in_addr << 4 ) | DEF_USB_PID_IN );
  if(irq==USB_INT_SUCCESS){
    int16_t len = rd_usb_data(RECV_BUFFER);
#ifdef CH375_DBG
    for(int16_t i=0;i<len;i++){
      // point hid device
      Serial.print(" 0x");
      Serial.print((int16_t)RECV_BUFFER[i],HEX);
    }
    Serial.println();
#endif
    stallCount = 0;
    return len;
  }else if(irq==USB_INT_DISCONNECT){
    device_online = false;
    device_ready = false;
#ifdef CH375_DBG
    Serial.println("##### disconn #####");
#endif
    return 0;
  }else{
    clr_stall6();
#ifdef CH375_DBG
    Serial.println("##### stall #####");
#endif
    delay(10);
    /*
    stallCount++;
    if(stallCount>10){
      device_online = false;
      device_ready = false;
      resetBus();
    }
    */
    return 0;
  }
}

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
void MeUSBHost::resetBus()
{
  int16_t c;
  c = set_usb_mode(7);
#ifdef CH375_DBG
  Serial.print("set mode 7: 0x");
  Serial.print(c,HEX);
  Serial.println();
#endif
  delay(10);
  c = set_usb_mode(6);
#ifdef CH375_DBG
  Serial.print("set mode 6: 0x");
  Serial.print(c,HEX);
  Serial.println();
#endif
  delay(10);
}

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
void MeUSBHost::init(int8_t type)
{
  ch375_online = false;
  device_online = false;
  device_ready = false;
  usbtype = type;
  if(HSerial == NULL)
  {
    HSerial = new MeSerial(s2, s1);
  }
  
  HSerial->begin(9600);
}


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
int16_t MeUSBHost::initHIDDevice()
{
  int16_t irq, len, address;
  if(usbtype==USB1_0) set_freq(); //work on a lower freq, necessary for ch375
  irq = get_desr(1);
#ifdef CH375_DBG
  Serial.print("get des irq: 0x");
  Serial.print(irq,HEX);
  Serial.println();
#endif
  if(irq==USB_INT_SUCCESS){
      len = rd_usb_data( RECV_BUFFER );
#ifdef CH375_DBG
      Serial.print("descr1 len: ");
      Serial.print(len,DEC);
      Serial.print(" type: 0x");
      Serial.print(p_dev_descr->bDescriptorType,HEX);
      Serial.println();
#endif
      irq = set_addr(2);
      if(irq==USB_INT_SUCCESS){
        irq = get_desr(2); // max buf 64byte, todo:config descr overflow
        if(irq==USB_INT_SUCCESS){
          len = rd_usb_data( RECV_BUFFER );
#ifdef CH375_DBG
          Serial.print("descr1 len: ");
          Serial.print(len,DEC);
          Serial.print(" class: 0x");
          Serial.print(p_cfg_descr->itf_descr.bInterfaceClass,HEX);
          Serial.print(" subclass: 0x");
          Serial.print(p_cfg_descr->itf_descr.bInterfaceSubClass,HEX);
          Serial.println();

          Serial.print("num of ep: ");
          Serial.print(p_cfg_descr->itf_descr.bNumEndpoints,DEC);
          Serial.println();

          Serial.print("ep0: 0x");
          Serial.print(p_cfg_descr->endp_descr[0].bLength,HEX);
          Serial.print(" 0x");
          Serial.print(p_cfg_descr->endp_descr[0].bDescriptorType,HEX);
          Serial.println();
#endif
          if(p_cfg_descr->endp_descr[0].bDescriptorType==0x21){ // skip hid des
            tmpEp = (PUSB_ENDP_DESCR)((int8_t*)(&(p_cfg_descr->endp_descr[0]))+p_cfg_descr->endp_descr[0].bLength); // get the real ep position
          }
#ifdef CH375_DBG
          Serial.print("endpoint: 0x");
          Serial.print(tmpEp->bEndpointAddress,HEX);
          Serial.print(" 0x");
          Serial.print(tmpEp->bDescriptorType,HEX);
          Serial.println();
#endif
          endp_out_addr=endp_in_addr=0;
          address =tmpEp->bEndpointAddress;  /* Address of First EndPoint */
          // actually we only care about the input end points
          if( address&0x80 ){
            endp_in_addr = address&0x0f;  /* Address of IN EndPoint */
          }else{  /* OUT EndPoint */
            endp_out_addr = address&0x0f;
            endp_out_size = p_cfg_descr->endp_descr[0].wMaxPacketSize;
			/* Length of Package for Received Data EndPoint */
            if( endp_out_size == 0 || endp_out_size > 64 )
              endp_out_size = 64;
          }
          // todo: some joystick with more than 2 node
          // just assume every thing is fine, bring the device up
          irq = set_config(p_cfg_descr->cfg_descr.bConfigurationvalue);
          if(irq==USB_INT_SUCCESS){
            CH375_WR( CMD_SET_RETRY );  // set the retry times
            CH375_WR( 0x25 );
            CH375_WR( 0x85 );
            device_ready = true;
            return 1;
          }
        }

      }
  }
  return 0;
}

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
int16_t MeUSBHost::probeDevice()
{
  int16_t c;
  if(!ch375_online){
    CH375_WR( CMD_CHECK_EXIST );
    CH375_WR( 0x5A);
    c = CH375_RD(); // should return 0xA5
    if(c!=0xA5) return 0;
    ch375_online = true;
    resetBus();
  }

  c = getIrq();
  if(c!=USB_INT_CONNECT) return 0;
  resetBus(); // reset bus and wait the device online again
  c=0;
  while(c!=USB_INT_CONNECT){
    delay(500); // some device may need long time to get ready
    c = getIrq();
#ifdef CH375_DBG
    Serial.print("waiting:");
    Serial.println(c,HEX);
#endif
  }
  if( initHIDDevice()==1)
    device_online=true;
}


