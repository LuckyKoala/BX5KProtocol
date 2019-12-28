//
//  main.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/24.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#include <stdio.h>
#include "BX4K.h"

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
        14, str
    };
    verifyAndPrint("Display", display(arr), littleEndian(0xA87D));
    
    return 0;
}
