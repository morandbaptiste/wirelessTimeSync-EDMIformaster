#include "clp_api.h"
#include "endian.h"
#include "crc.h"
#include <string.h>

// Global value to store last error code
int CLP_G_ERROR_VAL = CLP_LIB_ERROR_NONE;

inline float CLP_BUFFER_TO_FLOAT(uint8_t* b)
{
  uint32_t u = _byteswap_ulong(*(uint32_t*)b);
  return *(float*)&u;
}

CLP_Extended::CLP_Extended()
  : dest(0), src(0), frame(0)
{
  
}

CLP_Extended::CLP_Extended(uint32_t _dest, uint32_t _src, uint16_t _frame)
  : dest(_dest), src(_src), frame(_frame)
{
  
}

CLP_Extended::CLP_Extended(const CLP_Extended& other)
  : dest(other.dest), src(other.src), frame(other.frame)
{
  
}

CLP_FileAccess::CLP_FileAccess()
  : type(0), opt(0), start(0), num(0)
{
  
}

CLP_FileAccess::CLP_FileAccess(uint8_t _type, uint8_t _opt, uint32_t _start, uint16_t _num)
  : type(_type), opt(_opt), start(_start), num(_num)
{
  
}

CLP_FileAccess::CLP_FileAccess(const CLP_FileAccess& other)
  : type(other.type), opt(other.opt), start(other.start), num(other.num)
{
  
}

CLP_Information::CLP_Information()
  : type(CLP_REG_TYPE_NONE), unit(CLP_REG_UNIT_NO_UNIT)
{
  strcpy(desc,"Register xxxx");
}

CLP_Information::CLP_Information(uint8_t _type, uint8_t _unit, const char* _desc)
  : type(_type), unit(_unit)
{
  if(!_desc)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_STRING_NOT_GIVEN;
    return;
  }

  size_t length = (strlen(_desc) < sizeof(desc)) ? strlen(_desc) : sizeof(desc);

  memcpy(desc,_desc,length);
}

CLP_Information::CLP_Information(const CLP_Information& other)
  : type(other.type), unit(other.unit)
{
  memcpy(desc,other.desc,sizeof(desc));
}

size_t CLP_GenerateLogonCommand(uint8_t* buffer, const char* user, const char* pass)
{
  if(!user || !pass)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_STRING_NOT_GIVEN;
    return 0;
  }

  size_t userlength = strlen(user) + 1;
  size_t passlength = strlen(pass) + 1;

  buffer[0] = CLP_COMMAND_LOGON;
  memcpy(buffer + 1, user, userlength);
  buffer[userlength] = ',';
  memcpy(buffer + userlength + 1, pass, passlength);

  return 1 + userlength + passlength;
}

size_t CLP_GenerateExitCommand(uint8_t* buffer)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_COMMAND_EXIT;
  buffer[1] = '\0';

  return 2;
}

size_t CLP_GenerateInformationCommand(uint8_t* buffer, uint16_t reg)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_COMMAND_INFORMATION;
  (*(uint16_t*)&buffer[1]) = _byteswap_ushort(reg);

  return 3;
}

size_t CLP_GenerateRegisterReadCommand(uint8_t* buffer, uint16_t reg)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_COMMAND_READ_REG;
  (*(uint16_t*)&buffer[1]) = _byteswap_ushort(reg);

  return 3;
}

size_t CLP_GenerateRegisterWriteCommand(uint8_t* buffer, uint16_t reg, uint8_t* data, size_t datalength)
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

  buffer[0] = CLP_COMMAND_WRITE_REG;
  (*(uint16_t*)&buffer[1]) = _byteswap_ushort(reg);

  if(data)
  {
    memcpy(buffer + 3, data, datalength);
  }

  return 3 + datalength;
}

size_t CLP_GenerateFileAccessCommand(uint8_t* buffer, CLP_FileAccess& file)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_COMMAND_FILE_ACCESS;
  buffer[1] = file.type;
  buffer[2] = file.opt;
  (*(uint32_t*)&buffer[3]) = _byteswap_ulong(file.start);
  (*(uint16_t*)&buffer[7]) = _byteswap_ushort(file.num);

  return 9;
}

size_t CLP_GenerateExtendedCommand(uint8_t* buffer, const CLP_Extended &ext)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_COMMAND_EXTENDED;
  (*(uint32_t*)&buffer[1]) = _byteswap_ulong(ext.dest);
  (*(uint32_t*)&buffer[5]) = _byteswap_ulong(ext.src);
  (*(uint16_t*)&buffer[9]) = _byteswap_ushort(ext.frame);

  return 11;
}

size_t CLP_GenerateEscape(uint8_t* buffer, size_t length)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  size_t count = 0;

/*
  for(size_t i = 0; i < length; ++i)
  {
    switch(buffer[i])
    {
      case CLP_ESCAPE_STX:
      case CLP_ESCAPE_ETX:
      case CLP_ESCAPE_XON:
      case CLP_ESCAPE_XOFF:
      case CLP_ESCAPE_DLE:  ++count; break;
      default: break;
    }
  }
*/

  uint8_t temp[CLP_UTILITY_MAX_PACKET_SIZE];
//  uint8_t* temp = new uint8_t[length + count];
//  count = 0;

  for(size_t i = 0; i < length; ++i)
  {
    switch(buffer[i])
    {
      case CLP_ESCAPE_STX:
      case CLP_ESCAPE_ETX:
      case CLP_ESCAPE_XON:
      case CLP_ESCAPE_XOFF:
      case CLP_ESCAPE_DLE:
      {
        temp[i + count] = CLP_ESCAPE_DLE;
        ++count;
        temp[i + count] = buffer[i] + 0x40;
      }
      break;
      default: temp[i + count] = buffer[i];
    }
  }

  memcpy(buffer, temp, length + count);
//  delete[] temp;

  return length + count;
}

size_t CLP_GeneratePacketEscape(const uint8_t* buffer, size_t length, uint8_t* packet)
{
  if(!buffer || !packet)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  size_t count = 0;

  for(size_t i = 0; i < length; ++i)
  {
    switch(buffer[i])
    {
      case CLP_ESCAPE_STX:
      case CLP_ESCAPE_ETX:
      case CLP_ESCAPE_XON:
      case CLP_ESCAPE_XOFF:
      case CLP_ESCAPE_DLE:
      {
        packet[i + count] = CLP_ESCAPE_DLE;
        ++count;
        packet[i + count] = buffer[i] + 0x40;
      }
      break;
      default: packet[i + count] = buffer[i];
    }
  }

  return length + count;
}

size_t CLP_GeneratePacket(uint8_t* buffer, const uint8_t* data, size_t datalength)
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

  buffer[0] = CLP_ESCAPE_STX;
  size_t length = 1;
  if(data)
  {
    memcpy(buffer + length, data, datalength);
  }
  length += datalength;
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  buffer[length] = crc & 255;
  buffer[length+1] = (crc >> 8) & 255;
 //(*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateExtendedPacket(uint8_t* buffer, const CLP_Extended& ext, const uint8_t* data, size_t datalength)
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

  buffer[0] = CLP_ESCAPE_STX;
  size_t length = 1;
  length += CLP_GenerateExtendedCommand(buffer + length, ext);
  if(data)
  {
    memcpy(buffer + length, data, datalength);
  }
  length += datalength;
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateLogonPacket(uint8_t* buffer, CLP_Extended* ext, const char* user, const char* pass)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  if(!user || !pass)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_STRING_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_ESCAPE_STX;
  size_t length = 1;
  if(ext != 0)
  {
    length += CLP_GenerateExtendedCommand(buffer + length, (*ext));
  }
  length += CLP_GenerateLogonCommand(buffer + length, user, pass);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateExitPacket(uint8_t* buffer, CLP_Extended* ext)
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
  length += CLP_GenerateExitCommand(buffer + length);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateInformationPacket(uint8_t* buffer, CLP_Extended* ext, uint16_t reg)
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
  length += CLP_GenerateInformationCommand(buffer + length, reg);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateRegisterReadPacket(uint8_t* buffer, CLP_Extended* ext, uint16_t reg)
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
  length += CLP_GenerateRegisterReadCommand(buffer + length, reg);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateRegisterWritePacket(uint8_t* buffer, CLP_Extended* ext, uint16_t reg, uint8_t* data, size_t datalength)
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
  length += CLP_GenerateRegisterWriteCommand(buffer + length, reg, data, datalength);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_GenerateFileAccessPacket(uint8_t* buffer, CLP_Extended* ext, CLP_FileAccess& file)
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
  length += CLP_GenerateFileAccessCommand(buffer + length, file);
  uint16_t crc = CalculateBufferCRC16(buffer, length, 0x0000);
  length = CLP_GenerateEscape(buffer + 1, length);

  buffer[length + 2] = CLP_ESCAPE_ETX;
  (*(uint16_t*)&buffer[length]) = _byteswap_ushort(crc);
  length += 3;

  return length;
}

size_t CLP_ParseEscape(uint8_t* buffer, size_t length)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

//  uint8_t* temp = new uint8_t[length];
  uint8_t temp[CLP_UTILITY_MAX_PACKET_SIZE];
  size_t count = 0;

  for(size_t i = 0; i < length; ++i)
  {
    if(buffer[i] == CLP_ESCAPE_DLE)
    {
      temp[count] = buffer[++i] - 0x40;
    }
    else
    {
      temp[count] = buffer[i];
    }

    ++count;
  }

  memcpy(buffer, temp, count);
//  delete[] temp;

  return count;
}

size_t CLP_ParsePacketEscape(const uint8_t* packet, uint8_t* buffer)
{
  if(!packet || !buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  size_t count = 0;
  size_t i = 0;

  while(packet[i] != CLP_ESCAPE_ETX)
  {
    if(packet[i] == CLP_ESCAPE_DLE)
    {
      buffer[count] = packet[++i] - 0x40;
    }
    else
    {
      buffer[count] = packet[i];
    }

    ++i;
    ++count;
  }

  buffer[count] = packet[i];

  return count+1;
}

size_t CLP_GenerateACKResponse(uint8_t* buffer)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_RESPONSE_ACK;

  return 1;
}

size_t CLP_GenerateCANResponse(uint8_t* buffer, uint8_t error)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_RESPONSE_CAN;
  buffer[1] = error;

  return 2;
}

size_t CLP_GenerateInformationResponse(uint8_t* buffer, uint16_t reg, CLP_Information& info)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  buffer[0] = CLP_COMMAND_READ_REG;
  (*(uint16_t*)&buffer[1]) = _byteswap_ushort(reg);

  buffer[3] = info.type;
  buffer[4] = info.unit;

  memcpy(buffer + 5, info.desc, sizeof(info.desc));

  return 22;
}

size_t CLP_GenerateRegisterReadResponse(uint8_t* buffer, uint16_t reg, uint8_t* data, size_t datalength)
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

  buffer[0] = CLP_COMMAND_READ_REG;
  (*(uint16_t*)&buffer[1]) = _byteswap_ushort(reg);

  if(data)
  {
    memcpy(buffer + 3, data, datalength);
  }

  return 3 + datalength;
}

size_t CLP_GenerateFileAccessResponse(uint8_t* buffer, CLP_FileAccess& file, uint8_t* data, size_t datalength)
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

  buffer[0] = CLP_COMMAND_FILE_ACCESS;
  buffer[1] = file.type;
  buffer[2] = file.opt;
  (*(uint32_t*)&buffer[3]) = _byteswap_ulong(file.start);
  (*(uint16_t*)&buffer[7]) = _byteswap_ushort(file.num);

  if(data)
  {
    memcpy(buffer + 9, data, datalength);
  }

  return 9;
}

bool CLP_CheckPacketCRCFail(const uint8_t* packet, size_t length, bool escape)
{
  if(!packet)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }
  
  bool crcfail = false;

  if(escape)
  {
    uint8_t clpbuffer[CLP_UTILITY_MAX_PACKET_SIZE];
    size_t clplength = CLP_ParsePacketEscape(packet, clpbuffer);

    uint16_t crc = CalculateBufferCRC16(clpbuffer, clplength - 3, 0x0000);

    crcfail = (*(uint16_t*)&clpbuffer[clplength - 3]) != _byteswap_ushort(crc);
  }
  else
  {
    uint16_t crc = CalculateBufferCRC16(packet, length - 3, 0x0000);
	uint16_t packetcrc = (packet[length - 2] << 8) + packet[length - 3];
	crcfail = crc != packetcrc;
//    crcfail = (*(uint16_t*)&packet[length - 3]) != _byteswap_ushort(crc);
  }
  
  return crcfail;
}

const char* CLP_DecodeCANError(uint8_t error)
{
  switch(error)
  {
    case CLP_ERROR_CANNOT_WRITE: return "Cannot write";
    case CLP_ERROR_UNIMPLEMENTED: return "Unimplemented operation";
    case CLP_ERROR_REG_NOT_FOUND: return "Register not found";
    case CLP_ERROR_ACCESS_DENIED: return "Access denied";
    case CLP_ERROR_WRONG_LENGTH: return "Wrong length";
    case CLP_ERROR_BAD_TYPE_CODE: return "Bad type code";
    case CLP_ERROR_DATA_NOT_READY: return "Data not ready yet. Try again later.";
    case CLP_ERROR_OUT_OF_RANGE: return "Out of range. Written value was out of defined ranges.";
    case CLP_ERROR_NOT_LOGGED_IN: return "Not logged in.";
    default: return '\0';
  };
}

time_t CLP_ConvertMeterTimeToCTime(uint32_t metertime)
{
  time_t epoch96 = 0;
  tm* date = gmtime(&epoch96);
  date->tm_year = 96;
  epoch96 = mktime(date);

  return epoch96 + metertime;
}

uint32_t CLP_ConvertCTimeToMeterTime(time_t time)
{
  time_t epoch96 = 0;
  tm* date = gmtime(&epoch96);
  date->tm_year = 96;
  epoch96 = mktime(date);

  return (uint32_t)time - epoch96;
}

void CLP_ConvertHexTimeToTM(const uint8_t* buffer, tm* time)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
  }

  if(!time)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
  }

  if(time && buffer)
  {
    time->tm_hour = buffer[0];
    time->tm_min = buffer[1];
    time->tm_sec = buffer[2];
  }
}

void CLP_ConvertTMToHexTime(uint8_t* buffer, const tm* time)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
  }

  if(!time)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
  }

  if(time && buffer)
  {
    buffer[0] = time->tm_hour;
    buffer[1] = time->tm_min;
    buffer[2] = time->tm_sec;
  }
}

void CLP_ConvertHexDateToTM(const uint8_t* buffer, tm* time)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
  }

  if(!time)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
  }

  if(time && buffer)
  {
    time->tm_mday = buffer[0];
    time->tm_mon = buffer[1] - 1;
    time->tm_year = buffer[2] + 100;
  }
}

void CLP_ConvertTMToHexDate(uint8_t* buffer, const tm* time)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
  }

  if(!time)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
  }

  if(time && buffer)
  {
    buffer[0] = time->tm_mday;
    buffer[1] = time->tm_mon + 1;
    buffer[2] = time->tm_year - 100;
  }
}

void CLP_ConvertHexTimeDateToTM(const uint8_t* buffer, tm* time)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
  }

  if(!time)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
  }

  if(time && buffer)
  {
    CLP_ConvertHexDateToTM(buffer, time);
    CLP_ConvertHexTimeToTM(buffer + 3, time);
  }
}

void CLP_ConvertTMToHexTimeDate(uint8_t* buffer, const tm* time)
{
  if(!buffer)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
  }

  if(!time)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
  }

  if(time && buffer)
  {
    CLP_ConvertTMToHexDate(buffer, time);
    CLP_ConvertTMToHexTime(buffer + 3, time);
  }
}

bool CLP_IsPacket(const uint8_t* packet)
{
  if(!packet)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return false;
  }

  return (packet[0] == CLP_ESCAPE_STX);
}

bool CLP_IsExtendedPacket(const uint8_t* packet)
{
  if(!packet)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return false;
  }

  return (packet[1] == CLP_COMMAND_EXTENDED);
}

bool CLP_IsPacketType(const uint8_t* packet, uint8_t type)
{
  if(!CLP_IsPacket(packet)) return false;

  bool isext = CLP_IsExtendedPacket(packet);
  if(isext && (type == CLP_COMMAND_EXTENDED)) return true;

  size_t start = isext ? 12 : 1;

  return (packet[start] == type);
}

/*
size_t CLP_FindEndOfPacket(const uint8_t* packet, size_t length)
{
  if(!CLP_IsPacket(packet)) return 0;

  size_t count = 1;
  while(packet[count] != CLP_ESCAPE_ETX)
  {
    if((length != 0) && (count >= length))
    {
      return 0;
    }

    ++count;
  }

  return count + 1;
}

size_t CLP_GenerateParsedEscapePacket(const uint8_t* packet, uint8_t* &buffer)
{
  if(!packet)
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_BUFFER_NOT_GIVEN;
    return 0;
  }

  size_t end = CLP_FindEndOfPacket(packet);
  if(end == 0) return 0;

  buffer = new uint8_t[end];
  memcpy(buffer, packet, end);
  end = CLP_ParseEscape(buffer, end);

  return end;
}

void CLP_DeletePacket(uint8_t* &packet)
{
  if(packet)
  {
    delete [] packet;
    packet = 0;
  }
}
*/

uint16_t CLP_GetRegisterPacketAddress(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if((packet[start] == CLP_COMMAND_INFORMATION) || (packet[start] == CLP_COMMAND_READ_REG) || (packet[start] == CLP_COMMAND_WRITE_REG))
  {
    return _byteswap_ushort(*(uint16_t*)&packet[start + 1]);
  }

  return 0;
}

void CLP_ConvertPacketExtendedToValues(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return;

  if((ext != 0) && CLP_IsExtendedPacket(packet))
  {
    ext->dest = _byteswap_ulong(*(uint32_t*)&packet[2]);
    ext->src = _byteswap_ulong(*(uint32_t*)&packet[6]);
    ext->frame = _byteswap_ushort(*(uint16_t*)&packet[10]);
  }
}

bool ConvertPacketACKResponseToFlag(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return false;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  return (packet[start] == CLP_RESPONSE_ACK);
}

uint8_t CLP_ConvertPacketCANResponseToError(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_RESPONSE_CAN)
  {
    return packet[start + 1];
  }

  return 0;
}

/*CLP_Information*/void CLP_ConvertPacketInformationResponseToValues(const uint8_t* packet, CLP_Information* info, CLP_Extended* ext)
{
//  CLP_Information info;

  if(!CLP_IsPacket(packet)) return; //info;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_COMMAND_INFORMATION)
  {
    if(info != 0)
    {
      info->type = packet[start + 3];
      info->unit = packet[start + 4];
      strcpy(info->desc, (const char*)packet + start + 5);
    }
  }
  else
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_INVALID_REG_TYPE;
  }

//  return info;
}

void CLP_ConvertPacketRegisterReadResponseToRaw(const uint8_t* packet, size_t packetlength, uint8_t* buffer, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_COMMAND_READ_REG)
  {
    if(buffer)
    {
//      size_t end = CLP_FindEndOfPacket(packet);
//      if(end == 0) return;

      start += 3;

      size_t datalength = packetlength - 3 - start;
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

void CLP_ConvertPacketRegisterReadResponseToRawOfSize(const uint8_t* packet, size_t datalength, uint8_t* buffer, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_COMMAND_READ_REG)
  {
    if(buffer)
    {
      start += 3;
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

void CLP_ConvertPacketRegisterReadResponseToCString(const uint8_t* packet, char* buffer, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_COMMAND_READ_REG)
  {
    strcpy(buffer, (const char*)packet + start + 3);
  }
  else
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_INVALID_REG_TYPE;
  }
}

bool CLP_ConvertPacketRegisterReadResponseToBool(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return false;

  uint8_t temp;
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 1, &temp, ext);
  return (temp != 0);
}

uint8_t CLP_ConvertPacketRegisterReadResponseToByte(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0;

  uint8_t temp;
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 1, &temp, ext);
  return temp;
}

float CLP_ConvertPacketRegisterReadResponseToFloat(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0.0f;

//  float temp;
  uint8_t temp[4];
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 4, (uint8_t*)&temp, ext);
//  uint32_t i = _byteswap_ulong(*(uint32_t*)&temp);
//  temp = *(float*)&i;
//  return temp;
  return CLP_BUFFER_TO_FLOAT(temp);
}

uint16_t CLP_ConvertPacketRegisterReadResponseToShort(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0;

  uint16_t temp;
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 2, (uint8_t*)&temp, ext);
  return _byteswap_ushort(temp);
}

uint32_t CLP_ConvertPacketRegisterReadResponseToLong(const uint8_t* packet, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return 0;

  uint32_t temp;
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 4, (uint8_t*)&temp, ext);
  return _byteswap_ulong(temp);
}

void ConvertPacketRegisterReadResponseToTime(const uint8_t* packet, tm* time, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet) || !time) return;

  uint8_t buffer[3];
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 3, buffer, ext);
  CLP_ConvertHexTimeToTM(buffer, time);
}

void CLP_ConvertPacketRegisterReadResponseToDate(const uint8_t* packet, tm* time, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet) || !time) return;

  uint8_t buffer[3];
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 3, buffer, ext);
  CLP_ConvertHexDateToTM(buffer, time);
}

void CLP_ConvertPacketRegisterReadResponseToTimeDate(const uint8_t* packet, tm* time, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet) || !time) return;

  uint8_t buffer[6];
  CLP_ConvertPacketRegisterReadResponseToRawOfSize(packet, 6, buffer, ext);
  CLP_ConvertHexTimeDateToTM(buffer, time);
}

uint32_t CLP_ConvertPacketRegisterReadResponseToMeterTime(const uint8_t* packet, CLP_Extended* ext)
{
  return CLP_ConvertPacketRegisterReadResponseToLong(packet, ext);
}

time_t CLP_ConvertPacketRegisterReadResponseToCTime(const uint8_t* packet, CLP_Extended* ext)
{
  return CLP_ConvertMeterTimeToCTime(CLP_ConvertPacketRegisterReadResponseToLong(packet, ext));
}

void CLP_ConvertPacketFileAccessResponseHeaderToValues(const uint8_t* packet, CLP_FileAccess* file, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return; //file;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_COMMAND_FILE_ACCESS)
  {
    if(file != 0)
    {
      file->type = packet[start + 1];
      file->opt = packet[start + 2];
      file->start = _byteswap_ulong(*(uint32_t*)&packet[start + 3]);
      file->num = _byteswap_ushort(*(uint16_t*)&packet[start + 7]);
    }
    else
    {
      CLP_G_ERROR_VAL = CLP_LIB_ERROR_PARAM_NOT_GIVEN;
    }
  }
  else
  {
    CLP_G_ERROR_VAL = CLP_LIB_ERROR_INVALID_REG_TYPE;
  }
}

/*CLP_FileAccess*/void CLP_ConvertPacketFileAccessResponseToRaw(const uint8_t* packet, size_t packetlength, uint8_t* buffer, CLP_FileAccess* file, CLP_Extended* ext)
{
//  CLP_FileAccess file;

  if(!CLP_IsPacket(packet)) return; //file;

  size_t start = CLP_IsExtendedPacket(packet) ? 12 : 1;

  if(ext != 0)
  {
    CLP_ConvertPacketExtendedToValues(packet, ext);
  }

  if(packet[start] == CLP_COMMAND_FILE_ACCESS)
  {
    if(file != 0)
    {
      file->type = packet[start + 1];
      file->opt = packet[start + 2];
      file->start = _byteswap_ulong(*(uint32_t*)&packet[start + 3]);
      file->num = _byteswap_ushort(*(uint16_t*)&packet[start + 7]);
    }

    start =+ 9;

    if(buffer)
    {
//      size_t end = CLP_FindEndOfPacket(packet);
//      if(end == 0) return; //file;

      memcpy(buffer, packet + start, packetlength - start - 3);
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

//  return file;
}

void CLP_ConvertPacketSpecialF530ResponseToValues(const uint8_t* packet, size_t packetlength, CLP_Special_F530& data, CLP_Extended* ext)
{
  if(!CLP_IsPacket(packet)) return;

  uint8_t buffer[200];

  CLP_ConvertPacketRegisterReadResponseToRaw(packet, packetlength, buffer, ext);

  size_t pos = 0;
  
  data.voltage_phase_a = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.voltage_phase_b = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.voltage_phase_c = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.current_phase_a = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.current_phase_b = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.current_phase_c = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_a = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_b = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_c = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_va_ia = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_va_ib = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_va_ic = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_va_va = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_va_vb = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.angle_phase_va_vc = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.watts_phase_a = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.watts_phase_b = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.watts_phase_c = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.vars_phase_a = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.vars_phase_b = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.vars_phase_c = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.va_phase_a = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.va_phase_b = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.va_phase_c = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);
  data.frequency = CLP_BUFFER_TO_FLOAT(buffer + pos);
  pos += sizeof(uint32_t);

  data.button_jumper_state = buffer[pos++];
  data.elements = buffer[pos++];

  memcpy(data.active_efa, buffer + pos, 17);
  pos += 17;
  memcpy(data.latched_efa, buffer + pos, 17);
  pos += 17;

  data.time_date = _byteswap_ulong(*(uint32_t*)(buffer + pos));
  pos += sizeof(uint32_t);

  memcpy(data.relay_status, buffer + pos, 3);
  pos += 3;
  memcpy(data.relay_hardware, buffer + pos, 3);
  pos += 3;
  memcpy(data.lcd_map, buffer + pos, 20);
  pos += 20;
  memcpy(data.output_state, buffer + pos, 16);
  pos += 16;
}


int CLP_GetLastError()
{
  return CLP_G_ERROR_VAL;
}
