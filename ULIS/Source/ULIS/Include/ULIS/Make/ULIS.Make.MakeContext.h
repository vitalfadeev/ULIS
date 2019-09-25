/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.Make.MakeContext.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#pragma once

#include <assert.h>
#include "ULIS/Base/ULIS.Base.BaseTypes.h"
#include "ULIS/Base/ULIS.Base.PerformanceOptions.h"
#include "ULIS/Make/ULIS.Make.BlockCopier.h"
#include "ULIS/Data/ULIS.Data.Block.h"

namespace ULIS {
/////////////////////////////////////////////////////
// Defines
#define tSpec TBlockInfo< _SH >

/////////////////////////////////////////////////////
// TMakeContext
template< uint32 _SH >
class TMakeContext
{
public:
    // Copy
    static void CopyBlockInto( const TBlock< _SH >* iSrc, TBlock< _SH >* iDst, const FPerformanceOptions& iPerformanceOptions= FPerformanceOptions() )
    {
        CopyBlockRectInto( iSrc, iDst, FRect( 0, 0, iSrc->Width(), iSrc->Height() ), iPerformanceOptions);
    }

    static void CopyBlockRectInto( const TBlock< _SH >* iSrc, TBlock< _SH >* iDst, const FRect& iRect, const FPerformanceOptions& iPerformanceOptions= FPerformanceOptions() )
    {
        FRect src_bb = FRect( 0, 0, iSrc->Width(), iSrc->Height() );
        FRect rect_bb = FRect( iRect.x, iRect.y, FMath::Min( iDst->Width(), iRect.w ), FMath::Min( iDst->Height(), iRect.h ) );
        FRect inter_bb  = src_bb & rect_bb;
        if( inter_bb.Area() <= 0 ) return;
        FPoint shift( -iRect.x, -iRect.y );

        TBlockCopier< _SH >::Run( iSrc, iDst, inter_bb, shift, iPerformanceOptions);
    }
};

/////////////////////////////////////////////////////
// Undefines
#undef tSpec

} // namespace ULIS