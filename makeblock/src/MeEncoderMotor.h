#ifndef MeEncoderMotor_h
#define MeEncoderMotor_h

#include <stdbool.h>
#include "MeConfig.h"
#include "MePort.h"

///@brief Class for Encoder Motor Driver
class MeEncoderMotor: public MePort{
    public:
        MeEncoderMotor(uint8_t addr,uint8_t slot);
        MeEncoderMotor(uint8_t slot);
        MeEncoderMotor();
        void begin();
        boolean reset();
        boolean move(float angle, float speed);
        boolean moveTo(float angle, float speed);
        boolean runTurns(float turns, float speed);
        boolean runSpeed(float speed);
        boolean runSpeedAndTime(float speed, float time);
        float getCurrentSpeed();
        float getCurrentPosition();
    private:
		void request(byte *writeData, byte *readData, int wlen, int rlen);
        uint8_t _slot;    
		uint8_t _slaveAddress;
};
#endif
