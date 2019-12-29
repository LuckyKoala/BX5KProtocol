//
//  PComm.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/29.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

//Purpose: workaround for Xcode run-time

#include <stdio.h>
#include "PComm.h"

int WINAPI sio_open(int port) {
    printf("sio_open(%d)\n", port);
    return SIO_OK;
}

int WINAPI sio_ioctl(int port, int baud, int mode) {
    printf("sio_ioctl(%d, %d, %d)\n", port, baud, mode);
    return SIO_OK;
}

int WINAPI sio_write(int port, char *buf, int len) {
    printf("sio_write to port %d, data: \n", port);
    int i;
    for(i = 0; i < len; i++) {
        printf("%02X ", (unsigned char)buf[i]);
    }
    printf("\n");
    return len;
}

int WINAPI sio_close(int port) {
    printf("sio_close(%d)\n", port);
    return SIO_OK;
}
