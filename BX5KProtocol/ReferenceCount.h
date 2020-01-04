//
//  ReferenceCount.h
//  BX5KProtocol
//
//  Created by Xin Luo on 2020/1/4.
//  Copyright © 2020 Xin Luo. All rights reserved.
//

#ifndef ReferenceCount_h
#define ReferenceCount_h

#include <stdlib.h>

void* rc_malloc(size_t size);
void* rc_realloc (void* ptr, size_t size);
void rc_free (void* ptr);
int getRefCount(void);

#endif /* ReferenceCount_h */
