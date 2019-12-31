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
    verifyAndPrint("Display", display(arr, NULL), littleEndian(0x5EB9));
    
    //============ Actual Usage ============
    
    //=== Initialize serial port ===
    int port = 4;
    int ret = initialize(port, B57600, P_NONE | BIT_8 | STOP_1);
    if(ret != SIO_OK) {
        printf("Serial port initialize failed, errCode is %d", ret);
        return 1;
    }
    
    //=== Generate display command ===
    BYTE chStr[] = {
        0xB6, 0xF5, 'A', 'C', 'D', '3', '4', '5', 0xB3, 0xAC, 0xD6, 0xD8 //GB2312编码： 鄂ACD345超重
        //'\\', 'n', //换行
        //0xB6, 0xF5, 0x42, 0x44, 0x43, 0x35, 0x33, 0x34, 0xB3, 0xAC, 0xD6, 0xD8
    };
    ByteArray ch = {
        sizeof(chStr) / sizeof(BYTE),
        chStr
    };
    //区域长宽配置
    BYTE areaCustomConfig[4] = {
        0x28, 0x00, //宽度 单位8像素点 小端法 0x0028 = (2*16+8*1)*8 = 320
        0xA0, 0x00, //高度 单位1像素点 小端法 0x00A0 = (10*16+0*1)*1 = 160
    };
    ByteArray displayCommand = display(ch, areaCustomConfig);
    
    //=== Send command through serial port ===
    ret = sendCommand(port, clearScreen()); //清空屏幕
    
    //For test
    if(ret >= 0) {
        printf("ClearScreen success, data length: %d\n", ret);
    } else {
        printf("ClearScreen error: %d\n", ret);
    }
    
    ret = sendCommand(port, displayCommand);
    
    if(ret >= 0) {
        printf("SendCommand success, data length: %d\n", ret);
    } else {
        printf("SendCommand error: %d\n", ret);
    }
    
    close(port);
    
    return 0;
}
