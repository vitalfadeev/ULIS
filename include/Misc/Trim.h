// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Trim.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Trim entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

ULIS_API FRectI GetTrimmedTransparencyRect( FOldThreadPool*            iOldThreadPool
                                          , bool                    iBlocking
                                          , uint32                  iPerfIntent
                                          , const FHostDeviceInfo&  iHostDeviceInfo
                                          , bool                    iCallCB
                                          , const FBlock*           iSource );

ULIS_NAMESPACE_END

