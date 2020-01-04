#ifndef _BX4K_H
#define _BX4K_H

typedef unsigned char BYTE;

#define DATA_BIT 8
#define STOP_BIT 1

#define RESERVED_DEFAULT 0x00
#define FRAME_START_HALF 0xA5A5A5A5
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

//ProcessMode
#define PROCESS_MODE_APPEND 0x00
#define PROCESS_MODE_OVERWRITE 0x01

//RunMode
#define RUN_MODE_CYCLE 0x00
#define RUN_MODE_ITERATE_THEN_STAY_AT_LAST 0x01
#define RUN_MODE_CYCLE_AND_TIMEOUT 0x02
#define RUN_MODE_CYCLE_AND_TIMEOUT_TO_LAST 0x03
#define RUN_MODE_ITERATE 0x04
#define RUN_MODE_ITERATE_AND_TIMEOUT 0x05

//TextAlignment
#define TEXT_ALIGN_LEFT 0x00 //00 00
#define TEXT_ALIGN_RIGHT 0x01 //00 01
#define TEXT_ALIGN_HORIZONTAL_CENTER 0x02 //00 10
#define TEXT_ALIGN_UP 0x00 //00 00
#define TEXT_ALIGN_DOWN 0x04 //01 00
#define TEXT_ALIGN_VERTICAL_CENTER 0x08 //10 00

//SingleLine
#define LINE_SINGLE 0x01
#define LINE_MULTI 0x02

//NewLine
#define NEWLINE_MANUAL 0x01
#define NEWLINE_AUTO 0x02

//DisplayMode
#define DISPLAY_MODE_STATIC 0x01
#define DISPLAY_MODE_QUICK_TYPE 0x02
#define DISPLAY_MODE_MOVE_LEFT 0x03
#define DISPLAY_MODE_MOVE_RIGHT 0x04
#define DISPLAY_MODE_MOVE_UP 0x05
#define DISPLAY_MODE_MOVE_DOWN 0x06

//DisplaySpeed
#define DISPLAY_SPEED_FASTEST 0x00
#define DISPLAY_SPEED_SLOWEST 0x18

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

//LengthConstant
#define LEN_FRAME_START 8
#define LEN_FRAME_END 1
#define LEN_PACKAGE_HEADER 14
#define LEN_PACKAGE_DATA_MIN 5
#define LEN_PACKAGE_FOR_CRC_MIN 19
#define LEN_CRC 2
#define LEN_REALTIME_AREA_CONFIG_LEN 4
#define LEN_REALTIME_AREA_DATA_MIN 31
#define LEN_DISPLAY_DATA_LEN 4
#define LEN_FONT_CONFIG 7 // "\\FO000"

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

//Not to use now due to struct padding will add 0x0000 in middle of struct
//typedef struct {
//    BYTE areaType;
//    unsigned short areaX;
//    unsigned short areaY;
//    unsigned short areaWidth;
//    unsigned short areaHeight;
//    BYTE dynamicAreaLoc;
//    BYTE linesSizes;
//    BYTE runMode;
//    unsigned short timeout;
//    BYTE soundMode;
//    BYTE extendParaLen;
//    BYTE textAlignment;
//    BYTE singleLine;
//    BYTE newLine;
//    BYTE displayMode;
//    BYTE exitMode;
//    BYTE speed;
//    BYTE stayTime;
//    unsigned int dataLen;
//    const BYTE *data;
//} AreaData;

#define LANGUAGE_CHINESE 'O'
#define LANGUAGE_ENGLISH 'E'
#define LANGUAGE_OTHER 'K'
#define ENCODING_GB2312_ASCII '0'
#define ENCODING_GBK '1'
#define ENCODING_UNICODE '2'
#define FONT_SONG_TI '0' //宋体
#define FONT_KAI_TI '1' //楷体
#define FONT_HEI_TI '2' //黑体
#define FONT_FANG_SONG '3' //仿宋
#define FONT_MICROSOFT_RUAN_HEI '4' //微软雅黑
#define FONT_SIZE_16 '0'
#define FONT_SIZE_24 '1'
#define FONT_SIZE_32 '2'

typedef struct {
    BYTE language;
    BYTE encoding;
    BYTE fontFamily;
    BYTE fontSize;
} BX5KFontConfig;

typedef struct {
    int len;
    const BYTE *data;
} ByteArray;

unsigned short calcCRC(ByteArray arr);
unsigned short littleEndian(unsigned short bits);
ByteArray escape(ByteArray arr);
ByteArray unescape(ByteArray arr);
ByteArray genFrame(BX4KPackageData packageData, int dataLength);
//ByteArray wrapText(ByteArray arr, BX5KFontConfig fontConfig);

void init(void);
ByteArray ping(void);
ByteArray queryStatus(void);
ByteArray display(ByteArray, BYTE[4]);
ByteArray clearScreen(void);

int parseResponse(ByteArray);

#endif
