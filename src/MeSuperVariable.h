/**
 * \par Copyright (C), 2012-2018, MakeBlock
 * \class MeSuperVariable
 * \brief   Driver for super var communicate.
 * @file    MeSuperVariable.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2018/03/6
 * @brief   Header for MeSuperVariable.cpp module
 *
 * \par Copyright
 * This software is Copyright (C), 2012-2018, MakeBlock. Use is subject to license \n
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
 * This file is the drive for super var communicate, It supports
 *      mBlock5.0 provided by MakeBlock.
 * \par Method List:
 *
 *    1. void comm_protocol_pump_char(uint8_t c, uint8_t *output_buf, uint32_t *output_len);
 *    2. void comm_build_frame(uint8_t protocol_id, uint8_t *inoutput_data, uint32_t *inoutput_len);
 *    3. void arduino_super_var_init(void);
 *    4. void arduino_super_var(uint8_t *data, uint32_t len, uint8_t *output_buf, uint32_t *output_len);
 *    5. void *set_super_value(char *name,uint8_t type,void *data, uint8_t len);
 *    6. bool get_super_value(char *name,void *data);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * lanweiting         2018/03/06        1.0.0            build the new files.
 * </pre>
 */
#ifndef MESUPERVARIABLE_H
#define MESUPERVARIABLE_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <Arduino.h>

/* Communication CMD */
#define CHAR_TYPE             0x01
#define UNCHAR_TYPE           0x02
#define SHORT_TYPE            0x03
#define UNSHORT_TYPE          0x04
#define INT_TYPE              0x05
#define UNINT_TYPE            0x06
#define LONG_LONG_INT_TYPE    0x07
#define UNLONG_LONG_INT_TYPE  0x08
#define FLOAT_TYPE            0x09
#define DOUBLE_TYPE           0x0a
#define BOOL_TYPE             0x0b
#define STRING_TYPE           0x0c

#define   FRAME_HEADER                  (0xF3)
#define   FRAME_END                     (0xF4)
#define   TX_BUF_SIZE                   (256)
#define   FRAME_DATA_MAX_LEN            (256 + 128)
//#define   MAX_NODE                      (16)
//#define   NAME_MAX_LEN                  (32)
#define   VALUE_MAX_LEN                 (32)

#if defined(__AVR_ATmega328P__)
  #define   MAX_NODE                      (3)
  #define   NAME_MAX_LEN                  (26)
#elif defined(__AVR_ATmega2560__)
  #define   MAX_NODE                      (16)
  #define   NAME_MAX_LEN                  (32)
#else
  
#endif


/******************************************************************************
 DEFINE COMM ID
 ******************************************************************************/
typedef enum
{
  NEU_FTP_ID = 0x01,
  SUPER_VAR_ID,
  MSG_BCAST_ID,
  SENSOR_REPORT_ID,
  GET_MAC_ID,
  GET_FIRMWARE,
  DTR_REQ,
  READY_NOTIFY,
  GET_CHN,
  PRODUCT_TEST,
  PROTOCOL_NUM,
} protocol_id_t;
/******************************************************************************
 DEFINE TYPES & CONSTANTS
 ******************************************************************************/
typedef enum
{
  SUPER_VAR_NONE_T = 0,
  SUPER_VAR_CHAR_T,
  SUPER_VAR_UN_CHAR_T,
  SUPER_VAR_SHORT_T,
  SUPER_VAR_UN_SHORT_T,
  SUPER_VAR_INT_T,
  SUPER_VAR_UN_INT_T,
  SUPER_VAR_LL_T,
  SUPER_VAR_UN_LL_T,
  SUPER_VAR_FLOAT_T,
  SUPER_VAR_DOUBLE_T,
  SUPER_VAR_BOOL_T,
  SUPER_VAR_STR_T,
  SUPER_VAR_LIST_T,
  
  SUPER_VAR_NUM,
}super_var_type_t;

enum
{
  HEAD_S = 0,
  LEN_S,
  DATA_S,
  CHECK_S,
  END_S,
};

union
{
  uint8_t byteVal[4];
  float floatVal;
  int32_t int32Val;
}super_var_val;

typedef enum
{
  S_HEAD = 0,
  S_HEAD_CHECK,
  S_LEN1,
  S_LEN2,
  S_DATA,
  S_CHECK,
  S_END,
} fsm_state_t;

typedef struct
{
  fsm_state_t   s;
  uint8_t       recv_head_check;
  uint8_t       calc_head_check;
  uint16_t      len;
  uint16_t      recv_len;
  uint8_t       check;
  uint8_t       data[FRAME_DATA_MAX_LEN];
} fsm_t;

typedef struct
{
  bool used;
  char name[NAME_MAX_LEN];
  super_var_type_t type;
  uint8_t len;
  uint8_t value[VALUE_MAX_LEN];
}super_var_node_t;

typedef struct
{
  super_var_type_t  type;
  uint8_t           size;
  char const*       desc;
}super_var_type_desc_t;

const super_var_type_desc_t super_var_type_desc_tab[] = 
{
  { SUPER_VAR_CHAR_T,         1,        "char" },
  { SUPER_VAR_UN_CHAR_T,      1,        "un char" },
  { SUPER_VAR_SHORT_T,        2,        "short" },
  { SUPER_VAR_UN_SHORT_T,     2,        "un short" },
  { SUPER_VAR_INT_T,          4,        "int" },
  { SUPER_VAR_UN_INT_T,       4,        "un int" },  
  { SUPER_VAR_LL_T,           8,        "long long" }, 
  { SUPER_VAR_UN_LL_T,        8,        "un long long" }, 
  { SUPER_VAR_FLOAT_T,        4,        "float" }, 
  { SUPER_VAR_DOUBLE_T,       8,        "double" }, 
  { SUPER_VAR_BOOL_T,         1,        "bool" }, 
  { SUPER_VAR_STR_T,          0,        "string" }
};


/**
 * Class: MeSuperVariable
 * \par Description
 * Declaration of Class MeSuperVariable.
 */
class MeSuperVariable
{
public:
  /**
   * initialized here.
   * \param[in]
   *   None
   */
  MeSuperVariable();
  
  /**
   * Finalize here.
   * \param[in]
   *   None
   */
  ~MeSuperVariable();
  
  /**
   * \par Function
   *    comm_protocol_pump_char
   * \par Description
   *    Read the serial data.Parse the data frame format.
   * \param[in]
   *    c - char from serial.read()
   *    output_buf - output data array pointer
   *    output_len - output data length pointer
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void comm_protocol_pump_char(uint8_t c, uint8_t *output_buf, uint32_t *output_len);
  
  /**
   * \par Function
   *    comm_build_frame
   * \par Description
   *    build a frame for send buffer.
   * \param[in]
   *    protocol_id - char from serial.read()
   *    inoutput_data - inoutput data array pointer
   *    inoutput_len - inoutput data length pointer
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void comm_build_frame(uint8_t protocol_id, uint8_t *inoutput_data, uint32_t *inoutput_len);
  
  /**
   * \par Function
   *    arduino_super_var_init
   * \par Description
   *    Initializes the super var array of structures..
   * \param[in]
   *    None
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void arduino_super_var_init(void);
  
  /**
   * \par Function
   *    arduino_super_var
   * \par Description
   *    New or updated super var structure array.
   * \param[in]
   *    data - input data pointer
   *    len - data length
   *    output_buf - output_buf data pointer
   *    output_len - output data len
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void arduino_super_var(uint8_t *data, uint32_t len, uint8_t *output_buf, uint32_t *output_len);
  
  /**
   * \par Function
   *    set_variable
   * \par Description
   *    Set or updata the super var for button module.
   * \param[in]
   *    name - super var name pointer.
   *    type - super var data type.
   *    data - super var data pointer.
   *    len - super var data length.
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void *set_variable(char *name,uint8_t type,void *data, uint8_t len);
  
  /**
   * \par Function
   *    get_super_value
   * \par Description
   *    Get the super var value from  structure array.
   * \param[in]
   *    name - super_var name pointer.
   *    data - receive data pointer address.
   * \par Output
   *    None
   * \return
   *    return data pointer.
   * \par Others
   *    None
   */
  bool get_super_value(char *name,void *data);
    /**
   * \par Function
   *    get_variable
   * \par Description
   *    Get the float var value from  structure array.
   * \param[in]
   *    name - super_var name pointer.
   * \par Output
   *    None
   * \return
   *    return data pointer.
   * \par Others
   *    None
   */
  float get_variable(char *name);
private:
  /**
   * \par Function
   *    calc_add_check_sum
   * \par Description
   *    Calculate data add check sum.
   * \param[in]
   *    data - data array pointer.
   *    len - data length.
   * \par Output
   *    None
   * \return
   *    return add check sum result.data type is uint8_t
   * \par Others
   *    None
   */
  uint8_t calc_add_check_sum(uint8_t *data, uint32_t len);
  
  /**
   * \par Function
   *    send_data
   * \par Description
   *    Send the data array to Serial COM.
   * \param[in]
   *    data - data array pointer.
   *    len - data length.
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void send_data(uint8_t *senddata, uint8_t len);
  
  /**
   * \par Function
   *    char_to_string
   * \par Description
   *    char array transform to String type.
   * \param[in]
   *    data - data array pointer.
   *    len - data length.
   * \par Output
   *    None
   * \return
   *    return String pointer.
   * \par Others
   *    None
   */
  void char_to_string(char *char_data, String *str,uint8_t len);
  
  /**
   * \par Function
   *    super_var_frame_parse
   * \par Description
   *    super variable frame parse.
   * \param[in]
   *    data - data array pointer.
   *    len - data length.
   * \par Output
   *    None
   * \return
   *    None
   * \par Others
   *    None
   */
  void super_var_frame_parse(uint8_t *data, uint8_t len);
  
  /**
  * \par Function
  *    super_var_build_data
  * \par Description
  *    super variable send value.
  * \param[in]
  *    super_var - super pointer.
  *    buf - buf pointer.
  *    out_len - length.
  * \par Output
  *    None
  * \return
  *    None
  * \par Others
  *    None
  */
  void super_var_build_data(super_var_node_t *super_var, uint8_t *buf, uint32_t *out_len);

  /**
  * \par Function
  *    super_var_build_data
  * \par Description
  *    super variable send value.
  * \param[in]
  *    super_var - super pointer.
  *    buf - buf pointer.
  *    out_len - length.
  * \par Output
  *    None
  * \return
  *    None
  * \par Others
  *    None
  */
  void super_var_send_value(super_var_node_t *super_var);

  /**
  * \par Function
  *    node_value_update
  * \par Description
  *    node value update.
  * \param[in]
  *    super_var - node pointer.
  *    val_type - type.
  *    val_data - value of data.
  *    value_len - length.
  * \par Output
  *    None
  * \return
  *    None
  * \par Others
  *    None
  */
  void node_value_update(super_var_node_t *super_var, super_var_type_t val_type, const uint8_t *val_data, uint8_t value_len);

  /**
  * \par Function
  *    malloc_a_node
  * \par Description
  *    malloc a node.
  * \param[in]
  *    none
  * \par Output
  *    None
  * \return
  *    super node pointer.
  * \par Others
  *    None
  */
  super_var_node_t *malloc_a_node(void);

  /**
  * \par Function
  *    super_var_create
  * \par Description
  *    super variable create.
  * \param[in]
  *    name - name pointer.
  * \par Output
  *    None
  * \return
  *    node pointer.
  * \par Others
  *    None
  */
  super_var_node_t *super_var_create(const char *name);

  /**
  * \par Function
  *    super_var_find_by_name
  * \par Description
  *    super variable find by name.
  * \param[in]
  *    none
  * \par Output
  *    None
  * \return
  *    super node pointer.
  * \par Others
  *    None
  */
  super_var_node_t *super_var_find_by_name(const char *name);

  fsm_t s_fsm_tab[1];
  super_var_node_t node_list[MAX_NODE];
  bool s_super_var_module_init = false;
  uint8_t s_tx_buf[TX_BUF_SIZE];
};
#endif //end super_var.h
