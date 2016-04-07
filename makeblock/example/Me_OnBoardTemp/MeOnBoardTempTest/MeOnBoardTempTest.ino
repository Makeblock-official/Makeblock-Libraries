/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * @file    MeOnBoardTempTest.ino
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2016/04/01
 * @brief   Description: this file is sample code for On Board Temperature device.
 *
 * Function List:
 *    1. uint16_t MeOnBoardTemp::readAnalog(void);
 *    2. float MeOnBoardTemp::readValue(void)
 *
 * \par History:
 * <pre>
 * <Author>     <Time>        <Version>      <Descr>
 * Mark Yan     2016/04/01    1.0.0          build the new
 * </pre>
 */
#include "MeAuriga.h"

MeOnBoardTemp OnBoardTemp(PORT_13);

void setup() 
{
  Serial.begin(115200);
}

void loop() 
{
  Serial.print("Analog Value is: ");
  Serial.println(OnBoardTemp.readAnalog());
  Serial.print("Temp Value is: ");
  Serial.println(OnBoardTemp.readValue());
  delay(200);
}
