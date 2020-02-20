// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Transform2D.cpp
* @author       Clement Berthaud
* @brief        This file provides the Transform2D class definition.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Maths/Transform2D.h"
#include "Maths/Maths.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat2x2.hpp>
#include <glm/vec2.hpp>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTransform2D
//----------------------------------------------------------------------------------------------
//------------------------------------------------------------------- Construction / Destruction
FTransform2D::FTransform2D()
    : mMatrix( 1.f )
    , mDirtyID( true )
    , mDirtyInverseMatrix( true )
{
}


FTransform2D::FTransform2D( const glm::mat3& iMat )
    : mMatrix( iMat )
    , mDirtyID( true )
    , mDirtyInverseMatrix( true )
{
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------- Public API
const std::string&
FTransform2D::ID() const {
    UpdateID();
    return  mID;
}


const glm::mat3&
FTransform2D::Matrix() const {
     return  mMatrix;
}


const glm::mat3&
FTransform2D::InverseMatrix() const {
    UpdateInverseMatrix();
    return  mInverseMatrix;
}

void
FTransform2D::DecomposeMatrix( float* iTx, float* iTy, float* iRotation, float* iScaleX, float* iScaleY, float* iSkewX, float* iSkewY ) const {
    float a = mMatrix[0][0];
    float b = mMatrix[0][1];
    float c = mMatrix[1][0];
    float d = mMatrix[1][1];
    float e = mMatrix[2][0];
    float f = mMatrix[2][1];
    float delta = a * d - b * c;

    float tx = e;
    float ty = f;
    float rot = 0.f;
    float scx = 0.f;
    float scy = 0.f;
    float skx = 0.f;
    float sky = 0.f;
    // Apply the QR-like decomposition.
    if( a != 0.f || b != 0.f ) {
        float r = sqrtf( a * a + b * b );
        rot = FMaths::Sign( b ) * acosf( a / r );
        scx = r;
        scy = delta / r;
        skx = atanf( ( a * c + b * d ) / ( r * r ) );
        sky = 0.f;
    } else if( c != 0.f || d != 0.f ) {
        float s = sqrtf( c * c + d * d );
        rot = FMaths::kPIf / 2.f - ( d > 0 ? acosf( -c / s) : - acosf( c / s ) );
        scx = delta / s;
        scy = s;
        skx = 0.f;
        sky = atanf( ( a * c + b * d ) / ( s * s ) );
    } else {
    }
    *iTx        = tx;
    *iTy        = ty;
    *iRotation  = rot;
    *iScaleX    = scx;
    *iScaleY    = scy;
    *iSkewX     = skx;
    *iSkewY     = sky;
}












//----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Private API
void
FTransform2D::UpdateID() const {
    if( !mDirtyID )
        return;

    constexpr auto sz   = sizeof( glm::mat3 );
    char* buf           = new char[ sz + 1 ];
    memcpy( buf, glm::value_ptr( mMatrix ), sz );
    buf[sz]     = 0;
    mID         = buf;
    mDirtyID    = false;
    delete [] buf;
}


void
FTransform2D::UpdateInverseMatrix() const {
    if( !mDirtyInverseMatrix )
        return;

    mInverseMatrix = glm::inverse( mMatrix );
    mDirtyInverseMatrix = false;
}

/////////////////////////////////////////////////////
// Makers
glm::mat3
MakeIdentityMatrix()
{
    return  glm::mat3(1.f);
}


glm::mat3
MakeRotationMatrix( float iAngleRag )
{
    return  glm::rotate( glm::mat3(1.f), iAngleRag );
}


glm::mat3
MakeScaleMatrix( float iX, float iY )
{
    return  glm::scale( glm::mat3( 1.f ), glm::vec2( iX, iY ) );
}


glm::mat3
MakeShearMatrix( float iX, float iY )
{
    return  glm::shearY( glm::shearX( glm::mat3(1.f), iX ), iY );
}


glm::mat3
MakeTranslationMatrix( float iX, float iY )
{
    return  glm::translate( glm::mat3( 1.f), glm::vec2( iX, iY ) );
}


glm::mat3
ComposeMatrix( const glm::mat3& iA, const glm::mat3& iB )
{
    // Non commutative operation order matters.
    return  iB * iA;
}


ULIS2_NAMESPACE_END
