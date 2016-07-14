// crc.cpp
// Author:  Joe Cowlyn
// Created: 10/06/16
// Purpose: Functions to generate CRC16-CCITT parity values.

#include "crc.h"

uint16_t gencrc_16(uint16_t i)
{
  uint16_t j;
  uint16_t k;
  uint16_t crc;

  k = i<<8;
  crc = 0;

  for(j = 0; j < 8; j++)
  {
    if((crc ^ k)&0x8000)
    {
      crc = (crc<<1)^0x1021;
    }
    else
    {
      crc <<= 1;
    }
    k <<= 1;
  }

  return(crc);
}

uint16_t CalculateCharacterCRC16(uint16_t crc, uint8_t c)
{
  return ((crc<<8)^gencrc_16((crc>>8)^c));
}

uint16_t CalculateBufferCRC16(const uint8_t* buffer, size_t length, uint16_t start = 0x1D0F)
{
  uint16_t crc = start;
//  uint16_t crc = 0xFFFF;

  for(size_t i = 0; i < length; ++i)
  {
    crc = CalculateCharacterCRC16(crc, buffer[i]);
  }
  return crc;
}
