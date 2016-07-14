// endian.h
// Author:  Joe Cowlyn
// Created: 11/06/16
// Purpose: Functions to switch endianness of values.
// https://www.ibm.com/developerworks/aix/library/au-endianc/
// http://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c

#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#ifdef _MSC_VER

#include <intrin.h>

#elif __GNUC__

#include <stdint.h>
#define _byteswap_ulong __builtin_bswap32
uint16_t _byteswap_ushort(uint16_t i);

#else

#include <stdint.h>
uint16_t _byteswap_ushort(uint16_t i);
uint32_t _byteswap_ulong(uint32_t i);

#endif

#endif
