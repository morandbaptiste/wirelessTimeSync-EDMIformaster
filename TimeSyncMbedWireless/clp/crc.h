// crc.h
// Author:  Joe Cowlyn
// Created: 10/06/16
// Purpose: Functions to generate CRC16-CCITT parity values.

#ifndef __CRC_H__
#define __CRC_H__

#include <stddef.h>
#include <stdint.h>

uint16_t gencrc_16(uint16_t i);
uint16_t CalculateCharacterCRC16(uint16_t crc, uint8_t c);
uint16_t CalculateBufferCRC16(const uint8_t* buffer, size_t length, uint16_t start);

#endif
