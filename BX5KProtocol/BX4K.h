#ifndef _BX4K_H
#define _BX4K_H

typedef unsigned char BYTE;

#define DATA_BIT 8
#define STOP_BIT 1

#define RESERVED_DEFAULT 0x00
#define FRAME_START 0xA5A5A5A5A5A5A5A5
#define FRAME_END 0x5A
#define MAX_CONTROLLER_RECEIVE_BUFFER 1024
#define PROTOCOL_DEFAULT 0x02

//Addr
#define ADDR_BROADCAST_TIMING 0xFFFF
#define ADDR_BROADCAST_SETTING 0xFFFE
#define ADDR_DST_DEFAULT 0x0001
#define ADDR_SRC_DEFAULT 0x8000

//DeviceType
#define DEVICE_WILDCARD 0xFE
#define DEVICE_BX5K1 0x51
#define DEVICE_BX5K2 0x58
#define DEVICE_BX5MK2 0x53
#define DEVICE_BX5MK1 0x54

//CmdError
#define ERR_NO 0x00 //No Error
#define ERR_OUTOFGROUP 0x01 //Command Group Error
#define ERR_NOCMD 0x02 //Command Not Found
#define ERR_BUSY 0x03 //The Controller is busy now
#define ERR_MEMORYVOLUME 0x04 //Out of the Memory Volume
#define ERR_CHECKSUM 0x05 //CRC16 Checksum Error
#define ERR_FILENOTEXIST 0x06 //File Not Exist
#define ERR_FLASH 0x07 //Flash Access Error
#define ERR_FILE_DOWNLOAD 0x08 //File Download Error
#define ERR_FILE_NAME 0x09 //Filename Error
#define ERR_FILE_TYPE 0x0A //File type Error
#define ERR_FILE_CRC16 0x0B //File CRC16 Error
#define ERR_FONT_NOT_EXIST 0x0C //Font Library Not Exist
#define ERR_FIRMWARE_TYPE 0x0D //Firmware Type Error (Check the controller type)
#define ERR_DATE_TIME_FORMAT 0x0E //Date Time format error
#define ERR_FILE_EXIST 0x0F //File Exist for File overwrite
#define ERR_FILE_BLOCK_NUM 0x10 //File block number error

//Response
#define CONTROLLER_MUST_RESPONSE 0x01
#define CONTROLLER_NO_RESPONSE 0x02

/*
涉及到的多字节参数，均以先低字节(LSB)后高字节(MSB)顺序发送，但是对于文件名和
控制器名称等字符串参数，发送时按顺序发送，如“P123”则先发送‘P’，最后发送‘3’。
*/

typedef struct
{
  unsigned short dstAddr;
  unsigned short srcAddr;
  BYTE reserved[6];
  BYTE deviceType;
  BYTE protocolVersion;
  unsigned short dataLen;
} BX4KPackageHeader;

typedef struct {
    BYTE cmdGroup;
    BYTE cmd;
    BYTE response;
    BYTE reserved[2];
    BYTE *data;
} BX4KPackageData;

#define LEN_FRAME_START 8
#define LEN_FRAME_END 1
#define LEN_PACKAGE_HEADER 14
#define LEN_PACKAGE_DATA_MIN 5
#define LEN_PACKAGE_FOR_CRC_MIN 19

typedef struct {
    BX4KPackageHeader header;
    BX4KPackageData data;
} BX4KPackageForCRC;

typedef struct {
    int len;
    BYTE *data;
} ByteArray;

unsigned short calcCRC(const BYTE *data, long size);
ByteArray escape(ByteArray arr);
ByteArray unescape(ByteArray arr);
ByteArray genFrame(BX4KPackageData packageData, int dataLength);

void init(void);
ByteArray ping(void);
ByteArray queryStatus(void);
//ByteArray display(char* message);

#endif
