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

#include "MeSuperVariable.h"

/**
 * initialized here.
 * \param[in]
 *   None
 */
MeSuperVariable::MeSuperVariable()
{
  
}
  
/**
 * Finalize here.
 * \param[in]
 *   None
 */
MeSuperVariable::~MeSuperVariable()
{
  
}

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
void MeSuperVariable::comm_protocol_pump_char( uint8_t c, uint8_t *output_buf, uint32_t *output_len)
{
  fsm_t *p_fsm;

  static bool packet_flag = false;
  if(!output_buf || !output_len)
  {
  	packet_flag = false;
    return;
  } 

  p_fsm = &s_fsm_tab[0];

  *output_len = 0;
  switch(p_fsm->s)
  {
    case S_HEAD:
      if(FRAME_HEADER == c)
      {
        p_fsm->len = 0;
        p_fsm->recv_head_check = 0;
        p_fsm->calc_head_check = FRAME_HEADER;
        p_fsm->recv_len = 0;
        p_fsm->check = 0;
        p_fsm->s = S_HEAD_CHECK;
      }
    break;

    case S_HEAD_CHECK:
      p_fsm->recv_head_check = c;
      p_fsm->s = S_LEN1;
    break;

    case S_LEN1:
      p_fsm->calc_head_check += c;
      p_fsm->len += c;
      p_fsm->s = S_LEN2;
    break;

    case S_LEN2:
      p_fsm->calc_head_check += c;
      p_fsm->len += c * 256;
      if(p_fsm->recv_head_check == p_fsm->calc_head_check)
      {
        p_fsm->s = S_DATA;
      }
      else
      {
      	packet_flag = false;
        p_fsm->s = S_HEAD;
      }
    break;

    case S_DATA:
      p_fsm->check += c;
      p_fsm->data[p_fsm->recv_len++] = c;
      if(p_fsm->recv_len == p_fsm->len)
      {
      	packet_flag = false;
        p_fsm->s = S_CHECK;
      }
    break;

    case S_CHECK:
      if(p_fsm->check == c)
      {
        p_fsm->s = S_END;
      }
      else
      {
      	packet_flag = false;
        p_fsm->s = S_HEAD;
      }
    break;

    case S_END:
      if(FRAME_END == c)
      {
        if((p_fsm->data[0]) == 0x02)
        {
        	packet_flag = true;
        	*output_len = p_fsm->len - 1;
        }
      }
      p_fsm->s = S_HEAD;
    break;
    default:
    	packet_flag = false;
    break;
  }
  if (packet_flag == true)
  {
  	packet_flag = false;
  	*output_len = 0;
  	arduino_super_var(p_fsm->data + 1, p_fsm->len - 1, output_buf, output_len);
  }
}

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
void MeSuperVariable::arduino_super_var(uint8_t *data, uint32_t len, uint8_t *output_buf, uint32_t *output_len)
{
  arduino_super_var_init();
  
  if(!output_buf || !output_len)
  {
    return;
  }
  super_var_frame_parse(data, len);
  *output_len = 0;
}

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
void * MeSuperVariable::set_variable(char *name, uint8_t type, void *data, uint8_t len)//设置变量的值，要指定数据类型
{
  bool ret;
  bool bool_val,*bool_p;
  float float_val,*float_p;
  int32_t int_val,*int_p;
  const char *str_val;
  uint16_t str_val_len;
  super_var_node_t *node;

  arduino_super_var_init();
  node = super_var_find_by_name(name);
  if (!node)
  {
    node = super_var_create(name);
    if (!node)
    {
      return false;
    }
  }

  switch (type)
  {
  case SUPER_VAR_BOOL_T:
    bool_p = (bool *)(data);
    bool_val = (*bool_p == 1)?1:0;
    node_value_update(node,SUPER_VAR_BOOL_T,(const uint8_t *)(&bool_val), len);
    ret = true;
    super_var_send_value(node);
  break;

  case SUPER_VAR_INT_T:
  case SUPER_VAR_UN_INT_T:
    int_p = (int32_t *)(data);
    int_val = *int_p;
    node_value_update(node,SUPER_VAR_INT_T,(const uint8_t *)(&int_val), len);
    ret = true;
    super_var_send_value(node);
  break;
  case SUPER_VAR_FLOAT_T:
    float_p = (float *)(data);
    float_val = *float_p;
    node_value_update(node,SUPER_VAR_FLOAT_T,(const uint8_t *)(&float_val), len);
    ret = true;
    super_var_send_value(node);
  break;

  case SUPER_VAR_STR_T:	
    str_val_len = len;
    str_val = (const char *)data;
    node_value_update(node,SUPER_VAR_STR_T,(const uint8_t *)(str_val), len);
    ret = true;
    super_var_send_value(node);
  break;
  default:
    ret = false;
  break;
  }
}

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
bool MeSuperVariable::get_super_value(char *name,void *data)
{
  bool ret;
  super_var_node_t *node;

  arduino_super_var_init();
  ret = true;
  node = super_var_find_by_name(name);
  if (!node)
  {
    // Serial.println("Not found!!!");
    return false;
  }

  switch (node->type)
  {
    case SUPER_VAR_NONE_T:
    break;

    case  SUPER_VAR_CHAR_T:
    break;

    case  SUPER_VAR_UN_CHAR_T:
    break;

    case  SUPER_VAR_SHORT_T:
    break;

    case  SUPER_VAR_UN_SHORT_T:
    break;

    case  SUPER_VAR_INT_T:
      int32_t var_int_val;
      super_var_val.byteVal[0] = node->value[0];
      super_var_val.byteVal[1] = node->value[1];
      super_var_val.byteVal[2] = node->value[2];
      super_var_val.byteVal[3] = node->value[3];
      var_int_val = super_var_val.int32Val;

      *(int32_t *)data = var_int_val;

      ret = true;
    break;

    case  SUPER_VAR_UN_INT_T:
    break;

    case  SUPER_VAR_LL_T:
    break;

    case  SUPER_VAR_UN_LL_T:
    break;

    case  SUPER_VAR_FLOAT_T:
      float var_float_val;
      super_var_val.byteVal[0] = node->value[0];
      super_var_val.byteVal[1] = node->value[1];
      super_var_val.byteVal[2] = node->value[2];
      super_var_val.byteVal[3] = node->value[3];
      var_float_val = super_var_val.floatVal;

      *(float *)data = var_float_val;

      ret = true;
    break;

    case  SUPER_VAR_DOUBLE_T:
    break;

    case  SUPER_VAR_BOOL_T:
      bool var_bool_val;
      var_bool_val = node->value[0];

      *(bool *)data = var_bool_val;

      ret = true;
    break;

    case  SUPER_VAR_STR_T:
      memcpy(data, node->value, node->len);
      ret = true;
    break;

    case SUPER_VAR_LIST_T:
    break;

    case SUPER_VAR_NUM:
    break;
  }

  return ret;
}

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
float MeSuperVariable::get_variable(char *name)
{
  float ret =0;
  super_var_node_t *node;

  arduino_super_var_init();
  node = super_var_find_by_name(name);
  if (!node)
  {
    // Serial.println("Not found!!!");
    return ret;
  }

  switch (node->type)
  {
    case  SUPER_VAR_FLOAT_T:
      float var_float_val;
      super_var_val.byteVal[0] = node->value[0];
      super_var_val.byteVal[1] = node->value[1];
      super_var_val.byteVal[2] = node->value[2];
      super_var_val.byteVal[3] = node->value[3];
      var_float_val = super_var_val.floatVal;

      ret = var_float_val;    //return float value.
    break;

    default:
      ret = 0;
    break;
  }

  return ret;
}

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
void MeSuperVariable::comm_build_frame(uint8_t protocol_id, uint8_t *inoutput_data, uint32_t *inoutput_len)
{
  uint16_t pos;
  uint8_t tmp_buf[(*inoutput_len + 7)];
  
  /*header + head_check + len1 + len 2 + protocol_id + data + checksum + end */

  pos = 0;
  tmp_buf[pos++] = FRAME_HEADER;
  tmp_buf[pos++] = FRAME_HEADER + ((*inoutput_len + 1)&(0xFF)) + (((*inoutput_len + 1)>>8)&0xFF);
  tmp_buf[pos++] = (*inoutput_len + 1)&(0xFF);
  tmp_buf[pos++] = ((*inoutput_len + 1)>>8)&0xFF;
  tmp_buf[pos++] = protocol_id;

  memcpy(tmp_buf+pos, inoutput_data, *inoutput_len);
  pos += (*inoutput_len);

  tmp_buf[pos] = calc_add_check_sum(inoutput_data, *inoutput_len);
  tmp_buf[pos++] += protocol_id;

  tmp_buf[pos++] = FRAME_END;

  *inoutput_len = pos;
  memcpy(inoutput_data, tmp_buf, *inoutput_len);
}

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
uint8_t MeSuperVariable::calc_add_check_sum(uint8_t *data, uint32_t len)
{
  uint32_t i;
  uint8_t check_sum;

  check_sum = 0;
  for(i = 0; i < len; i++)
  {
    check_sum += data[i];
  }

  return check_sum;
}

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
void MeSuperVariable::arduino_super_var_init(void)
{
  if(!s_super_var_module_init)
  {
    for (int i = 0; i < MAX_NODE; i++)
    {
    	node_list[i] = {false,{0x00},SUPER_VAR_NONE_T,0,{0}};
    }
    s_super_var_module_init = true;
  }
}

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
void MeSuperVariable::send_data(uint8_t *senddata, uint8_t len)
{
  int8_t i;

  for(i = 0; i < len ; i++)
  {
   	Serial.write(senddata[i]);
  }
}

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
void MeSuperVariable::char_to_string(char *char_data, String *str,uint8_t len)
{ 
  for (int i = 0; i < len; i++)
  {
    str += char_data[i];
  }
}

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
void MeSuperVariable::super_var_frame_parse(uint8_t *data, uint8_t len)
{
  super_var_node_t *super_var;
  uint16_t super_var_name_len,super_var_value_len;
  super_var_type_t super_var_type;
  uint8_t *super_var_name;
  uint8_t *super_var_value;

  super_var_name = data;
  super_var_name_len = strlen((const char *)super_var_name);
  if(super_var_name_len > NAME_MAX_LEN)
  {
    return;
  }
  if(len <= super_var_name_len + 2)
  {
    return;
  }
  super_var_value_len = len - (super_var_name_len + 2);
  super_var_type = (super_var_type_t)data[super_var_name_len + 1];
  super_var_value = data + super_var_name_len + 2;

  super_var = super_var_find_by_name((const char *)super_var_name);
  if (!super_var)
  {
    // Serial.println("a new super var");
    super_var = super_var_create((const char *)super_var_name);
    if (super_var)
    {
      node_value_update(super_var,super_var_type,super_var_value,super_var_value_len);
    }
    else
    {
      return;
    }
  }
  else
  {
    // Serial.println("alreadey have node!");
    node_value_update(super_var,super_var_type,super_var_value,super_var_value_len);
  }
}

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
super_var_node_t * MeSuperVariable::super_var_create(const char *name)
{
  super_var_node_t *super_var;

  super_var = malloc_a_node();
  if(super_var)
  {
    strcpy(super_var->name, name);
  }

  return super_var;
}

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
void MeSuperVariable::node_value_update(super_var_node_t *super_var, super_var_type_t val_type, const uint8_t *val_data, uint8_t value_len)
{
  super_var->type = val_type;
  if (super_var->value)
  {
    for (int i = 0; i < VALUE_MAX_LEN; i++)
    {
      super_var->value[i] = 0;
    }
  }
  super_var->len = value_len;
  memcpy(super_var->value, val_data, value_len);
}

/**
 * \par Function
 *    super_var_send_value
 * \par Description
 *    super variable send value.
 * \param[in]
 *    super_var - node pointer.
 * \par Output
 *    None
 * \return
 *    None
 * \par Others
 *    None
 */
void MeSuperVariable::super_var_send_value(super_var_node_t *super_var)
{
  uint32_t data_len;

  super_var_build_data(super_var, s_tx_buf, &data_len);
  comm_build_frame(SUPER_VAR_ID, s_tx_buf, &data_len);
  send_data(s_tx_buf, data_len);
}

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
void MeSuperVariable::super_var_build_data(super_var_node_t *super_var, uint8_t *buf, uint32_t *out_len)
{
  uint32_t idx = 0;

  // 1) name  
  strcpy((char *)(buf + idx), super_var->name);
  idx += strlen(super_var->name);
  buf[idx++] = '\0';

  // 2) type
  buf[idx++] = super_var->type;

  // 3) value
  memcpy(buf + idx, super_var->value, super_var->len);
  idx += super_var->len;

  *out_len = idx;
}

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
super_var_node_t * MeSuperVariable::malloc_a_node(void)
{
  for (int i = 0; i < MAX_NODE; i++)
  {
    if (false == node_list[i].used)
    {
      node_list[i].used = true;
      return &node_list[i];
    }
  }
  return NULL;
}

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
super_var_node_t * MeSuperVariable::super_var_find_by_name(const char *name)
{
  for (int i = 0; i < MAX_NODE; i++)
  {
    if (true == node_list[i].used && 0 == strcmp(name, node_list[i].name))
    {
      return &node_list[i];
    }
  }
  return NULL;
}

