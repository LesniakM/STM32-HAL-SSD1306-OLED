#ifndef SSD1306_H        // Include guard start.
#define SSD1306_H

#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "SSD1306_font_5x8.h"
#include <string>

using std::string;

constexpr uint8_t SSD1306_HEIGHT {32};
constexpr uint8_t SSD1306_WIDTH {128};

class SSD1306
{
public:
  SSD1306(I2C_HandleTypeDef* _i2c);
  //  Core functions
  uint8_t init();
  void drawPixel(uint8_t x, uint8_t y, bool color);
  void drawPixelToBuff(uint8_t x, uint8_t y, bool color);
  void writeChar(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5]);
  void writeCharToBuff(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5]);
  void writeStringToBuff(string s, uint8_t start_x, uint8_t row, const fontData font[][5]);
  void updateScreen();
  void updateScreenDMA();
  bool x_reverse = false;
  bool y_reverse = false;

private:
  uint8_t _writeCommand(uint8_t command);
  uint8_t _writeData(uint8_t* data, uint16_t data_len);
  uint8_t _writeDataDMA(uint8_t* data, uint16_t data_len);
  void _updatePage(uint8_t x, uint8_t p_index);
  void _updateChar(uint8_t start_x, uint8_t row, uint8_t char_width);
  I2C_HandleTypeDef* _i2c;
  uint8_t _address = (0x3C << 1);
  uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8] {};
};

#endif