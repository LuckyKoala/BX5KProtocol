//
//  main.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/24.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#include <stdio.h>
#include "BX4K.h"
#include "BXNKController.h"

void printForDebug(const char *name, ByteArray arr) {
    printf("%s: ", name);
    
    int i;
    for (i = 0; i < arr.len; i++) {
        printf("%02X ", arr.data[i]);
    }
    printf("\n\n");
}

void verifyAndPrint(const char *name, ByteArray arr, unsigned short crc) {
    printf("%s: ", name);
    
    BYTE *crcArr = (BYTE *) &crc;
    if (arr.data[arr.len-2] == crcArr[0]
        && arr.data[arr.len-3] == crcArr[1]) {
        printf("\nVerified.");
    } else {
        printf("\nVerify failed: %02X - %02X, %02X - %02X",
               crcArr[0], arr.data[arr.len-2],
               crcArr[1], arr.data[arr.len-1]);
    }
    
    int i;
    for (i = 0; i < arr.len; i++) {
        if (i%8 == 0) {
            printf("\n");
        }
        printf("%02X ", arr.data[i]);
    }
    printf("\n\n");
}

int main(int argc, const char * argv[]) {
    //============ Test cases ============
    
    printf("Verifing and printing BX5K protocols ... \n\n");
    init();
    verifyAndPrint("Ping", ping(), littleEndian(0x68F8));
    verifyAndPrint("QueryStatus", queryStatus(), littleEndian(0x2D40));
    verifyAndPrint("ClearScreen", clearScreen(), littleEndian(0x51F8));
    
    BYTE str[] = {
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
        0x51, 0x52, 0x53, 0x54, 0x54, 0x56, 0x27, 0x42
    };
    ByteArray arr = {
        sizeof(str) / sizeof(BYTE), str
    };
    verifyAndPrint("Display", display(arr, NULL), littleEndian(0xA87D));
    
    //============ Actual Usage ============
    
    //=== Initialize serial port ===
    int port = 4;
    int ret = initialize(port, B57600, P_NONE | BIT_8 | STOP_1);
    if(ret != SIO_OK) {
        printf("Serial port initialize failed, errCode is %d", ret);
        return 1;
    }
    
    //=== Generate display command ===
    BYTE chStr[] = "鄂AC3450超重";
    ByteArray ch = {
        (sizeof(chStr) / sizeof(BYTE)) - 1, //减去 \0
        chStr
    };
    //中文编码配置
    BX5KFontConfig fontConfig = {
        LANGUAGE_CHINESE, //中文
        ENCODING_UNICODE, //Unicode 编码
        FONT_SONG_TI,     //宋体
        FONT_SIZE_16      //字体大小16（最小）
    };
    //区域长宽配置
    BYTE areaCustomConfig[4] = {
        0x28, 0x00, //宽度 单位8像素点 小端法 0x0028 = (2*16+8*1)*8 = 320
        0xA0, 0x00, //高度 单位1像素点 小端法 0x00A0 = (10*16+0*1)*1 = 160
    };
    ByteArray displayCommand = display(wrapText(ch, fontConfig), areaCustomConfig);
    //printForDebug("协议内容(鄂AC3450超重)", displayCommand);
    
    //=== Send command through serial port ===
    ret = sendCommand(port, displayCommand);
    
    //For test
    if(ret >= 0) {
        printf("SendCommand success, data length: %d\n", ret);
    } else {
        printf("SendCommand error: %d\n", ret);
    }
    
    close(port);
    
    return 0;
}
