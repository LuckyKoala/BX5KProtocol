//
//  BXNKController.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/28.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#include "ReferenceCount.h"
#include "BXNKController.h"

//=== Actual Implementation ===

//初始化串口及配置
int initialize(int port, int baud, int mode) {
    init();
    int ret = sio_open(port);
    if(ret == SIO_OK) {
        ret = sio_ioctl(port, baud, mode);
    }
    
    return ret;
}

//发送指令
int sendCommand(int port, ByteArray cmd) {
    int ret = sio_write(port, cmd.data, cmd.len);
    return ret;
}

int dynamicDisplay(int port, BYTE str[], int len) {
    ByteArray strArr = {
        len,
        str
    };
    BYTE areaCustomConfig[4] = {
        0x18, 0x00, //宽度 单位8像素点 小端法 0x0018 = (1*16+8*1)*8 = 192
        0x5C, 0x00, //高度 单位1像素点 小端法 0x00A0 = (5*16+12*1)*1 = 92
    };
    ByteArray cmd = display(strArr, areaCustomConfig);
    int ret = sendCommand(port, cmd);
    rc_free(cmd.data);
    cmd.data = NULL;
    return ret;
}

int clearDisplay(int port) {
    return sendCommand(port, clearScreen());
}

int close(int port) {
    return sio_close(port);
}
