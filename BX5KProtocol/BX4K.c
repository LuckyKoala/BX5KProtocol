#import <stdio.h>
#import <string.h>
#import <stdlib.h>
#import "BX4K.h"

#define CRC(crc,byte) (((crc) >> 8 ) ^ tabel[((crc) ^ (unsigned int) (byte)) & 0XFF])
static const unsigned short tabel[256] = {
    0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
    0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
    0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
    0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
    0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
    0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
    0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
    0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
    0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
    0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
    0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
    0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
    0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
    0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
    0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
    0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
    0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
    0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
    0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
    0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
    0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
    0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
    0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
    0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
    0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
    0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
    0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
    0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
    0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
    0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
    0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
    0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040
};

unsigned short calcCRC(ByteArray arr) {
    long i;
    unsigned short crc = 0;
    for (i = 0; i < arr.len; i++) {
      crc = CRC(crc, arr.data[i]);
    }
    return crc;
}

int isLittleEndian = 1;
//Cached request constant
#define EMPTY { 0, NULL }
ByteArray PING_FRAME = EMPTY;
ByteArray QUERY_STATUS_FRAME = EMPTY;
ByteArray CLEAR_SCREEN_FRAME = EMPTY;

void init() {
    int i = 1;
    char *p = (char *)&i;
    if(*p == 1) {
        isLittleEndian = 1;
        //printf("Using Little Endian\n");
    } else {
        isLittleEndian = 0;
        //printf("Using Big Endian\n");
    }
}

//0x1234
unsigned short littleEndian(unsigned short bits) {
    if(isLittleEndian != 1) {
        return (bits>>8) | (bits<<8);
    } else {
        return bits;
    }
}

//0x12345678
unsigned int littleEndianInt(unsigned int bits) {
    if(isLittleEndian != 1) {
        //return (bits>>8) | (bits<<8);
        return (bits<<24) | (bits>>24)
        || (bits & 0xff00) << 8
        || (bits & 0xff0000) >> 8;
    } else {
        return bits;
    }
}

/*
Escape Rules:

0xA5 => 0xA6，0x02
0xA6 => 0xA6，0x01
0x5A => 0x5B，0x02
0x5B => 0x5B，0x01
*/
ByteArray escape(ByteArray arr) {
    //Find how many byte to add
    int escapeLen = 0;
    int i;
    for (i = 0; i < arr.len; i++) {
        if (arr.data[i] == 0xA5 || arr.data[i] == 0xA6
            || arr.data[i] == 0x5A || arr.data[i] == 0x5B) {
            escapeLen++;
        }
    }
    //Escape and realloc
    int newLen = arr.len + escapeLen;
    //TODO remember to free these memory allocated by hand !!
    BYTE *data = (BYTE *) malloc(sizeof(BYTE) * newLen);
    int index = 0; //Indicate where to put next byte
    for (i = 0; i < arr.len; i++) {
        switch (arr.data[i]) {
            case 0xA5:
                data[index] = 0xA6;
                data[index+1] = 0x02;
                index = index + 2;
                break;
            case 0xA6:
                data[index] = 0xA6;
                data[index+1] = 0x01;
                index = index + 2;
                break;
            case 0x5A:
                data[index] = 0x5B;
                data[index+1] = 0x02;
                index = index + 2;
                break;
            case 0x5B:
                data[index] = 0x5B;
                data[index+1] = 0x01;
                index = index + 2;
                break;
            default:
                data[index] = arr.data[i];
                index = index + 1;
                break;
        }
    }
    ByteArray newArr = { newLen, data };
    return newArr;
}

/*
Escape Rules:

0xA6，0x02 => 0xA5
0xA6，0x01 => 0xA6
0x5B，0x02 => 0x5A
0x5B，0x01 => 0x5B
*/
ByteArray unescape(ByteArray arr) {
  //Find how many byte to remove
  int unescapeLen = 0;
  int i;
  for (i = 0; i < arr.len; i++) {
      if (arr.data[i] == 0xA6 || arr.data[i] == 0x5B) {
          if (arr.data[i+1] == 0x02 || arr.data[i+1] == 0x01) {
              unescapeLen++;
              i = i + 1; //skip since next byte are already byte to be unescaped
          }
      }
  }
  //Escape and realloc
  int newLen = arr.len - unescapeLen;
  BYTE *data = (BYTE *) malloc(sizeof(BYTE) * newLen);
  int index = 0; //Indicate where to put next byte
  for (i = 0; i < arr.len; i++) {
      switch (arr.data[i]) {
          case 0xA6:
              if (arr.data[i+1] == 0x02) {
                  data[index] = 0xA5;
                  index = index + 1;
                  i = i + 1;
              } else if (arr.data[i+1] == 0x01) {
                  data[index] = 0xA6;
                  index = index + 1;
                  i = i + 1;
              } else {
                  data[index] = 0xA6;
                  index = index + 1;
              }
              break;
          case 0x5B:
              if (arr.data[i+1] == 0x02) {
                  data[index] = 0x5A;
                  index = index + 1;
                  i = i + 1;
              } else if (arr.data[i+1] == 0x01) {
                  data[index] = 0x5B;
                  index = index + 1;
                  i = i + 1;
              } else {
                  data[index] = 0xA6;
                  index = index + 1;
              }
              break;
          default:
              data[index] = arr.data[i];
              index = index + 1;
              break;
      }
  }
  ByteArray newArr = { newLen, data };
  return newArr;
}

//增加包头和CRC校验值
ByteArray genFrame(BX4KPackageData packageData, int dataLength) {
    unsigned short packageDataLen = LEN_PACKAGE_DATA_MIN + dataLength;
    //包头
    BX4KPackageHeader header = {
      littleEndian(ADDR_DST_DEFAULT), //屏地址 默认 0x0001
      littleEndian(ADDR_SRC_DEFAULT), //源地址 默认 0x8000
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //保留 默认 0x000000000000
      DEVICE_WILDCARD, //设备类型 通配符 0xFE
      PROTOCOL_DEFAULT, //协议版本号 默认 0x02
      littleEndian(packageDataLen) //数据长度
    };
    //合并包头和数据域，直接使用struct会因为packing产生多余的0
    int packageForCRCLen =LEN_PACKAGE_FOR_CRC_MIN+dataLength;
    BYTE *packageForCRCData = malloc(sizeof(BYTE) * packageForCRCLen);
    memcpy(packageForCRCData, (BYTE *) &header, sizeof(BYTE) * LEN_PACKAGE_HEADER);
    memcpy(packageForCRCData + LEN_PACKAGE_HEADER,
           (BYTE *) &packageData, sizeof(BYTE) * LEN_PACKAGE_DATA_MIN);
    memcpy(packageForCRCData + LEN_PACKAGE_HEADER + LEN_PACKAGE_DATA_MIN,
           packageData.data, sizeof(BYTE) * dataLength);
    ByteArray packageForCRC = { packageForCRCLen, packageForCRCData };
    //CRC包校验
    unsigned short crc = littleEndian(calcCRC(packageForCRC));
    //合并PHY1层数据
    int newLen = LEN_PACKAGE_HEADER + packageDataLen + LEN_CRC;
    BYTE *phy1 = realloc(packageForCRCData, (sizeof(BYTE) * newLen));
    memcpy(phy1 + packageForCRCLen, (BYTE *) &crc, sizeof(BYTE) * 2);
    ByteArray arr = { newLen, phy1 };
    //转义PHY1层数据
    ByteArray escapedArr = escape(arr);
    free((void *)arr.data);
    //增加帧头和帧尾
    int frameLen = newLen + LEN_FRAME_START + LEN_FRAME_END;
    BYTE *phy0 = malloc(sizeof(BYTE) * frameLen);
    unsigned long frameStart = FRAME_START;
    unsigned short frameEnd = FRAME_END;
    memcpy(phy0, &frameStart, sizeof(BYTE) * LEN_FRAME_START);
    memcpy(phy0 + (sizeof(BYTE) * LEN_FRAME_START), escapedArr.data,
           sizeof(BYTE) * escapedArr.len);
    memcpy(phy0 + (sizeof(BYTE) * LEN_FRAME_START) + (sizeof(BYTE) * escapedArr.len), &frameEnd, sizeof(unsigned short));
    free((void *) escapedArr.data);
    ByteArray frame = { frameLen, phy0 };

    return frame;
}

/*
 Example:
  A5 A5 A5 A5 A5 A5 A5 A5
  01 00 00 80 00 00 00 00
  00 00 FE 02 05 00 A2 00
  01 00 00 68 F8 5A
 */
ByteArray ping() {
    if (PING_FRAME.len == 0) {
        BX4KPackageData packageData = {
            0xA2, 0x00,
            CONTROLLER_MUST_RESPONSE,
            {0x00, 0x00}
        };
        PING_FRAME = genFrame(packageData, 0);
    }
    
    return PING_FRAME;
}

/*
 Example:
  A5 A5 A5 A5 A5 A5 A5 A5
  01 00 00 80 00 00 00 00
  00 00 FE 02 05 00 A1 02
  01 00 00 2D 40 5A
 */
ByteArray queryStatus() {
    if(QUERY_STATUS_FRAME.len == 0) {
        BX4KPackageData packageData = {
            0xA1, 0x02,
            CONTROLLER_MUST_RESPONSE,
            {0x00, 0x00}
        };
        QUERY_STATUS_FRAME = genFrame(packageData, 0);
    }
    
    return QUERY_STATUS_FRAME;
}

/*
 Example:
  A5 A5 A5 A5 A5 A5 A5 A5
  01 00 00 80 00 00 00 00
  00 00 FE 02 05 00 A3 10
  01 00 00 51 F8 5A
 */
ByteArray clearScreen() {
    if(CLEAR_SCREEN_FRAME.len == 0) {
        BX4KPackageData packageData = {
            0xA3, 0x10,
            CONTROLLER_MUST_RESPONSE,
            {0x00, 0x00}
        };
        CLEAR_SCREEN_FRAME = genFrame(packageData, 0);
    }
    
    return CLEAR_SCREEN_FRAME;
}

/*
 Example:
  A5 A5 A5 A5 A5 A5 A5 A5             -- 帧头
  01 00 00 80 00 00 00 00 00 00 FE 02 -- 设备信息及协议信息，基本固定
  32 00                               -- 数据长度
  A3 06                               -- 命令分组，序号 (发送实时显示区域数据)
  01                                  -- 必须回复
  2D                                  -- ProcessMode
  00                                  -- 保留
  00 01                               -- 不删除区域，更新一个区域
  29 00                               -- 区域0 数据长度 0x29 => 41
  00                                  -- 区域类型
  00 00 00 00                         -- 区域XY坐标
  18 00                               -- 区域宽度(8个像素点)  0x18*8 => 24*8 => 192
  20 00                               -- 区域高度                    0x20*1 => 32
  00                                  -- 动态区域编号
  00 00                               -- 行间距0，动态区循环显示
  02 00                               -- 动态区数据超时时间 2秒/次
  00                                  -- 不使能语音
  00 00                               -- 拓展位个数0，水平垂直居中
  02 02                               -- 多行显示，自动换行
  03 00 04 05                         -- 向左移动，默认退出，速度0x04，特技停留2.5秒
  0E 00 00 00                         -- 数据长度 0x0E => 14
  30 31 32 33 34 35                   -- 数据 => "012345"
  51 52 53 54 54 56 27 42             -- 数据 => "QRSTTV’B"
  A8 7D                               -- CRC16 校验值
  5A                                  -- 帧尾
 */
ByteArray display(ByteArray arr) {
    int newDataLen = LEN_REALTIME_AREA_DATA_MIN + arr.len;
    BYTE areaData[] = {
        0x00, //区域类型
        0x00, 0x00, //区域 X 坐标，默认以字节(8 个像素点)为单位 高字节最高位为 1 时，表示以像素点为单位
        0x00, 0x00, //区域 Y 坐标，以像素点为单位
        0x18, 0x00, //区域宽度，默认以字节(8 个像素点)为单位 高字节最高位为 1 时，表示以像素点为单位
        0x20, 0x00, //区域高度，以像素点为单位
        0x00, //动态区域编号，从0开始递增 - 第一个动态区域
        0x00, //行间距
        RUN_MODE_CYCLE, //运行模式
        0x02, 0x00, //动态区数据超时时间
        0x00, //是否使能语音播放，仅 5K1Q-YY/6K-YY 有效
        RESERVED_DEFAULT, //拓展位个数
        TEXT_ALIGN_UP | TEXT_ALIGN_LEFT, //字体对齐方式
        LINE_MULTI, //是否单行显示
        NEWLINE_AUTO, //是否自动换行
        DISPLAY_MODE_MOVE_LEFT, //显示方式
        0x00, //退出方式
        0x04, //显示速度
        0x05, //显示特技停留时间，单位0.5秒 -- 停留0秒
        //字符串长度
        //0x00000000
    };
    
    BYTE *realtimeDataField = malloc(newDataLen * sizeof(BYTE));
    realtimeDataField[0] = 0x00; //删除区域个数 0
    realtimeDataField[1] = 0x01; //更新区域个数 1
    unsigned short areaDataLen = littleEndian(newDataLen - LEN_REALTIME_AREA_CONFIG_LEN);
    memcpy(realtimeDataField + 2, &areaDataLen, sizeof(unsigned short)); //区域数据长度
    memcpy(realtimeDataField + 4, areaData,
           (LEN_REALTIME_AREA_DATA_MIN - LEN_DISPLAY_DATA_LEN - 4) * sizeof(BYTE)); //拷贝除了显示数据和对应长度的区域数据
    unsigned int displayDataLen = littleEndianInt(arr.len);
    memcpy(realtimeDataField + LEN_REALTIME_AREA_DATA_MIN - LEN_DISPLAY_DATA_LEN,
           (BYTE *) &displayDataLen, sizeof(unsigned int)); //拷贝显示数据长度
    memcpy(realtimeDataField + LEN_REALTIME_AREA_DATA_MIN, arr.data,
           arr.len * sizeof(BYTE)); //拷贝显示数据

    BX4KPackageData packageData = {
        0xA3, 0x06,
        CONTROLLER_MUST_RESPONSE,
        //{PROCESS_MODE_OVERWRITE, 0x00},
        {0x2D, 0x00},
        realtimeDataField
    };
    return genFrame(packageData, newDataLen);
}
