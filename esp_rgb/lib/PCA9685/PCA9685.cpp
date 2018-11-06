// #define STOP_BUS
// #define DEBUG_COL

#include <brzo_i2c.h>

#include "PCA9685.h"

PCA9685::PCA9685(){
  #ifndef STOP_BUS
  brzo_i2c_setup(1, 3, 1000);
  initPCA9685();
  #endif
  initCol();
}

uint16_t PCA9685::getLedValue(uint8_t ledIndex){
  regToUint16(  this->col[ledIndex * 4 + 1],
                this->col[ledIndex * 4 + 2]);
  return this->currentColor;
}

void PCA9685::setLedValue(  uint8_t ledIndex,
                            uint16_t value) {
  if (value < 4096) {
    uint16ToReg(ledIndex, value);
    parseColors(this->reg, 2);
  }
}

void PCA9685::writeBusCommands(){
  for (uint8_t i = 0; i < 16; i++) {
    if (busCommand[i] > 0) {
      if (busCommand[i] == 1) {
        regDown(i);
      }else{
        regUp(i);
      }
      this->busCommand[i] = 0;
    }
  }
  // while (!(queue.empty())) {
  //   BusCommand* command = queue.front();
  //   if (command->ledUP()) {
  //     regUp(command->ledIndex());
  //   }else{
  //     regDown(command->ledIndex());
  //   }
  //   queue.pop();
  //   delete command;
  // }
}

void PCA9685::regUp(uint8_t ledIndex) {
  regToUint16(col[(ledIndex * 4) + 1], col[(ledIndex * 4) + 2]);
  if ( this->currentColor < 4095) {
    uint16ToReg(ledIndex, this->currentColor + 1);
    parseColors(this->reg, 2);
  }
}

void PCA9685::regDown(uint8_t ledIndex) {
  regToUint16(col[(ledIndex * 4) + 1], col[(ledIndex * 4) + 2]);
  if ( this->currentColor > 0) {
    uint16ToReg(ledIndex, this->currentColor - 1);
    parseColors(this->reg, 2);
  }
}

void PCA9685::regToUint16(  uint8_t L,
                            uint8_t H )
{
  this->currentColor = (uint16_t)(  (((uint16_t)H & (uint16_t)0x0F) << 8) | (uint16_t)L   );
}

void PCA9685::uint16ToReg(  uint8_t ledIndex,
                            uint16_t value)
{
  this->reg[0] = (uint8_t)value;
  this->reg[1] = (uint8_t)(((value >> 8) & 0x0F) | (ledIndex << 4));
}

void PCA9685::ledUp(    uint8_t ledIndex   ) {
  this->busCommand[ledIndex] = 2;
  // queue.push(new BusCommand(true, ledIndex));
}

void PCA9685::ledDown(  uint8_t ledIndex    ) {
  this->busCommand[ledIndex] = 1;
  // queue.push(new BusCommand(false, ledIndex));
}

void ICACHE_RAM_ATTR PCA9685::parseColors(  uint8_t *data,
                                            uint8_t length)
{
  #ifndef STOP_BUS
  brzo_i2c_start_transaction(0x40, 1000);
  #endif
  for (int i = 0; i < length; i+=2) {
    data0 = data[i];
    data1 = data[i+1];
    regLow = (data1 >> 4)*4;
    data1 &= 0x0F;
    colorLow = regLow +1;
    colorHigh = colorLow+1;

    #ifdef DEBUG_COL
    delay(300);
    Serial.printf("ledIndex %d\n", regLow/4);
    #endif

    if (col[colorLow] != data0) {
      if (col[colorHigh] == data1) {
        this->col[colorLow] = data0;
        #ifdef DEBUG_COL
        Serial.printf("col[colorLow] %X\n", this->col[colorLow]);
        #endif
        #ifndef STOP_BUS
        brzo_i2c_write(&this->col[regLow], 2, false);
        #endif
      }else{
        this->col[colorLow] = data0;
        this->col[colorHigh] = data1;
        #ifdef DEBUG_COL
        Serial.printf("col[colorLow] %X\n", this->col[colorLow]);
        Serial.printf("col[colorHigh] %X\n", this->col[colorHigh]);
        #endif
        #ifndef STOP_BUS
        brzo_i2c_write(&this->col[regLow], 3, false);
        #endif
      }
    }else{
      if (this->col[colorHigh] != data1) {
        this->col[colorHigh] = data1;
        reg[0] = this->col[colorHigh + 1];
        reg[1] = this->col[colorHigh];
        #ifdef DEBUG_COL
        Serial.printf("col[colorHigh] %X\n", this->col[colorHigh]);
        #endif
        #ifndef STOP_BUS
        brzo_i2c_write(reg, 2, false);
        #endif
      }
    }
  }
  #ifndef STOP_BUS
  brzo_i2c_end_transaction();
  #endif
}

void ICACHE_RAM_ATTR PCA9685::initPCA9685(){
  uint8_t preScale[2];
  preScale[0] = 0xFE;
  preScale[1] = 0x03;

  uint8_t mode1[2];
  mode1[0] = 0x00;
  mode1[1] = 0x21;

  uint8_t mode2[2];
  mode2[0] = 0x01;
  mode2[1] = 0x05;

  brzo_i2c_start_transaction(0x40, 1000);
  brzo_i2c_write(preScale, 2, false);
  brzo_i2c_write(mode1, 2, false);
  brzo_i2c_write(mode2, 2, false);
  brzo_i2c_end_transaction();
}

void PCA9685::initCol() {
  for (uint8_t i = 0; i < 64; i++) {
    col[i] = 0;
  }

  // s1
  col[0] =  0x40;
  col[3] =  0x41;

  col[4] =  0x44;
  col[7] =  0x45;

  col[8] =  0x3C;
  col[11] = 0x3D;

  col[12] = 0x38;
  col[15] = 0x39;

  // s2
  col[16] = 0x10;
  col[19] = 0x11;

  col[20] = 0x14;
  col[23] = 0x15;

  col[24] = 0x0C;
  col[27] = 0x0D;

  col[28] = 0x08;
  col[31] = 0x09;

  // s3
  col[32] = 0x20;
  col[35] = 0x21;

  col[36] = 0x24;
  col[39] = 0x25;

  col[40] = 0x1C;
  col[43] = 0x1D;

  col[44] = 0x18;
  col[47] = 0x19;

  // s4
  col[48] = 0x30;
  col[51] = 0x31;

  col[52] = 0x34;
  col[55] = 0x35;

  col[56] = 0x2C;
  col[59] = 0x2D;

  col[60] = 0x28;
  col[63] = 0x29;
}
