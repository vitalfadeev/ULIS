// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Matrix3x3.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the TMatrix3 class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/MatrixFunc.h"
#include "Math/Geometry/Vector3.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TMatrix3
/// @brief      The TMatrix3 class provides a simple 3x3 square matrix class for
///             geometry applications.
/// @details    The TMatrix3 class is a template class that provides basic matrix
///             functionnalities, it can be used with any numeric types,
///             ranging from integer values to floating point values.
///
///             TMatrix3 are column-major ordered.
template< typename T >
class TMatrix3
{
public:
    typedef TVector3< T > tRow;
    typedef TVector3< T > tColumn;

private:
    /*! The columns of the matrix. */
    tColumn mCols[3];

public:
    // Constructors
    /*! Default constructor ( identity matrix ). */
    ULIS_MATRIX_FUNC TMatrix3();

    /*! Constructor from input diagonal value. */
    ULIS_MATRIX_FUNC TMatrix3( T iValue );

    /*! Constructor from input cells values. */
    ULIS_MATRIX_FUNC TMatrix3(
          T iM00, T iM01, T iM02
        , T iM10, T iM11, T iM12
        , T iM20, T iM21, T iM22
    );

    /*! Constructor from row vector values. */
    ULIS_MATRIX_FUNC TMatrix3(
          const tColumn& iCol0
        , const tColumn& iCol1
        , const tColumn& iCol2
    );

    /*! Copy constructor. */
    ULIS_MATRIX_FUNC TMatrix3( const TMatrix3& iOther );

    /*! Assignment operator from matrix. */
    ULIS_MATRIX_FUNC TMatrix3& operator=( const TMatrix3& iOther );



    // Conversion
    /*! Conversion constructor from other matrix type. */
    template< typename U >
    ULIS_MATRIX_FUNC TMatrix3( const TMatrix3< U >& iOther );



    // Named Functions
    /*! Obtain the inverse of the matrix. */
    ULIS_MATRIX_FUNC TMatrix3 Inverse();

    /*! Obtain the transpose of the matrix. */
    ULIS_MATRIX_FUNC TMatrix3 Transpose();

    /*! Obtain the determinant of the matrix. */
    ULIS_MATRIX_FUNC T Determinant();



    // Transform Functions
    /*! Static maker for 2D 2x2 rotation matrix. */
    static ULIS_MATRIX_FUNC TMatrix3 MakeRotationMatrix( T iAngleRad );

    /*! Static maker for 2D 2x2 scale matrix. */
    static ULIS_MATRIX_FUNC TMatrix3 MakeScaleMatrix( T iScaleX, T iScaleY );

    /*! Static maker for 2D 2x2 skew matrix. */
    static ULIS_MATRIX_FUNC TMatrix3 MakeSkewMatrix( T iSkewX, T iSkewY );

    /*! Static maker for 2D 2x2 translation matrix. */
    static ULIS_MATRIX_FUNC TMatrix3 MakeTranslationMatrix( T iX, T iY );



    // Boolean operators
    /*! Equality operator. */
    ULIS_MATRIX_FUNC bool operator==( const TMatrix3& iOther );

    /*! Inequality operator. */
    ULIS_MATRIX_FUNC bool operator!=( const TMatrix3& iOther );



    // Unary Negative Operator
    /*! Unary negative operator. */
    ULIS_MATRIX_FUNC TMatrix3 operator-() const;



    // Unary arithmetic operator with scalar
    /*! Unary addition operator with value. */
    ULIS_MATRIX_FUNC TMatrix3& operator+=( T iValue );

    /*! Unary substraction operator with value. */
    ULIS_MATRIX_FUNC TMatrix3& operator-=( T iValue );

    /*! Unary multiplication operator with value. */
    ULIS_MATRIX_FUNC TMatrix3& operator*=( T iValue );

    /*! Unary division operator with value. */
    ULIS_MATRIX_FUNC TMatrix3& operator/=( T iValue );



    // Unary arithmetic operator with matrix
    /*! Unary addition operator with matrix. */
    ULIS_MATRIX_FUNC TMatrix3& operator+=( const TMatrix3& iOther );

    /*! Unary substraction operator with matrix. */
    ULIS_MATRIX_FUNC TMatrix3& operator-=( const TMatrix3& iOther );

    /*! Unary multiplication operator with matrix. */
    ULIS_MATRIX_FUNC TMatrix3& operator*=( const TMatrix3& iOther );

    /*! Unary division operator with matrix. */
    ULIS_MATRIX_FUNC TMatrix3& operator/=( const TMatrix3& iOther );



    // Other Operators
    /*! Subscript accessor for matrix columns. */
    ULIS_MATRIX_FUNC tColumn& operator[]( int iIndex );

    /*! Subscript accessor for const matrix columns. */
    ULIS_MATRIX_FUNC const tColumn& operator[]( int iIndex ) const;
};



// Constructors
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >::TMatrix3()
    : mCols {
          tColumn( 1, 0, 0 )
        , tColumn( 0, 1, 0 )
        , tColumn( 0, 0, 1 )
    }
{}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >::TMatrix3( T iValue )
    : mCols {
          tColumn( iValue, 0, 0 )
        , tColumn( 0, iValue, 0 )
        , tColumn( 0, 0, iValue )
    }
{}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >::TMatrix3(
      T iM00, T iM01, T iM02
    , T iM10, T iM11, T iM12
    , T iM20, T iM21, T iM22
    )
    : mCols {
          tColumn( iM00, iM01, iM02 )
        , tColumn( iM10, iM11, iM12 )
        , tColumn( iM20, iM21, iM22 )
    }
{}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >::TMatrix3(
      const tColumn& iCol0
    , const tColumn& iCol1
    , const tColumn& iCol2
    )
    : mCols {
          iCol0
        , iCol1
        , iCol2
    }
{}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >::TMatrix3( const TMatrix3< T >& iOther )
    : mCols {
          iOther.mCols[0]
        , iOther.mCols[1]
        , iOther.mCols[2]
    }
{}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator=( const TMatrix3< T >& iOther ) {
    mCols[0] = iOther.mCols[0];
    mCols[1] = iOther.mCols[1];
    mCols[2] = iOther.mCols[2];
}



// Conversion
template< typename T >
template< typename U >
ULIS_MATRIX_FUNC TMatrix3< T >::TMatrix3( const TMatrix3< U >& iOther )
    : mCols {
          tColumn( iOther.mCols[0] )
        , tColumn( iOther.mCols[1] )
        , tColumn( iOther.mCols[2] )
    }
{}



// Named Functions
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > TMatrix3< T >::Inverse() {
    T inv_det = static_cast< T >( 1 ) / Determinant();

    return  TMatrix3< T >(
          + ( mCols[1][1] * mCols[2][2] - mCols[2][1] * mCols[1][2] ) * inv_det
        , - ( mCols[0][1] * mCols[2][2] - mCols[2][1] * mCols[0][2] ) * inv_det
        , + ( mCols[0][1] * mCols[1][2] - mCols[1][1] * mCols[0][2] ) * inv_det
        , - ( mCols[1][0] * mCols[2][2] - mCols[2][0] * mCols[1][2] ) * inv_det
        , + ( mCols[0][0] * mCols[2][2] - mCols[2][0] * mCols[0][2] ) * inv_det
        , - ( mCols[0][0] * mCols[1][2] - mCols[1][0] * mCols[0][2] ) * inv_det
        , + ( mCols[1][0] * mCols[2][1] - mCols[2][0] * mCols[1][1] ) * inv_det
        , - ( mCols[0][0] * mCols[2][1] - mCols[2][0] * mCols[0][1] ) * inv_det
        , + ( mCols[0][0] * mCols[1][1] - mCols[1][0] * mCols[0][1] ) * inv_det
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > TMatrix3< T >::Transpose() {
    return  TMatrix3< T >(
          mCols[0][0]
        , mCols[1][0]
        , mCols[2][0]
        , mCols[0][1]
        , mCols[1][1]
        , mCols[2][1]
        , mCols[0][2]
        , mCols[1][2]
        , mCols[2][2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC T TMatrix3< T >::Determinant() {
    return  + mCols[0][0] * ( mCols[1][1] * mCols[2][2] - mCols[2][1] * mCols[1][2] )
            - mCols[1][0] * ( mCols[0][1] * mCols[2][2] - mCols[2][1] * mCols[0][2] )
            + mCols[2][0] * ( mCols[0][1] * mCols[1][2] - mCols[1][1] * mCols[0][2] );
}



// Transform Functions
template< typename T >
//static
ULIS_MATRIX_FUNC
TMatrix3< T >
TMatrix3< T >::MakeRotationMatrix( T iAngleRad ) {
    const T cos_angle = cos( iAngleRad );
    const T sin_angle = sin( iAngleRad );
    return  TMatrix3< T >(
          +cos_angle, +sin_angle, 0
        , -sin_angle, +cose_angle, 0
        , 0, 0, 1
    );
}

template< typename T >
//static
ULIS_MATRIX_FUNC
TMatrix3< T >
TMatrix3< T >::MakeScaleMatrix( T iScaleX, T iScaleY ) {
    return  TMatrix3< T >(
          iScaleX, 0, 0
        , 0, iScaleY, 0
        , 0, 0, 1
    );
}

template< typename T >
//static
ULIS_MATRIX_FUNC
TMatrix3< T >
TMatrix3< T >::MakeSkewMatrix( T iSkewX, T iSkewY ) {
    return  TMatrix3< T >(
          1, iSkewY, 0
        , iSkewX, 1, 0
        , 0, 0, 1
    );
}

template< typename T >
//static
ULIS_MATRIX_FUNC
TMatrix3< T >
TMatrix3< T >::MakeTranslationMatrix( T iX, T iY ) {
    return  TMatrix3< T >(
          0, 0, 0
        , 0, 0, 0
        , iX, iY, 1
    );
}



// Boolean operators
template< typename T >
ULIS_MATRIX_FUNC bool TMatrix3< T >::operator==( const TMatrix3< T >& iOther ) {
    return  mCols[0] == iOther.mCols[0]
        &&  mCols[1] == iOther.mCols[1]
        &&  mCols[2] == iOther.mCols[2];
}

template< typename T >
ULIS_MATRIX_FUNC bool TMatrix3< T >::operator!=( const TMatrix3< T >& iOther ) {
    return  !( *this == iOther );
}



// Unary Negative Operator
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > TMatrix3< T >::operator-() const {
    return  TMatrix3< T >( -mCols[0], -mCols[1], -mCols[2] );
}



// Unary arithmetic operator with scalar
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator+=( T iValue ) {
    mCols[0] += iValue;
    mCols[1] += iValue;
    mCols[2] += iValue;
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator-=( T iValue ) {
    mCols[0] -= iValue;
    mCols[1] -= iValue;
    mCols[2] -= iValue;
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator*=( T iValue ) {
    mCols[0] *= iValue;
    mCols[1] *= iValue;
    mCols[2] *= iValue;
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator/=( T iValue ) {
    mCols[0] /= iValue;
    mCols[1] /= iValue;
    mCols[2] /= iValue;
}



// Unary arithmetic operator with matrix
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator+=( const TMatrix3& iOther ) {
    mCols[0] += iOther.mCols[0];
    mCols[1] += iOther.mCols[1];
    mCols[2] += iOther.mCols[2];
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator-=( const TMatrix3& iOther ) {
    mCols[0] -= iOther.mCols[0];
    mCols[1] -= iOther.mCols[1];
    mCols[2] -= iOther.mCols[2];
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator*=( const TMatrix3& iOther ) {
    return  ( *this = *this * iOther );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T >& TMatrix3< T >::operator/=( const TMatrix3& iOther ) {
    return  ( *this *= iOther.Inverse() );
}



// Other Operators
template< typename T >
ULIS_MATRIX_FUNC typename TMatrix3< T >::tColumn& TMatrix3< T >::operator[]( int iIndex ) {
    ULIS_ASSERT( iIndex >= 0 && iIndex < 3, "Bad Index" );
    return  mCols[ iIndex ];
}

template< typename T >
ULIS_MATRIX_FUNC const typename TMatrix3< T >::tColumn& TMatrix3< T >::operator[]( int iIndex ) const {
    ULIS_ASSERT( iIndex >= 0 && iIndex < 3, "Bad Index" );
    return  mCols[ iIndex ];
}



// Binary arithmetic operator with scalar on left hand side
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator+( T iValue, const TMatrix3< T >& iMat ) {
    return  TMatrix3< T >(
          iValue + iMat[0]
        , iValue + iMat[1]
        , iValue + iMat[2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator-( T iValue, const TMatrix3< T >& iMat ) {
    return  TMatrix3< T >(
          iValue - iMat[0]
        , iValue - iMat[1]
        , iValue - iMat[2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator*( T iValue, const TMatrix3< T >& iMat ) {
    return  TMatrix3< T >(
          iValue * iMat[0]
        , iValue * iMat[1]
        , iValue * iMat[2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator/( T iValue, const TMatrix3< T >& iMat ) {
    return  TMatrix3< T >(
          iValue / iMat[0]
        , iValue / iMat[1]
        , iValue / iMat[2]
    );
}



// Binary arithmetic operator with scalar on right hand side
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator+( const TMatrix3< T >& iMat, T iValue ) {
    return  TMatrix3< T >(
          iMat[0] + iValue
        , iMat[1] + iValue
        , iMat[2] + iValue
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator-( const TMatrix3< T >& iMat, T iValue ) {
    return  TMatrix3< T >(
          iMat[0] - iValue
        , iMat[1] - iValue
        , iMat[2] - iValue
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator*( const TMatrix3< T >& iMat, T iValue ) {
    return  TMatrix3< T >(
          iMat[0] * iValue
        , iMat[1] * iValue
        , iMat[2] * iValue
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator/( const TMatrix3< T >& iMat, T iValue ) {
    return  TMatrix3< T >(
          iMat[0] / iValue
        , iMat[1] / iValue
        , iMat[2] / iValue
    );
}



// Binary arithmetic operator with other matrix
template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator+( const TMatrix3< T >& iMat, const TMatrix3< T >& iOther ) {
    return  TMatrix3< T >(
          iMat[0] + iOther[0]
        , iMat[1] + iOther[1]
        , iMat[2] + iOther[2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator-( const TMatrix3< T >& iMat, const TMatrix3< T >& iOther ) {
    return  TMatrix3< T >(
          iMat[0] - iOther[0]
        , iMat[1] - iOther[1]
        , iMat[2] - iOther[2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator*( const TMatrix3< T >& iMat, const TMatrix3< T >& iOther ) {
    return  TMatrix3< T >(
          iMat[0][0] * iOther[0][0] + iMat[1][0] * iOther[0][1] + iMat[2][0] * iOther[0][2]
        , iMat[0][0] * iOther[1][0] + iMat[1][0] * iOther[1][1] + iMat[2][0] * iOther[1][2]
        , iMat[0][0] * iOther[2][0] + iMat[1][0] * iOther[2][1] + iMat[2][0] * iOther[2][2]
        , iMat[0][1] * iOther[0][0] + iMat[1][1] * iOther[0][1] + iMat[2][1] * iOther[0][2]
        , iMat[0][1] * iOther[1][0] + iMat[1][1] * iOther[1][1] + iMat[2][1] * iOther[1][2]
        , iMat[0][1] * iOther[2][0] + iMat[1][1] * iOther[2][1] + iMat[2][1] * iOther[2][2]
        , iMat[0][2] * iOther[0][0] + iMat[1][2] * iOther[0][1] + iMat[2][2] * iOther[0][2]
        , iMat[0][2] * iOther[1][0] + iMat[1][2] * iOther[1][1] + iMat[2][2] * iOther[1][2]
        , iMat[0][2] * iOther[2][0] + iMat[1][2] * iOther[2][1] + iMat[2][2] * iOther[2][2]
    );
}

template< typename T >
ULIS_MATRIX_FUNC TMatrix3< T > operator/( const TMatrix3< T >& iMat, const TMatrix3< T >& iOther ) {
    return  iMat * iOther.Inverse();
}



// Binary matrix vector multiplication operators.
template< typename T >
ULIS_MATRIX_FUNC typename TMatrix3< T >::tColumn operator*( const TMatrix3< T >& iMat, const typename TMatrix3< T >::tRow& iRow ) {
    return  TVector3< T >(
          iMat[0][0] * iRow.x + iMat[1][0] * iRow.y + iMat[2][0] * iRow.z
        , iMat[0][1] * iRow.x + iMat[1][1] * iRow.y + iMat[2][1] * iRow.z
        , iMat[0][2] * iRow.x + iMat[1][2] * iRow.y + iMat[2][2] * iRow.z
    );
}

template< typename T >
ULIS_MATRIX_FUNC typename TMatrix3< T >::tRow operator*( const typename TMatrix3< T >::tColumn& iColumn, const TMatrix3< T >& iMat ) {
    return  TVector3< T >(
          iMat[0][0] * iColumn.x + iMat[0][1] * iColumn.y + iMat[0][2] * iColumn.z
        , iMat[1][0] * iColumn.x + iMat[1][1] * iColumn.y + iMat[1][2] * iColumn.z
        , iMat[2][0] * iColumn.x + iMat[2][1] * iColumn.y + iMat[2][2] * iColumn.z
    );
}

ULIS_NAMESPACE_END

