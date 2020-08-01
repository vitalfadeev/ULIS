// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Platform.h
* @author       Clement Berthaud
* @brief        This file provides platform definitions for the ULIS3 library.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <iostream>

/////////////////////////////////////////////////////
// Detect Build Configuration
#ifdef NDEBUG
    #define ULIS_RELEASE
#else // !NDEBUG
    #define ULIS_DEBUG
#endif // !NDEBUG

/////////////////////////////////////////////////////
// Detect Compiler
#if defined(__clang__)
    #define ULIS_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_GCC
#elif defined(_MSC_VER)
    #define ULIS_MSVC
#else
    #define ULIS_UNKNOWN_COMPILER
#endif

/////////////////////////////////////////////////////
// Disable CRT Secure non standard msvc versions of functions such as strcpy_s
//#ifdef ULIS_MSVC
//    #define _CRT_SECURE_NO_WARNINGS 1
//#endif // ULIS_MSVC

/////////////////////////////////////////////////////
// Detect Platform
#ifdef __EMSCRIPTEN__
    #define ULIS_EMSCRIPTEN
#else
    #ifdef _WIN32
        #define ULIS_WIN
       #ifdef _WIN64
          #define ULIS_WIN64
       #else
          #define ULIS_WIN32
       #endif
    #elif __APPLE__
        #include "TargetConditionals.h"
        #if TARGET_IPHONE_SIMULATOR
             #define ULIS_IOS_SIMULATOR
        #elif TARGET_OS_IPHONE
            #define ULIS_IOS_DEVICE
        #elif TARGET_OS_MAC
            #define ULIS_MACOS
        #else
            #error "Unknown Apple platform"
        #endif
    #elif __linux__
        #define ULIS_LINUX
    #elif __unix__
        #define ULIS_UNIX
    #elif defined(_POSIX_VERSION)
        #define ULIS_POSIX
    #else
        #error "Unknown Platform"
    #endif
#endif

/////////////////////////////////////////////////////
// Safety disable thread and SIMD for Emscripten target
#ifdef ULIS_EMSCRIPTEN
#define ULIS_NO_THREAD_SUPPORT
#define ULIS_NO_SIMD_SUPPORT
#endif

/////////////////////////////////////////////////////
// Force Inline Utility
#define ULIS_ENABLE_FORCEINLINE
#define ULIS_ENABLE_VECTORCALL

#ifdef ULIS_ENABLE_FORCEINLINE
    #if defined(__clang__)
    #define ULIS_FORCEINLINE inline __attribute__ ((always_inline))
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_FORCEINLINE inline __attribute__ ((always_inline))
    #elif defined(_MSC_VER)
    #define ULIS_FORCEINLINE __forceinline
    #else
    #define ULIS_FORCEINLINE inline
    #endif
#else
    #define ULIS_FORCEINLINE inline
#endif // ULIS_ENABLE_FORCEINLINE

#ifdef ULIS_ENABLE_VECTORCALL
    #if defined(__clang__)
    #define ULIS_VECTORCALL __vectorcall
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_VECTORCALL
    #elif defined(_MSC_VER)
    #define ULIS_VECTORCALL __vectorcall
    #else
    #define ULIS_VECTORCALL __vectorcall
    #endif
#else
    #define ULIS_VECTORCALL
#endif // ULIS_ENABLE_FORCEINLINE

/////////////////////////////////////////////////////
// Export utility macros
#ifdef ULIS_WIN
    #ifdef ULIS_BUILD_SHARED
        #define ULIS_SHARED
        #define ULIS_API __declspec( dllexport )
    #elif defined ULIS_DYNAMIC_LIBRARY
        #define ULIS_SHARED
        #define ULIS_API __declspec( dllimport )
    #else
        #define ULIS_STATIC
        #define ULIS_API
    #endif
#else
    #define ULIS_API
#endif

/////////////////////////////////////////////////////
// Macros for Thread and SIMD activation, for embeded targets or WASM
#ifndef ULIS_NO_THREAD_SUPPORT
#define ULIS_COMPILED_WITH_THREAD_SUPPORT
#endif // !ULIS_NO_THREAD_SUPPORT

#ifndef ULIS_NO_SIMD_SUPPORT
#define ULIS_COMPILED_WITH_SIMD_SUPPORT
#endif // !ULIS_NO_SIMD_SUPPORT

#ifndef ULIS_NO_FILESYSTEM_SUPPORT
#define ULIS_COMPILED_WITH_FILESYSTEM_SUPPORT
#endif // !ULIS_NO_SIMD_SUPPORT

/////////////////////////////////////////////////////
// Erors
//#pragma warning(disable : 4251)     // Shut down dll interface warnings.
//#pragma warning(disable : 26812)    // Shut non-class enum warnings.

/////////////////////////////////////////////////////
// Define Namespaces
#define ULIS_NAMESPACE_NAME        ULIS3
#define ULIS_SHORT_NAMESPACE_NAME  ul3
#define ULIS_NAMESPACE_BEGIN       namespace ULIS_NAMESPACE_NAME {
#define ULIS_NAMESPACE_END         }
namespace ULIS_NAMESPACE_NAME {}
namespace ULIS_SHORT_NAMESPACE_NAME = ULIS_NAMESPACE_NAME;

/////////////////////////////////////////////////////
// Version Specification
#define ULIS_VERSION_MAJOR      3
#define ULIS_VERSION_MINOR      0
#define ULIS_VERSION_MAJOR_STR  "3"
#define ULIS_VERSION_MINOR_STR  "0"

/////////////////////////////////////////////////////
// Crash Behaviours
#define ULIS_CRASH *((volatile int*)0) = 0

/////////////////////////////////////////////////////
// Assert Behaviours

#if defined( ULIS_DEBUG ) || defined( ULIS_FORCE_ASSERT )
    #define ULIS_ASSERT( cond, log )  if( !( cond ) ) { std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "Assertion failed: " << log << std::endl; ULIS_CRASH; }
    #define ULIS_ASSERT_ENABLED
#else
    #define ULIS_ASSERT( cond, log )
#endif

/////////////////////////////////////////////////////
// SIMD PATCH FOR GNU < 9
// CHECK: this conflicts with xcode sometimes identifying itself as GNUC < 9 but defining the appropriate simd instructions.
#ifdef ULIS_COMPILED_WITH_SIMD_SUPPORT
#if defined( ULIS_GCC ) && ( defined( ULIS_WIN ) || defined( ULIS_LINUX ) )
#if __GNUC__ < 9
// unaligned load and store functions
#define _mm_loadu_si16(p) _mm_cvtsi32_si128(*(unsigned short const*)(p))
#define _mm_storeu_si16(p, a) (void)(*(short*)(p) = (short)_mm_cvtsi128_si32((a)))
#define _mm_loadu_si32(p) _mm_cvtsi32_si128(*(unsigned int const*)(p))
#define _mm_storeu_si32(p, a) (void)(*(int*)(p) = _mm_cvtsi128_si32((a)))
#define _mm_loadu_si64(p) _mm_loadl_epi64((__m128i const*)(p))
#define _mm_storeu_si64(p, a) (_mm_storel_epi64((__m128i*)(p), (a)))
#endif
#endif
#include <immintrin.h>
#endif

/////////////////////////////////////////////////////
// Conditional compile time detection macro in order to decide if we should include SIMD versions in the various dispatch
#ifdef ULIS_COMPILED_WITH_SIMD_SUPPORT
    #ifdef __AVX2__
        #define ULIS_COMPILETIME_AVX2_SUPPORT
    #endif
    #ifdef __SSE4_2__
        #define ULIS_COMPILETIME_SSE42_SUPPORT
    #endif
#endif

/////////////////////////////////////////////////////
// glm FORCE extensions, before any glm related includes
#ifdef ULIS_COMPILED_WITH_SIMD_SUPPORT
#define GLM_FORCE_SSE42
#define GLM_FORCE_INTRINSICS
#endif
#define GLM_FORCE_SWIZZLE
