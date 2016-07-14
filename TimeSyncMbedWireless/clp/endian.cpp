// endian.cpp
// Author:  Joe Cowlyn
// Created: 11/06/16
// Purpose: Functions to switch endianness of values.
// https://www.ibm.com/developerworks/aix/library/au-endianc/
// http://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c

#if !(_MSC_VER || __GNUC__)
#include <stdint.h>

uint32_t _byteswap_ulong(uint32_t i)
{
  unsigned char c1, c2, c3, c4;

  c1 = i & 255;
  c2 = (i >> 8) & 255;
  c3 = (i >> 16) & 255;
  c4 = (i >> 24) & 255;

  return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
#endif

#ifndef _MSC_VER
#include <stdint.h>

uint16_t _byteswap_ushort(uint16_t i)
{
  unsigned char c1, c2;

  c1 = i & 255;
  c2 = (i >> 8) & 255;

  return ((short)c1 << 8) + c2;
}
#endif
