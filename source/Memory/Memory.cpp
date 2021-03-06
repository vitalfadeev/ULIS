// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Memory.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the memory functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Memory/Memory.h"

ULIS_NAMESPACE_BEGIN
void*
XMalloc(
    uint64 iSizeBytes
)
{
    return  malloc( iSizeBytes );
}

void
XFree(
    void* iAlloc
)
{
    free( iAlloc );
}

ULIS_NAMESPACE_END

