//
//  main.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2019/12/24.
//  Copyright © 2019 Xin Luo. All rights reserved.
//

#include <stdio.h>
#include "BX4K.h"

int main(int argc, const char * argv[]) {
    init();
    ByteArray pingArr = ping();
    
    printf("Ping Protocol:");
    int i;
    for (i = 0; i < pingArr.len; i++) {
        if (i%8 == 0) printf("\n");
        printf("%02X ", pingArr.data[i]);
    }
    printf("\n");
    
    return 0;
}
