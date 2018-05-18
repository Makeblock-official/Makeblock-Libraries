/**
 * \par Copyright (C), 2012-2016, MakeBlock
 * \class   MeHostParser
 * \brief   Driver for Me Host Parser module.
 * @file    MeHostParser.cpp
 * @author  MakeBlock
 * @version V1.0.0
 * @date    2015/11/12
 * @brief   Driver for Me Host Parser module.
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

#include "MeHostParser.h"

#define HEAD    0xA5
#define TAIL    0x5A

//  states
#define ST_WAIT_4_START     0x01
#define ST_HEAD_READ        0x02
#define ST_MODULE_READ      0x03
#define ST_LENGTH_READ      0x04
#define ST_DATA_READ        0x05
#define ST_CHECK_READ       0x06

/**
 * Alternate Constructor which can call your own function to map the Host Parser to arduino port,
 * no pins are used or initialized here.
 * \param[in]
 *   None
*/
MeHostParser::MeHostParser()
{
    state = ST_WAIT_4_START;
    in = 0;
    out = 0;
    packageReady = 0;

    module = 0;
    length = 0;
    data = NULL;
    check = 0;

    lengthRead = 0;
    currentDataPos = 0;
}

/**
 * Alternate Destructor which can call the system's destructor to free space,
 * no pins are used or initialized here.
 * \param[in]
 *   None
*/
MeHostParser::~MeHostParser()
{
    ;
}

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
uint8_t MeHostParser::getPackageReady()
{
    return (1 == packageReady);
}

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
uint8_t MeHostParser::pushStr(uint8_t * str, uint32_t length)
{
    if (length > ((in + BUF_SIZE - out - 1) & MASK))
    {
        return 0;
    }
    else
    {
        for (int i = 0; i < length; ++i)
        {
            pushByte(str[i]);
        }
    }
}

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
uint8_t MeHostParser::pushByte(uint8_t ch)
{
    if (((in + 1) & MASK) != out)
    {
        buffer[in] = ch;
        ++in;
        in &= MASK;
        return 1;
    }
    else
    {
        return 0;
    }
}

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
uint8_t MeHostParser::getByte(uint8_t * ch)
{
    if (in != out)
    {
        *ch = buffer[out];
        ++out;
        out &= MASK;
        return 1;
    }
    else
    {
        // Serial.println("GET error!");
        return 0;
    }
}

/**
 * \par Function
 *    calculateLRC
 * \par Description
 *    To calculate the LRC.
 * \param[in]
 *    data - A pointer to data.
 * \param[in]
 *    length - The length of LRC.
 * \par Output
 *    None
 * \par Return
 *    Return the LRC.
 * \par Others
 *    None
 */
uint8_t calculateLRC(uint8_t *data, uint32_t length)
{
    uint8_t LRC = 0;
    for (uint32_t i = 0; i < length; ++i)
    {
        LRC ^= data[i];
    }
    return LRC;
}

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
uint8_t MeHostParser::run(void)
{
    uint8_t ch = 0;
    while (getByte(&ch))
    {
        switch (state)
        {
        case ST_WAIT_4_START:
            if (HEAD == ch)
            {
                state = ST_HEAD_READ;
            }
            break;
        case ST_HEAD_READ:
            module = ch;
            state = ST_MODULE_READ;
            break;
        case ST_MODULE_READ:
            //  read 4 bytes as "length"
            *(((uint8_t *)&length) + lengthRead) = ch;
            ++lengthRead;
            if (4 == lengthRead)
            {
                lengthRead = 0;
                state = ST_LENGTH_READ;
            }
            break;
        case ST_LENGTH_READ:
            //  alloc space for data
            if (0 == currentDataPos)
            {
                if (length > 255)
                {
                    state = ST_WAIT_4_START;
                    currentDataPos = 0;
                    lengthRead = 0;
                    length = 0;
                    module = 0;
                    check = 0;
                    break;
                }
                data = (uint8_t *)malloc(length + 1);
                if (NULL == data)
                {
                    state = ST_WAIT_4_START;
                    currentDataPos = 0;
                    lengthRead = 0;
                    length = 0;
                    module = 0;
                    check = 0;
                    break;
                }
            }
            //  read data
            data[currentDataPos] = ch;
            ++currentDataPos;
            if (currentDataPos == length)
            {
                currentDataPos = 0;
                state = ST_DATA_READ;
            }
            break;
        case ST_DATA_READ:
            check = ch;
            if (check != calculateLRC(data, length))
            {
                state = ST_WAIT_4_START;
                if (NULL != data)
                {
                    free(data);
                    data = NULL;
                }
                currentDataPos = 0;
                lengthRead = 0;
                length = 0;
                module = 0;
                check = 0;
            }
            else
            {
                state = ST_CHECK_READ;
            }
            break;
        case ST_CHECK_READ:
            if (TAIL != ch)
            {
                if (NULL != data)
                {
                    free(data);
                    data = NULL;
                }
                length = 0;
            }
            else
            {
                packageReady = 1;
            }
            state = ST_WAIT_4_START;
            currentDataPos = 0;
            lengthRead = 0;
            module = 0;
            check = 0;
            break;
        default:
            break;
        }
    }
    return state;
}

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
uint8_t MeHostParser::getData(uint8_t *buf, uint32_t size)
{
    int copySize = (size > length) ? length : size;
    if ((NULL != data) && (NULL != buf))
    {
        memcpy(buf, data, copySize);
        free(data);
        data = NULL;
        length = 0;
        packageReady = 0;

        return copySize;
    }
    else
    {
        return 0;
    }
}
