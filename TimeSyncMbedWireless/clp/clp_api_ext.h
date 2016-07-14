#pragma once

extern "C"
{

#include "clp_api.h"

////////////////////////////////////////////////////////////////
//                 TIME SYNC PACKET SETUP                     //
////////////////////////////////////////////////////////////////
/*
#define _CLP_USE_EXTENSIONS_
#include "clp.h"

void TimeSyncSendData(const uint8_t* data, size_t datasize)
{
  uint8_t packet[CLP_UTILITY_MAX_PACKET_SIZE];
  size_t packetsize = CLP_EXTENSION_GenerateTimeSyncPacket(packet, NULL, data, datalength);

  size_t count = 0;
  while(packet[count] != CLP_ESCAPE_ETX)
  {
    putc(packet[count++]);
  }
  putc(packet[count]);
}

void TimeSyncRecvData(const uint8_t* packet, uint8_t* data)
{
  uint8_t buffer[CLP_UTILITY_MAX_PACKET_SIZE];
  size_t packetsize = CLP_ParsePacketEscape(packet, buffer);

  if(!CLP_CheckPacketCRCFail(packet, packetlength, false))
  {
    CLP_EXTENSION_ConvertPacketTimeSyncResponseToRaw(buffer, packetsize, data);
    if(CLP_GetLastError == CLP_LIB_ERROR_NONE)
    {
      // Signal data ready
    }
  }
}
*/

// CLP Exension Command types
#define CLP_EXTENSION_COMMAND_TIME_SYNC 'T'

// Extended Commands
//   Extract time sync data
size_t CLP_EXTENSION_GenerateTimeSyncCommand(uint8_t* buffer, const uint8_t* data, size_t datalength);
size_t CLP_EXTENSION_GenerateTimeSyncPacket(uint8_t* buffer, CLP_Extended* ext, const uint8_t* data, size_t datalength);
size_t CLP_EXTENSION_ConvertPacketTimeSyncResponseToRaw(const uint8_t* packet, size_t packetlength, uint8_t* buffer, CLP_Extended* ext=0);
void CLP_EXTENSION_ConvertPacketTimeSyncResponseToRawOfSize(const uint8_t* packet, size_t datalength, uint8_t* buffer, CLP_Extended* ext=0);

}