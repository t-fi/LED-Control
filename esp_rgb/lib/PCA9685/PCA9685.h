#ifndef PCA9685_H
#define PCA9685_H

#include <stdint.h>


class PCA9685{
private:
    // std::queue<BusCommand*> queue;
    // BusCommand* command;
    uint8_t busCommand[16];

    uint8_t col[64];
    uint8_t reg[2];

    uint8_t regLow;
    uint8_t colorLow;
    uint8_t colorHigh;
    uint8_t data0;
    uint8_t data1;
    uint8_t dataColH;

    uint16_t currentColor;

    // uint8_t testCount = 0;

    void initPCA9685();
    void initCol();

    void writeBusCommands();
    void regUp(uint8_t ledIndex);
    void regDown(uint8_t ledIndex);
    void regToUint16( uint8_t L,
                          uint8_t H );
    void uint16ToReg( uint8_t ledIndex,
                      uint16_t value );


public:
    PCA9685();

    // void init();

    void run();

    uint16_t getLedValue(uint8_t ledIndex);
    void setLedValue(uint8_t ledIndex, uint16_t value);
    void ledUp( uint8_t ledIndex    );
    void ledDown(   uint8_t ledIndex    );
    void parseColors( uint8_t *data,
                      uint8_t length  );
    // virtual ~PCA9685 ();
};

#endif // PCA9685_H
