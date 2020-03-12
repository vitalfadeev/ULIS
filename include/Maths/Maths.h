// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Maths.h
* @author       Clement Berthaud
* @brief        This file provides the FMaths class declaration.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry.h"
#include <cmath>
#include <immintrin.h>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FMaths
struct ULIS2_API FMaths
{
    // Do not change this value !
    static constexpr float  kEpsilonf = .000001f;
    static constexpr double kPId = 3.1415926535897932;
    static constexpr float  kPIf = 3.14159265f;

    static ULIS2_FORCEINLINE int32 FloorToInt( ufloat iValue ) {
        return  _mm_cvt_ss2si( _mm_set_ss( iValue + iValue - 0.5f ) ) >> 1;
    }

    static ULIS2_FORCEINLINE ufloat FloorToFloat( ufloat iValue ) {
        return  static_cast< ufloat >( FloorToInt( iValue ) );
    }

    static ULIS2_FORCEINLINE int32 CeilToInt( ufloat iValue ) {
        return -( _mm_cvt_ss2si( _mm_set_ss( -0.5f - ( iValue + iValue ) ) ) >> 1 );
    }

    static ULIS2_FORCEINLINE ufloat CeilToFloat( ufloat iValue ) {
        return static_cast< ufloat >( CeilToInt( iValue ) );
    }

    static ULIS2_FORCEINLINE ufloat RoundToNegativeInfinity( ufloat iValue ) {
        return  FloorToFloat( iValue );
    }

    static ULIS2_FORCEINLINE ufloat RoundToPositiveInfinity( ufloat iValue ) {
        return  CeilToFloat( iValue );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Min( T iA, T iB ) {
        return  iA < iB ? iA : iB;
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Max( T iA, T iB ) {
        return  iA > iB ? iA : iB;
    }

    template< typename T >
    static ULIS2_FORCEINLINE TVec2< T > Max( const TVec2< T >& iValue, T iMax ) {
        return  TVec2< T >( Max< T >( iValue.x, iMax ), Max< T >( iValue.y, iMax ) );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Min3( T iA, T iB, T iC ) {
        return  Min( iA, Min( iB, iC ) );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Min4( T iA, T iB, T iC, T iD ) {
        return  Min( iA, Min3( iB, iC, iD ) );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Max3( T iA, T iB, T iC ) {
        return  Max( iA, Max( iB, iC ) );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Max4( T iA, T iB, T iC, T iD ) {
        return  Max( iA, Max3( iB, iC, iD ) );
    }

    template< typename T >
    static inline T&&  VMin( T&&  val )
    {
        return  std::forward< T >( val );
    }

    template< typename T0, typename T1, typename... Ts >
    static inline auto  VMin( T0&&  val1, T1&&  val2, Ts&&...  vs )
    {
        return  ( val1 < val2 ) ? VMin( val1, std::forward< Ts >( vs )... ) : VMin( val2, std::forward< Ts >( vs )... );
    }

    template< typename T >
    static inline T&&  VMax( T&&  val )
    {
        return  std::forward< T >( val );
    }

    template< typename T0, typename T1, typename... Ts >
    static inline auto  VMax( T0&&  val1, T1&&  val2, Ts&&...  vs )
    {
        return  ( val1 > val2 ) ? VMax( val1, std::forward< Ts >( vs )... ) : VMax( val2, std::forward< Ts >( vs )... );
    }


    template< typename T >
    static ULIS2_FORCEINLINE T Clamp( T iValue, T iMin, T iMax ) {
        return  Max( iMin, Min( iValue, iMax ) );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Abs( T iA ) {
        return ( iA < 0 ? -iA : iA );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Sign( T iValue ) {
        return  iValue < T(0) ? T(-1) : T(1);
    }

    template< typename T >
    static ULIS2_FORCEINLINE T PyModulo( T iValue, T iMod ) {
        return  ( iMod + ( iValue % iMod ) ) % iMod;
    }

    template< typename T >
    static ULIS2_FORCEINLINE TVec2< T > PyModulo( const TVec2< T >& iValue, const TVec2< T >& iMod ) {
        return  TVec2< T >( PyModulo( iValue.x, iMod.x ), PyModulo( iValue.y, iMod.y ) );
    }

    static ULIS2_FORCEINLINE double RadToDeg( double iRad )
    {
        return ( iRad * 180 / FMaths::kPId );
    }

    static ULIS2_FORCEINLINE double DegToRad( double iDeg )
    {
        return ( iDeg * FMaths::kPId / 180 );
    }

    static ULIS2_FORCEINLINE float DegToRadF( float iDeg )
    {
        return ( iDeg * FMaths::kPIf / 180.f );
    }

    static ULIS2_FORCEINLINE int IntegerPartOfNumber( float iNumber )
    {
        return (int)iNumber;
    }

    static ULIS2_FORCEINLINE float FloatingPartOfNumber( float iNumber )
    {
        if ( iNumber > 0 )
            return iNumber - IntegerPartOfNumber( iNumber );
        else
            return 1.f - ( iNumber - IntegerPartOfNumber( iNumber ) );
    }

    static ULIS2_FORCEINLINE FVec2F FloatingPart( const FVec2F& iVec ) {
        return  FVec2F( FloatingPartOfNumber( iVec.x ), FloatingPartOfNumber( iVec.y ) );
    }

    static ULIS2_FORCEINLINE FVec2F Abs( const FVec2F& iVec ) {
        return  FVec2F( Abs( iVec.x ), Abs( iVec.y ) );
    }

    static ULIS2_FORCEINLINE int RoundNumber( float iNumber )
    {
        return  (int)( iNumber + 0.5f );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Ceil( T iValue )
    {
        return  ceil( iValue );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T Floor( T iValue )
    {
        return  floor( iValue );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T RoundAwayFromZero( T iValue )
    {
        return  iValue < 0 ? (T)floor( iValue ) : (T)ceil( iValue );
    }

    template< typename T >
    static ULIS2_FORCEINLINE T RoundTowardsZero( T iValue )
    {
        return  iValue < 0 ? (T)ceil( iValue ) : (T)floor( iValue );
    }

    static ULIS2_FORCEINLINE bool EpsilonComp( float iA, float iB )
    {
        return  Abs( iA - iB ) <= kEpsilonf;
    }

    static ULIS2_FORCEINLINE ufloat FixInf( ufloat iValue ) {
        return  isinf( iValue ) || isnan( iValue ) ? 0.f : iValue;
    }

}; // struct FMaths

ULIS2_NAMESPACE_END

