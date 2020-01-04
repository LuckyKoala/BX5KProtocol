//
//  ReferenceCount.c
//  BX5KProtocol
//
//  Created by Xin Luo on 2020/1/4.
//  Copyright © 2020 Xin Luo. All rights reserved.
//

#include "ReferenceCount.h"

int refCount = 0;

int getRefCount() {
    return refCount;
}

void* rc_malloc(size_t size) {
    void *ptr = malloc(size);
    if(NULL != ptr) {
        refCount++;
    }
    
    return ptr;
}

void* rc_realloc (void* ptr, size_t size) {
    //Reference: http://www.cplusplus.com/reference/cstdlib/realloc/
    
    if(NULL == ptr) {
        if (0 == size) {
            //Otherwise, if size is zero, the memory previously allocated at ptr is deallocated
            // as if a call to free was made, and a null pointer is returned.
            //-- free(NULL) does nothing
        } else {
            //In case that ptr is a null pointer, the function behaves like malloc,
            // assigning a new block of size bytes and returning a pointer to its beginning.
            refCount++;
        }
    } else {
        if (0 == size) {
            //Otherwise, if size is zero, the memory previously allocated at ptr is deallocated
            // as if a call to free was made, and a null pointer is returned.
            refCount--;
        } else {
            //In case that ptr is a null pointer, the function behaves like malloc,
            // assigning a new block of size bytes and returning a pointer to its beginning.
            // aka. free and alloc, so refCount remain same.
        }
    }
    
    void *newPtr = realloc(ptr, size);
    if(NULL == newPtr) {
        //A null-pointer indicates either that size was zero (an thus ptr was deallocated),
        // or that the function did not allocate storage
        //  (and thus the block pointed by ptr was not modified).
    }
    
    return newPtr;
}

void rc_free (void* ptr) {
    free(ptr);
    
    if(NULL != ptr) {
        refCount--;
    }
}
