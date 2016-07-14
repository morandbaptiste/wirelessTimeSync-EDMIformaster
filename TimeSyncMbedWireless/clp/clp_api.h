#pragma once

extern "C"
{

#include <stddef.h>
#include <stdint.h>
#include <time.h>

// CLP Control Values
//   Start character
#define CLP_ESCAPE_STX 0x02
//   End character
#define CLP_ESCAPE_ETX 0x03
//   Serial control
#define CLP_ESCAPE_XON 0x11
#define CLP_ESCAPE_XOFF 0x13
//   Escape character
#define CLP_ESCAPE_DLE 0x10

// CLP Command types
#define CLP_COMMAND_NONE '\0'
#define CLP_COMMAND_LOGON 'L'
#define CLP_COMMAND_SECURELOGON 'K'
#define CLP_COMMAND_EXIT 'X'
#define CLP_COMMAND_INFORMATION 'I'
#define CLP_COMMAND_READ_REG 'R'
#define CLP_COMMAND_WRITE_REG 'W'
#define CLP_COMMAND_FILE_ACCESS 'F'
#define CLP_COMMAND_EXTENDED 'E'

// CLP Response types
#define CLP_RESPONSE_ACK '\x06'
#define CLP_RESPONSE_CAN '\x18'

// CLP Error types
#define CLP_ERROR_CANNOT_WRITE '\x01'
#define CLP_ERROR_UNIMPLEMENTED '\x02'
#define CLP_ERROR_REG_NOT_FOUND '\x03'
#define CLP_ERROR_ACCESS_DENIED '\x04'
#define CLP_ERROR_WRONG_LENGTH '\x05'
#define CLP_ERROR_BAD_TYPE_CODE '\x06'
#define CLP_ERROR_DATA_NOT_READY '\x07'
#define CLP_ERROR_OUT_OF_RANGE '\x08'
#define CLP_ERROR_NOT_LOGGED_IN '\x09'

// CLP File Access values
//   Download types
#define CLP_FILE_LOAD_SURVEY_0 0
#define CLP_FILE_LOAD_SURVEY_1 1
#define CLP_FILE_EVENT_LOG_0   2
#define CLP_FILE_EVENT_LOG_1   3
#define CLP_FILE_EVENT_LOG_2   4
#define CLP_FILE_EVENT_LOG_3   5
#define CLP_FILE_EVENT_LOG_4   6
//   Options
#define CLP_FILE_OPTIONS_NONE  0
//     Use a time instead of start position
#define CLP_FILE_OPTIONS_TIME  1
//   Record positions
#define CLP_FILE_START_RECORD  0
//   Maximum number of records in a packet
#define CLP_FILE_MAX_RECORD    0xFFFF

// CLP Information Register Type values
#define CLP_REG_TYPE_STRING 'A'
#define CLP_REG_TYPE_BOOLEAN 'B'
#define CLP_REG_TYPE_BYTE 'C'
#define CLP_REG_TYPE_FLOAT 'F'
#define CLP_REG_TYPE_SHORT 'I'
#define CLP_REG_TYPE_LONG 'L'
#define CLP_REG_TYPE_NONE 'N'
#define CLP_REG_TYPE_TIME 'Q'
#define CLP_REG_TYPE_DATE 'R'
#define CLP_REG_TYPE_SPECIAL 'S'
#define CLP_REG_TYPE_TIMEDATE 'T'
#define CLP_REG_TYPE_UNKNOWN 'U'

// CLP Information Register Unit values
#define CLP_REG_UNIT_AMPS 'A'
#define CLP_REG_UNIT_ANGLE_DEG 'D'
#define CLP_REG_UNIT_HZ 'H'
#define CLP_REG_UNIT_MINUITES 'M'
#define CLP_REG_UNIT_NO_UNIT 'N'
#define CLP_REG_UNIT_PERCENT 'P'
#define CLP_REG_UNIT_POWER_FACTOR 'Q'
#define CLP_REG_UNIT_VARS 'R'
#define CLP_REG_UNIT_VA 'S'
#define CLP_REG_UNIT_SECONDS 'T'
#define CLP_REG_UNIT_UNKNOWN 'U'
#define CLP_REG_UNIT_UNDEFINED CLP_REG_UNIT_UNKNOWN
#define CLP_REG_UNIT_VOLTS 'V'
#define CLP_REG_UNIT_WATTS 'W'
#define CLP_REG_UNIT_WH 'X'
#define CLP_REG_UNIT_VARH 'Y'
#define CLP_REG_UNIT_VAH 'Z'

// CLP Library errors
#define CLP_LIB_ERROR_NONE              0
#define CLP_LIB_ERROR_BUFFER_NOT_GIVEN  1
#define CLP_LIB_ERROR_STRING_NOT_GIVEN  2
#define CLP_LIB_ERROR_PARAM_NOT_GIVEN   3
#define CLP_LIB_ERROR_INVALID_REG_TYPE  4
#define CLP_LIB_ERROR_DATA_TOO_LONG     5

// CLP Utility values
#define CLP_UTILITY_MAX_PACKET_SIZE 300

// CLP Extended Command value wrapper
struct CLP_Extended
{
  CLP_Extended();
  CLP_Extended(uint32_t _dest, uint32_t _src, uint16_t _frame);
  CLP_Extended(const CLP_Extended& other);

  // Destination address
  uint32_t dest;
  // Source address
  uint32_t src;
  // Frame number
  uint16_t frame;
};

// CLP File Access command value wrapper
struct CLP_FileAccess
{
  CLP_FileAccess();
  CLP_FileAccess(uint8_t _type, uint8_t _opt, uint32_t _start, uint16_t _num);
  CLP_FileAccess(const CLP_FileAccess& other);

  // Download type
  uint8_t type;
  // Options
  uint8_t opt;
  // Start position
  uint32_t start;
  // Number of records
  uint16_t num;
};

// CLP Information response value wrapper
struct CLP_Information
{
  CLP_Information();
  CLP_Information(uint8_t _type, uint8_t _unit, const char* _desc);
  CLP_Information(const CLP_Information& other);

  // Register type
  uint8_t type;
  // Register unit
  uint8_t unit;
  // Register description
  char desc[16];
};

// Data layout for special register F530
struct CLP_Special_F530
{
  float voltage_phase_a;
  float voltage_phase_b;
  float voltage_phase_c;
  float current_phase_a;
  float current_phase_b;
  float current_phase_c;
  float angle_phase_a;
  float angle_phase_b;
  float angle_phase_c;
  float angle_phase_va_ia;
  float angle_phase_va_ib;
  float angle_phase_va_ic;
  float angle_phase_va_va;
  float angle_phase_va_vb;
  float angle_phase_va_vc;
  float watts_phase_a;
  float watts_phase_b;
  float watts_phase_c;
  float vars_phase_a;
  float vars_phase_b;
  float vars_phase_c;
  float va_phase_a;
  float va_phase_b;
  float va_phase_c;
  float frequency;
  uint8_t button_jumper_state;
  uint8_t elements;
  uint8_t active_efa[17];
  uint8_t latched_efa[17];
  // GMT time in internal meter format
  uint32_t time_date;
  uint8_t relay_status[3];
  uint8_t relay_hardware[3];
  uint8_t lcd_map[20];
  uint8_t output_state[16];
};

// Generate CLP Commands from values, return size of output command
size_t CLP_GenerateLogonCommand(uint8_t* buffer, const char* user, const char* pass);
//size_t CLP_GenerateSecureLogonCommand(uint8_t* buffer, const char* user, const char* pass);
size_t CLP_GenerateExitCommand(uint8_t* buffer);
size_t CLP_GenerateInformationCommand(uint8_t* buffer, uint16_t reg);
size_t CLP_GenerateRegisterReadCommand(uint8_t* buffer, uint16_t reg);
size_t CLP_GenerateRegisterWriteCommand(uint8_t* buffer, uint16_t reg, uint8_t* data, size_t datalength);
size_t CLP_GenerateFileAccessCommand(uint8_t* buffer, CLP_FileAccess& file);
size_t CLP_GenerateExtendedCommand(uint8_t* buffer, const CLP_Extended& ext);

// Generate CLP Responses from values, return size of output response
size_t CLP_GenerateACKResponse(uint8_t* buffer);
size_t CLP_GenerateCANResponse(uint8_t* buffer, uint8_t error);
//size_t CLP_GenerateSecureLogonResponse(uint8_t* buffer, uint32_t time, uint32_t serial);
size_t CLP_GenerateInformationResponse(uint8_t* buffer, uint16_t reg, CLP_Information& info);
size_t CLP_GenerateRegisterReadResponse(uint8_t* buffer, uint16_t reg, uint8_t* data, size_t datalength);
size_t CLP_GenerateFileAccessResponse(uint8_t* buffer, CLP_FileAccess& file, uint8_t* data, size_t datalength);

// Generate CLP Packets from values, return size of output packet
size_t CLP_GeneratePacket(uint8_t* buffer, const uint8_t* data, size_t datalength);
size_t CLP_GenerateExtendedPacket(uint8_t* buffer, const CLP_Extended &ext, const uint8_t* data, size_t datalength);
size_t CLP_GenerateLogonPacket(uint8_t* buffer, CLP_Extended* ext, const char* user, const char* pass);
//size_t CLP_GenerateSecureLogonPacket(uint8_t* buffer CLPExtended* ext, const char* user, const char* pass);
size_t CLP_GenerateExitPacket(uint8_t* buffer, CLP_Extended* ext);
size_t CLP_GenerateInformationPacket(uint8_t* buffer, CLP_Extended* ext, uint16_t reg);
size_t CLP_GenerateRegisterReadPacket(uint8_t* buffer, CLP_Extended* ext, uint16_t reg);
size_t CLP_GenerateRegisterWritePacket(uint8_t* buffer, CLP_Extended* ext, uint16_t reg, uint8_t* data, size_t datalength);
size_t CLP_GenerateFileAccessPacket(uint8_t* buffer, CLP_Extended* ext, CLP_FileAccess& file);

// Process CLP Escape sequences, return size of output packet
//   Output is written to input buffer
size_t CLP_GenerateEscape(uint8_t* buffer, size_t length);
size_t CLP_ParseEscape(uint8_t* buffer, size_t length);
//   Output is written to buffer
size_t CLP_GeneratePacketEscape(const uint8_t* buffer, size_t length, uint8_t* packet);
size_t CLP_ParsePacketEscape(const uint8_t* packet, uint8_t* buffer);

// Process CLP CRC, set escape to true to process escapes internally
bool CLP_CheckPacketCRCFail(const uint8_t* packet, size_t length, bool escape=true);

// Decode CLP CAN Error to cstring, invalid returns an empty string
const char* CLP_DecodeCANError(uint8_t error);

// Convert EDMI times to ctimes
time_t CLP_ConvertMeterTimeToCTime(uint32_t metertime);
uint32_t CLP_ConvertCTimeToMeterTime(time_t time);
void CLP_ConvertHexTimeToTM(const uint8_t* buffer, tm* time);
void CLP_ConvertTMToHexTime(uint8_t* buffer, const tm* time);
void CLP_ConvertHexDateToTM(const uint8_t* buffer, tm* time);
void CLP_ConvertTMToHexDate(uint8_t* buffer, const tm* time);
void CLP_ConvertHexTimeDateToTM(const uint8_t* buffer, tm* time);
void CLP_ConvertTMToHexTimeDate(uint8_t* buffer, const tm* time);

// Packet utility
//   Get last error generated by the CLP library
int CLP_GetLastError();
//   Checks if packet starts with STX
bool CLP_IsPacket(const uint8_t* packet);
//   Checks if packet is extended
bool CLP_IsExtendedPacket(const uint8_t* packet);
//   Checks if packet is of the given type, will also check if extended
bool CLP_IsPacketType(const uint8_t* packet, uint8_t type);
//   Finds the size of a packet.
//   [Do not give this function a buffer without an ETX, with length=0 (infinate loop). Escape parsed packets may also get incorrect length]
//size_t CLP_FindEndOfPacket(const uint8_t* packet, size_t length=0);
//   Gnerates a packet with escaped values resolved. [Returned buffer must be deleted]
//size_t CLP_GenerateParsedEscapePacket(const uint8_t* packet, uint8_t* &buffer);
//   Clean up buffer memory, nulls pointer
//void CLP_DeletePacket(uint8_t* &packet);
//   Extract register address from commands/responses that use registers, packet must have escape values processed before calling
uint16_t CLP_GetRegisterPacketAddress(const uint8_t* packet, CLP_Extended* ext=0);

// Extract packet data, packet must be have escape values processed before calling
//   Extract Extended values
void CLP_ConvertPacketExtendedToValues(const uint8_t* packet, CLP_Extended* ext);
//   Check if packet is an ACK response
bool CLP_ConvertPacketACKResponseToFlag(const uint8_t* packet, CLP_Extended* ext=0);
//   Extract error value from a CAN response, if not a CAN response returns 0
uint8_t CLP_ConvertPacketCANResponseToError(const uint8_t* packet, CLP_Extended* ext=0);
//   Extract register information values
void CLP_ConvertPacketInformationResponseToValues(const uint8_t* packet, CLP_Information* info, CLP_Extended* ext=0);
//   Extract register values
void CLP_ConvertPacketRegisterReadResponseToRaw(const uint8_t* packet, size_t packetlength, uint8_t* buffer, CLP_Extended* ext=0);
void CLP_ConvertPacketRegisterReadResponseToRawOfSize(const uint8_t* packet, size_t datalength, uint8_t* buffer, CLP_Extended* ext=0);
void CLP_ConvertPacketRegisterReadResponseToCString(const uint8_t* packet, char* buffer, CLP_Extended* ext=0);
bool CLP_ConvertPacketRegisterReadResponseToBool(const uint8_t* packet, CLP_Extended* ext=0);
uint8_t CLP_ConvertPacketRegisterReadResponseToByte(const uint8_t* packet, CLP_Extended* ext=0);
float CLP_ConvertPacketRegisterReadResponseToFloat(const uint8_t* packet, CLP_Extended* ext=0);
uint16_t CLP_ConvertPacketRegisterReadResponseToShort(const uint8_t* packet, CLP_Extended* ext=0);
uint32_t CLP_ConvertPacketRegisterReadResponseToLong(const uint8_t* packet, CLP_Extended* ext=0);
void CLP_ConvertPacketRegisterReadResponseToTime(const uint8_t* packet, tm* time, CLP_Extended* ext=0);
void CLP_ConvertPacketRegisterReadResponseToDate(const uint8_t* packet, tm* time, CLP_Extended* ext=0);
void CLP_ConvertPacketRegisterReadResponseToTimeDate(const uint8_t* packet, tm* time, CLP_Extended* ext=0);
uint32_t CLP_ConvertPacketRegisterReadResponseToMeterTime(const uint8_t* packet, CLP_Extended* ext=0);
time_t CLP_ConvertPacketRegisterReadResponseToCTime(const uint8_t* packet, CLP_Extended* ext=0);
//   Extract log file data
void CLP_ConvertPacketFileAccessResponseHeaderToValues(const uint8_t* packet, CLP_FileAccess* file, CLP_Extended* ext=0);
void CLP_ConvertPacketFileAccessResponseToRaw(const uint8_t* packet, uint8_t* buffer, size_t packetlength, CLP_FileAccess* file=0, CLP_Extended* ext=0);
//   Extract data from special register F530 response packet
void CLP_ConvertPacketSpecialF530ResponseToValues(const uint8_t* packet, size_t packetlength, CLP_Special_F530& data, CLP_Extended* ext=0);

}