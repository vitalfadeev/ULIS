// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMono_Misc_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"
#include "Blend/BlendHelpers.h"
#include "Blend/BlendingPRNGSeed.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncF.ipp"
#include "Color/ModelStructs.h"
#include "Maths/Geometry.h"

float gBayer8x8Matrix[8][8] = {
    { 0.015625f, 0.765625f, 0.203125f, 0.953125f, 0.0625f, 0.8125f, 0.25f, 1.0f          },
    { 0.515625f, 0.265625f, 0.703125f, 0.453125f, 0.5625f, 0.3125f, 0.75f, 0.5f          },
    { 0.140625f, 0.890625f, 0.078125f, 0.828125f, 0.1875f, 0.9375f, 0.125f, 0.875f       },
    { 0.640625f, 0.390625f, 0.578125f, 0.328125f, 0.6875f, 0.4375f, 0.625f, 0.375f       },
    { 0.046875f, 0.796875f, 0.234375f, 0.984375f, 0.03125f, 0.78125f, 0.21875f, 0.96875f },
    { 0.546875f, 0.296875f, 0.734375f, 0.484375f, 0.53125f, 0.28125f, 0.71875f, 0.46875f },
    { 0.171875f, 0.921875f, 0.109375f, 0.859375f, 0.15625f, 0.90625f, 0.09375f, 0.84375f },
    { 0.671875f, 0.421875f, 0.609375f, 0.359375f, 0.65625f, 0.40625f, 0.59375f, 0.34375f } };

ULIS2_NAMESPACE_BEGIN
template< typename T, eBlendingMode _BM, eAlphaMode _AM >
void BlendMono_Misc_MEM_Subpixel( const FBlock* iSource, FBlock* iBackdrop, const FRect& iSrcROI, const FRect& iBdpROI, const glm::vec2& iSubpixelComponent, ufloat iOpacity, const FPerf& iPerf ) {
    uint8* xidt;
    uint8 bpc, ncc, hea, spp, bpp, aid;
    tSize roi_w, roi_h, src_bps, bdp_bps, src_jmp, bdp_jmp;
    XBuildBlendParams( iBdpROI, iSource, iBackdrop, &bpc, &ncc, &hea, &spp, &bpp, &aid, &xidt, &roi_w, &roi_h, &src_bps, &bdp_bps, &src_jmp, &bdp_jmp );
    const tByte*        src = iSource->DataPtr()   + ( iSrcROI.y * src_bps ) + ( iSrcROI.x * bpp );
    tByte*              bdp = iBackdrop->DataPtr() + ( iBdpROI.y * bdp_bps ) + ( iBdpROI.x * bpp );
    const glm::vec2&    sub = iSubpixelComponent;
    glm::vec2           bus = glm::vec2( 1.f ) - iSubpixelComponent;

    switch( _BM ) {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case BM_DISSOLVE: {
            uint32 localPRNGSeed = gBlendingPRNGSeed;
            //  -------------
            //  | m00 | m10 |
            //  |_____|_____|___
            //  | m01 | m11 |
            //  |_____|_____|
            //     |  |  |
            //    vv0 | vv1  -> res
            float m11, m01, m10, m00, vv0, vv1, res;
            for( tSize y = 0; y < roi_w; ++y ) {
                m11 = m10 = vv1 = 0.f;
                for( tSize x = 0; x < roi_h; ++x ) {
                    m00 = m10;
                    m01 = m11;
                    vv0 = vv1;
                    SampleSubpixelAlphaOpt< T >( src, hea, aid, bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, vv0, &m11, &m10, &vv1, &res );
                    const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
                    const float alpha_src       = res * iOpacity;
                    localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                    float toss = ( localPRNGSeed % 65537 ) / 65537.f;
                    if( toss < alpha_src ) {
                        const float alpha_result = AlphaF< _AM >( alpha_src, alpha_bdp );
                        memcpy( bdp, src, bpp );
                        if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );
                    }
                    // Increment ptrs by one pixel
                    src += bpp;
                    bdp += bpp;
                }
                // Increment ptrs jump one line
                src += src_jmp;
                bdp += bdp_jmp;
            }
            break;
        } // BM_DISSOLVE
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case BM_BAYERDITHER8x8: {
            //  -------------
            //  | m00 | m10 |
            //  |_____|_____|___
            //  | m01 | m11 |
            //  |_____|_____|
            //     |  |  |
            //    vv0 | vv1  -> res
            float m11, m01, m10, m00, vv0, vv1, res;
            for( tSize y = 0; y < roi_w; ++y ) {
                m11 = m10 = vv1 = 0.f;
                for( tSize x = 0; x < roi_h; ++x ) {
                    m00 = m10;
                    m01 = m11;
                    vv0 = vv1;
                    SampleSubpixelAlphaOpt< T >( src, hea, aid, bpp, src_bps, x, y, iSrcROI.w, iSrcROI.h, sub, bus, vv0, &m11, &m10, &vv1, &res );
                    const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
                    const float alpha_src       = res * iOpacity;
                    const tSize bayerX = x % 8;
                    const tSize bayerY = y % 8;
                    const float bayerEl = gBayer8x8Matrix[ bayerY ][ bayerX ];
                    if( alpha_src >= bayerEl ) {
                        const float alpha_result = AlphaF< _AM >( alpha_src, alpha_bdp );
                        memcpy( bdp, src, bpp );
                        if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );
                    }
                    // Increment ptrs by one pixel
                    src += bpp;
                    bdp += bpp;
                }
                // Increment ptrs jump one line
                src += src_jmp;
                bdp += bdp_jmp;
            }
            break;
        } // BM_DISSOLVE
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    delete [] xidt;
}

template< typename T, eBlendingMode _BM, eAlphaMode _AM >
void BlendMono_Misc_MEM( const FBlock* iSource, FBlock* iBackdrop, const FRect& iSrcROI, const FRect& iBdpROI, const glm::vec2& iSubpixelComponent, ufloat iOpacity, const FPerf& iPerf ) {
    uint8* xidt;
    uint8 bpc, ncc, hea, spp, bpp, aid;
    tSize roi_w, roi_h, src_bps, bdp_bps, src_jmp, bdp_jmp;
    XBuildBlendParams( iBdpROI, iSource, iBackdrop, &bpc, &ncc, &hea, &spp, &bpp, &aid, &xidt, &roi_w, &roi_h, &src_bps, &bdp_bps, &src_jmp, &bdp_jmp );
    const tByte*        src = iSource->DataPtr()   + ( iSrcROI.y * src_bps ) + ( iSrcROI.x * bpp );
    tByte*              bdp = iBackdrop->DataPtr() + ( iBdpROI.y * bdp_bps ) + ( iBdpROI.x * bpp );

    switch( _BM ) {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case BM_DISSOLVE: {
            uint32 localPRNGSeed = gBlendingPRNGSeed;
            for( tSize y = 0; y < roi_w; ++y ) {
                for( tSize x = 0; x < roi_h; ++x ) {
                    const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
                    const float alpha_src       = hea ? TYPE2FLOAT( src, aid ) * iOpacity : iOpacity;
                    localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                    float toss = ( localPRNGSeed % 65537 ) / 65537.f;
                    if( toss < alpha_src ) {
                        const float alpha_result = AlphaF< _AM >( alpha_src, alpha_bdp );
                        memcpy( bdp, src, bpp );
                        if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );
                    }
                    // Increment ptrs by one pixel
                    src += bpp;
                    bdp += bpp;
                }
                // Increment ptrs jump one line
                src += src_jmp;
                bdp += bdp_jmp;
            }
            break;
        } // BM_DISSOLVE
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case BM_BAYERDITHER8x8: {
            for( tSize y = 0; y < roi_w; ++y ) {
                for( tSize x = 0; x < roi_h; ++x ) {
                    const float alpha_bdp       = hea ? TYPE2FLOAT( bdp, aid ) : 1.f;
                    const float alpha_src       = hea ? TYPE2FLOAT( src, aid ) * iOpacity : iOpacity;
                    const tSize bayerX = x % 8;
                    const tSize bayerY = y % 8;
                    const float bayerEl = gBayer8x8Matrix[ bayerY ][ bayerX ];
                    if( alpha_src >= bayerEl ) {
                        const float alpha_result = AlphaF< _AM >( alpha_src, alpha_bdp );
                        memcpy( bdp, src, bpp );
                        if( hea ) FLOAT2TYPE( bdp, aid, alpha_result );
                    }
                    // Increment ptrs by one pixel
                    src += bpp;
                    bdp += bpp;
                }
                // Increment ptrs jump one line
                src += src_jmp;
                bdp += bdp_jmp;
            }
            break;
        } // BM_DISSOLVE
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }

    delete [] xidt;
}

ULIS2_DELETE_COMP_OP_INSTANCIATION( ULIS2_FOR_ALL_NONSEPARABLE_BM_DO, BlendMono_Misc_MEM )
ULIS2_DELETE_COMP_OP_INSTANCIATION( ULIS2_FOR_ALL_NONSEPARABLE_BM_DO, BlendMono_Misc_MEM_Subpixel )
ULIS2_DELETE_COMP_OP_INSTANCIATION( ULIS2_FOR_ALL_SEPARABLE_BM_DO, BlendMono_Misc_MEM )
ULIS2_DELETE_COMP_OP_INSTANCIATION( ULIS2_FOR_ALL_SEPARABLE_BM_DO, BlendMono_Misc_MEM_Subpixel )

ULIS2_NAMESPACE_END
