/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeHostParser
 * \brief   Driver for Me Host Parser module.
 * @file    MeHostParser.h
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/12
 * @brief   Header for MeHostParser.cpp module
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
 * This file is a drive for Me Host Parser device, The Me Host Parser inherited the 
 * MeSerial class from SoftwareSerial.
 *
 * \par Method List:
 *
 *    1. uint8_t MeHostParser::pushStr(uint8_t * str, uint32_t length);
 *    2. uint8_t MeHostParser::pushByte(uint8_t ch);
 *    3. uint8_t MeHostParser::run();
 *    4. uint8_t MeHostParser::getPackageReady();
 *    5. uint8_t MeHostParser::getData(uint8_t *buf, uint32_t size);
 *    6. void MeHostParser::print(char *str, uint32_t * cnt);
 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * forfish         2015/11/12    1.0.0            Add description
 * </pre>
 *
 */

#ifndef MeHostParser_h
#define MeHostParser_h
#include <Arduino.h>
#define BUF_SIZE            256
#define MASK                255

/**
 * Class: MeHostParser
 * \par Description
 * Declaration of Class MeHostParser.
 */
class MeHostParser
{
public:
/**
 * Alternate Constructor which can call your own function to map the Host Parser to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
*/
  MeHostParser();
	
/**
 * Alternate Destructor which can call the system's destructor to free space,
 * no pins are used or initialized here.
 * \param[in]
 *   None
*/
  ~MeHostParser();

/**
 * \par Function
 *    pushStr
 * \par Description
 *    To push a string to Host Parser.
 * \param[in]
 *    str - A pointer to a string.
 * \param[in]
 *    length - The length of the string.
 * \par Output
 *    None
 * \par Return
 *    Return the index of pushing string.
 * \par Others
 *    None
 */
   uint8_t pushStr(uint8_t * str, uint32_t length);
   
/**
 * \par Function
 *    pushByte
 * \par Description
 *    To push a byte to Host Parser.
 * \param[in]
 *    ch - A pointer to a char.
 * \par Output
 *    None
 * \par Return
 *    Return the status of pushing char.
 * \par Others
 *    None
 */
   uint8_t pushByte(uint8_t ch);

/**
 * \par Function
 *    run
 * \par Description
 *    The running status of Host Parser.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the status of Host Parser's running.
 * \par Others
 *    None
 */
  uint8_t run();

/**
 * \par Function
 *    getPackageReady
 * \par Description
 *    Get the package ready state.
 * \param[in]
 *    None
 * \par Output
 *    None
 * \par Return
 *    Return the status ready or not.
 * \par Others
 *    None
 */
   //  get the package ready state
   uint8_t getPackageReady();
   
/**
 * \par Function
 *    getData
 * \par Description
 *    Copy data to user's buffer.
 * \param[in]
 *    buf - A buffer for a getting data.
 * \param[in]
 *    size - The length of getting data.
 * \par Output
 *    None
 * \par Return
 *    Return the length of getting data or 0.
 * \par Others
 *    None
 */
  uint8_t getData(uint8_t *buf, uint32_t size);

  void print(char *str, uint32_t * cnt);
private:
  int state;
  uint8_t buffer[BUF_SIZE];
  uint32_t in;
  uint32_t out;
  uint8_t packageReady;

  uint8_t module;
  uint32_t length;
  uint8_t *data;
  uint8_t check;

  uint32_t lengthRead;
  uint32_t currentDataPos;

/**
 * \par Function
 *    getByte
 * \par Description
 *    To get a byte from Host Parser.
 * \param[in]
 *    ch - A pointer to a char.
 * \par Output
 *    None
 * \par Return
 *    Return the status of getting char.
 * \par Others
 *    None
 */
  uint8_t getByte(uint8_t * ch);
};


#endif 
