#include "SSD1306.h"


//  Constructor
SSD1306::SSD1306(I2C_HandleTypeDef* i2c)
{
  _i2c         = i2c;
  init();
}

  // Init LCD
uint8_t SSD1306::init() {
  HAL_Delay(100);
  uint8_t errors = 0;

  errors += _writeCommand(0xA8);   // Set MUX ratio:
  errors += _writeCommand(0x1F);   // 0x1F (31) for 128x32, 0x3F (63) for 128x64.

  errors += _writeCommand(0xD3);   // Set display offset:
  errors += _writeCommand(0x00);   // No offset.

  errors += _writeCommand(0x40);   // Set Display Start Line (0x40~0x7F).

  errors += _writeCommand(0xA1);   // Set Segment Re-map (A0h/A1h). Set to A0 to flip horizontally.
  errors += _writeCommand(0xC8);   // Set COM Output Scan Direction (Scan direction: from COM0 to COM63, 0xC8 for reversed).

  errors += _writeCommand(0xDA);   // Set COM Pins Hardware Configuration to:
  errors += _writeCommand(0x02);   // 0x02 for 128x32, 0x12 for 128x64. 

  errors += _writeCommand(0x81);   // Set contrast Control for BANK0 (81h) to:
  errors += _writeCommand(0x20);   // 0x7F.

  errors += _writeCommand(0xA4);   // Get pixel info from RAM.

  errors += _writeCommand(0xA6);   // Normal display mode (0 in RAM: OFF, 1 in RAM: ON), 0xA6 for reversed.

  errors += _writeCommand(0xD5);   // Set Display Clock Divide Ratio/ Oscillator Frequency to:
  errors += _writeCommand(0xF0);   // F - max freq, 0 - min divider. 

  errors += _writeCommand(0x8D);   // Set charge pump to:
  errors += _writeCommand(0x14);   // 0x14 - enabled.

  errors += _writeCommand(0x20);   // Set addressing mode to:
  errors += _writeCommand(0x00);   // Horizontal mode. 0x10 for Page mode.

  errors += _writeCommand(0x21);   // Set min and max column
  errors += _writeCommand(0x00);   // Horizontal mode. 0x10 for Page mode.
  errors += _writeCommand(127);   // Horizontal mode. 0x10 for Page mode.

  errors += _writeCommand(0xAF);   // Enable display.

  return errors;
}


void SSD1306::drawPixelToBuff(uint8_t x, uint8_t y, bool color)
{   
  
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
  {
    return;
  }

  if (color)
  {
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
  }
  else
  {
    SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
  }
}

void SSD1306::drawPixel(uint8_t x, uint8_t y, bool color)
{ 
  drawPixelToBuff(x, y, color);
  _updatePage(x, y / 8);
}

void SSD1306::writeChar(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5])
{ 
  uint8_t font_width {5};
  for (int i = 0; i < font_width; i++) 
  {
    SSD1306_Buffer[start_x + row * SSD1306_WIDTH + i] = font[ch][i];
  }
  
  _updateChar(start_x, row, font_width);
}


void SSD1306::writeCharToBuff(unsigned char ch, uint8_t start_x, uint8_t row, const fontData font[][5])
{ 
  uint8_t font_width {5};
  for (int i = 0; i < font_width; i++) 
  {
    SSD1306_Buffer[start_x + row * SSD1306_WIDTH + i] = font[ch][i];
  }
}

void SSD1306::writeStringToBuff(string s, uint8_t start_x, uint8_t row, const fontData font[][5])
{ 
  uint8_t font_width_with_spaces {6};
  uint8_t chars_printed {0};
  for (char character : s)
  { 
    if (start_x + chars_printed*font_width_with_spaces < SSD1306_WIDTH)
    {
      writeCharToBuff(character, start_x + chars_printed*font_width_with_spaces, row, font);
      chars_printed++;
    }
  }
}


uint8_t SSD1306::_writeCommand(uint8_t command)
{
    return HAL_I2C_Mem_Write(_i2c, _address, 0x00, 1, &command, 1, 50);
}

uint8_t SSD1306::_writeData(uint8_t* data, uint16_t data_len)
{
    return HAL_I2C_Mem_Write(_i2c, _address, 0x40, 1, data, data_len, 100);
}

uint8_t SSD1306::_writeDataDMA(uint8_t* data, uint16_t data_len)
{
    return HAL_I2C_Mem_Write_DMA(_i2c, _address, 0x40, 1, data, data_len);
}

// Page is vertical 8 bit chunk of LCD.
void SSD1306::_updatePage(uint8_t x, uint8_t p_index)
{
  _writeCommand(0xB0 + p_index);    // Set the page start address of the target display location by command (0xB0 to 0xB7).
  _writeCommand(x % 16);            // Set the lower start column address of pointer by command (0x00~0x0F).
  _writeCommand(0x10 + x/16);       // Set the upper start column address of pointer by command (0x10~0x1F).

  _writeData(&SSD1306_Buffer[x + p_index * 128], 1);
}

// Page is vertical 8 bit chunk of LCD.
void SSD1306::_updateChar(uint8_t start_x, uint8_t row, uint8_t char_width)
{
  _writeCommand(0xB0 + row);              // Set the page start address of the target display location by command (0xB0 to 0xB7).
  _writeCommand(start_x % 16);            // Set the lower start column address of pointer by command (0x00~0x0F).
  _writeCommand(0x10 + start_x/16);       // Set the upper start column address of pointer by command (0x10~0x1F).

  _writeData(&SSD1306_Buffer[start_x + row * 128], char_width);
}

void SSD1306::updateScreen()
{
    _writeCommand(0xB0);       // Set the page start address of the target display location by command (0xB0 to 0xB7).
    _writeCommand(0x00);       // Set the lower start column address of pointer by command (0x00~0x0F).
    _writeCommand(0x10);       // Set the upper start column address of pointer by command (0x10~0x1F).
    _writeData(&SSD1306_Buffer[0], 512);
}

void SSD1306::updateScreenDMA()
{
    _writeCommand(0xB0);       // Set the page start address of the target display location by command (0xB0 to 0xB7).
    _writeCommand(0x00);       // Set the lower start column address of pointer by command (0x00~0x0F).
    _writeCommand(0x10);       // Set the upper start column address of pointer by command (0x10~0x1F).
    _writeDataDMA(&SSD1306_Buffer[0], 512);
    //_writeDataDMA(&SSD1306_Buffer[0], SSD1306_WIDTH);
}