/**
*
*   ULIS
*__________________
*
* @file     ULIS.Interface.Blend.h
* @author   Clement Berthaud
* @brief    This file provides the definitions for the FPainterContext class.
*/
#include "ULIS/Interface/ULIS.Interface.Paint.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Paint/ULIS.Paint.PainterContext.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FPainterContext
//static
void
FPainterContext::DrawLine( IBlock* iBlock
                         , const FPoint& iP0
                         , const FPoint& iP1
                         , const CColor& iColor
                         , const FPerformanceOptions& iPerformanceOptions
                         , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawLine( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                            \
                                                          , iP0, iP1, iColor, iPerformanceOptions, iCallInvalidCB );                            \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawLineAA( IBlock* iBlock, const FPoint& iP0, const FPoint& iP1, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                    \
            case ULIS_REG[ n ]:                                                                                                                                     \
            {                                                                                                                                                       \
                TPainterContext< ULIS_REG[ n ] >::DrawLineAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iP0, iP1, iColor, iPerformanceOptions, iCallInvalidCB );    \
                break;                                                                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}

//static
void
FPainterContext::DrawCircleAndres( IBlock* iBlock, const FPoint iCenter, const int iRadius, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                            \
            case ULIS_REG[ n ]:                                                                                                                                                             \
            {                                                                                                                                                                               \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleAndres( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    
//static
void
FPainterContext::DrawCircleAndresAA( IBlock* iBlock, const FPoint iCenter, const int iRadius, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleAndresAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawCircleBresenham( IBlock* iBlock, const FPoint iCenter, const int iRadius, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleBresenham( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    
    
//static
void
FPainterContext::DrawCircleBresenhamAA( IBlock* iBlock, const FPoint iCenter, const int iRadius, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawCircleBresenhamAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawEllipse( IBlock* iBlock, const FPoint iCenter, const int iA, const int iB, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawEllipse( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iA, iB, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    

//static
void
FPainterContext::DrawEllipseAA( IBlock* iBlock, const FPoint iCenter, const int iA, const int iB, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawEllipseAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iA, iB, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawRotatedEllipse( IBlock* iBlock, const FPoint iCenter, const int iA, const int iB, const int iRotationDegrees, const CColor& iColor, const bool iFilled,const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawRotatedEllipse( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iA, iB, iRotationDegrees, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    
    
//static
void
FPainterContext::DrawRotatedEllipseAA( IBlock* iBlock, const FPoint iCenter, const int iA, const int iB, const int iRotationDegrees, const CColor& iColor, const bool iFilled,const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawRotatedEllipseAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iA, iB, iRotationDegrees, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawArcAndres( IBlock* iBlock, const FPoint iCenter, const int iRadius, const int iStartDegree, const int iEndDegree, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawArcAndres( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iStartDegree, iEndDegree, iColor, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    

//static
void
FPainterContext::DrawArcAndresAA( IBlock* iBlock, const FPoint iCenter, const int iRadius, const int iStartDegree, const int iEndDegree, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawArcAndresAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iStartDegree, iEndDegree, iColor, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawArcBresenham( IBlock* iBlock, const FPoint iCenter, const int iRadius, const int iStartDegree, const int iEndDegree, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawArcBresenham( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iStartDegree, iEndDegree, iColor, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    

//static
void
FPainterContext::DrawArcBresenhamAA( IBlock* iBlock, const FPoint iCenter, const int iRadius, const int iStartDegree, const int iEndDegree, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                               \
            case ULIS_REG[ n ]:                                                                                                                                                  \
            {                                                                                                                                                                                  \
                TPainterContext< ULIS_REG[ n ] >::DrawArcBresenhamAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCenter, iRadius, iStartDegree, iEndDegree, iColor, iPerformanceOptions, iCallInvalidCB );     \
                break;                                                                                                                                                                         \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawRectangle( IBlock* iBlock, const FPoint iTopLeft, const FPoint iBottomRight, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                                         \
            case ULIS_REG[ n ]:                                                                                                                                                            \
            {                                                                                                                                                                                            \
                TPainterContext< ULIS_REG[ n ] >::DrawRectangle( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iTopLeft, iBottomRight, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );      \
                break;                                                                                                                                                                                   \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FPainterContext::DrawPolygon( IBlock* iBlock, std::vector< FPoint >& iPoints, const CColor& iColor, const bool iFilled, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                                         \
            case ULIS_REG[ n ]:                                                                                                                                                            \
            {                                                                                                                                                                                            \
                TPainterContext< ULIS_REG[ n ] >::DrawPolygon( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iPoints, iColor, iFilled, iPerformanceOptions, iCallInvalidCB );      \
                break;                                                                                                                                                                                   \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    
//static
void FPainterContext::DrawQuadraticBezier( IBlock* iBlock, const FPoint& iCtrlPt0, const FPoint& iCtrlPt1, const FPoint& iCtrlPt2, const float iWeight, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                                         \
            case ULIS_REG[ n ]:                                                                                                                                                            \
            {                                                                                                                                                                                            \
                TPainterContext< ULIS_REG[ n ] >::DrawQuadraticBezier( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCtrlPt0, iCtrlPt1, iCtrlPt2, iWeight, iColor, iPerformanceOptions, iCallInvalidCB );      \
                break;                                                                                                                                                                                   \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}
    
    
//static
void FPainterContext::DrawQuadraticBezierAA( IBlock* iBlock, const FPoint& iCtrlPt0, const FPoint& iCtrlPt1, const FPoint& iCtrlPt2, const float iWeight, const CColor& iColor, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                                                                         \
            case ULIS_REG[ n ]:                                                                                                                                                            \
            {                                                                                                                                                                                            \
                TPainterContext< ULIS_REG[ n ] >::DrawQuadraticBezierAA( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock, iCtrlPt0, iCtrlPt1, iCtrlPt2, iWeight, iColor, iPerformanceOptions, iCallInvalidCB );      \
                break;                                                                                                                                                                                   \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}



} // namespace ULIS

