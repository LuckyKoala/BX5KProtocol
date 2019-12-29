//
//  BXNKController.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/28.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#include "BXNKController.h"

//=== Actual Implementation ===

//初始化串口及配置
int initialize(int port, int baud, int mode) {
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

int close(int port) {
    return sio_close(port);
}
