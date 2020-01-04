//
//  BXNKController.h
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/28.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#ifndef BXNKController_h
#define BXNKController_h

#include "PComm.h"
#include "BX4K.h"

//初始化串口及配置
int initialize(int port, int baud, int mode);
//发送指令
int dynamicDisplay(int port, BYTE str[], int len);
int clearDisplay(int port);
//关闭串口
int close(int port);

#endif /* BXNKController_h */
