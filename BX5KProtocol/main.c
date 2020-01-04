//
//  main.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/24.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#include <stdio.h>
#include "BXNKController.h"
#include "ReferenceCount.h"

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

void testProtocol() {
    printf("Verifing and printing BX5K protocols ... \n\n");
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
    ByteArray displayProtocol = display(arr, NULL);
    verifyAndPrint("Display", displayProtocol, littleEndian(0x5EB9));
    rc_free((void *) displayProtocol.data);
    displayProtocol.data = NULL;
}

int main(int argc, const char * argv[]) {
    //============ Test cases ============
    testProtocol();
    
    //============ Actual Usage ============
    
    //=== 初始化 ===
    int port = 4;
    int ret = initialize(port, B57600, P_NONE | BIT_8 | STOP_1);
    if(ret != SIO_OK) {
        printf("Serial port initialize failed, errCode is %d", ret);
        return 1;
    }
    
    //=== 展示字符串 ===
    BYTE chStr[] = {
        0xB6, 0xF5, 'A', 'C', 'D', '3', '4', '5', 0xB3, 0xAC, 0xD6, 0xD8 //GB2312编码： 鄂ACD345超重
        //'\\', 'n', //换行
        //0xB6, 0xF5, 0x42, 0x44, 0x43, 0x35, 0x33, 0x34, 0xB3, 0xAC, 0xD6, 0xD8
    };
    ret = dynamicDisplay(port, chStr, 12);
    
    if(ret >= 0) {
        printf("SendCommand success, data length: %d\n", ret);
    } else {
        printf("SendCommand error: %d\n", ret);
    }
    
    //=== RefCount Test ===
    int i;
    for (i = 0; i < 10000; i++) {
        dynamicDisplay(port, chStr, 12);
        clearDisplay(port);
    }
    printf("\n\t=============\n\t RefCount: %d\n\t=============\n", getRefCount());
    //### Test End ###
    
    //=== 清屏 ===
//    ret = clearDisplay(port);
//
//    if(ret >= 0) {
//        printf("ClearScreen success, data length: %d\n", ret);
//    } else {
//        printf("ClearScreen error: %d\n", ret);
//    }
    
    //=== 结束使用 ===
    close(port);
    
    return 0;
}
