// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         OldThreadPool_dummy.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the dummy FOldThreadPool class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <iostream>
#include <deque>
#include <functional>
#include <random>

#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
// FOldThreadPool
class FOldThreadPool
{
public:
    // Construction / Destruction

    FOldThreadPool( unsigned int iCount = 0 ) {}
    ~FOldThreadPool() {}

public:
    // Public API
    template<class F>
    void ScheduleJob( F&& f ) {}

    template<class F, typename ... Args >
    void ScheduleJob( F&& f, Args&& ... args ) {}
    void WaitForCompletion() {}
    void SetNumWorkers( unsigned int iValue ) {}
    unsigned int    GetProcessed() const    { return  0; }
    unsigned int    GetNumWorkers() const   { return  0; }
    static uint32   MaxWorkers()            { return  0; }
};

#define ULIS_MACRO_INLINE_PARALLEL_FOR( _PERF, _POOL, _BLOCKING, _MAX, _FUNC, ... )    \
    {                                                                                   \
        for( int pLINE = 0; pLINE < _MAX; ++pLINE )                                     \
            _FUNC( __VA_ARGS__ );                                                       \
    }


ULIS_NAMESPACE_END

