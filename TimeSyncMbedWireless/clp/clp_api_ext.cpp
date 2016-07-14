#include "clp_api_ext.h"
#include "crc.h"
#include "endian.h"
#include <string.h>

extern int CLP_G_ERROR_VAL;

size_t CLP_EXTENSION_GenerateTimeSyncCommand(uint8_t* buffer, const uint8_t* data, size_t datalength)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  if(!data)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
    datalength = 0;
  }

  buffer[0] = CLP_EXTENSION_COMMAND_TIME_SYNC;

  if(data)
  {
    memcpy(buffer + 1, data, datalength);
  }

  return 1 + datalength;
}

#include "hmi.h"
size_t CLP_EXTENSION_GenerateTimeSyncPacket(uint8_t* buffer, CLP_Extended* ext, const uint8_t* data, size_t datalength)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_ESCAPE_STX;
  size_t length = 1;
  if(ext != 0)
  {
    length += CLP_GenerateExtendedCommand(buffer + length, (*ext));
  }
  length += CLP_EXTENSION_GenerateTimeSyncCommand(buffer + length, data, datalength);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  
  //  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  buffer[length] = crc & 0xff;
  buffer[length + 1] = (crc >> 8) & 0xff;
  length += 2;
  
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length] = CLP_ESCAPE_ETX;
  
  length += 1;

  return length;
}

size_t CLP_EXTENSION_ConvertPacketTimeSyncResponseToRaw(const uint8_t* packet, size_t packetlength, uint8_t* buffer, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_EXTENSION_COMMAND_TIME_SYNC)
  {
    if(buffer)
    {
      start += 1;

      memcpy(buffer, packet + start, packetlength - start - 3);
	  
	  return packetlength - start - 3;
    }
    else
    {
      CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    }
  }
  else
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_INVALID_REG_TYPE;
  }
  
  return 0;
}

void CLP_EXTENSION_ConvertPacketTimeSyncResponseToRawOfSize(const uint8_t* packet, size_t datalength, uint8_t* buffer, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_EXTENSION_COMMAND_TIME_SYNC)
  {
    if(buffer)
    {
      start += 1;
      memcpy(buffer, packet + start, datalength);
    }
    else
    {
      CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    }
  }
  else
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_INVALID_REG_TYPE;
  }
}