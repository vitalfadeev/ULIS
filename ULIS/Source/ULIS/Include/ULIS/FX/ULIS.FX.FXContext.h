/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS.FX.FXContext.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/
#pragma once
#include "ULIS/Base/ULIS.Base.BaseTypes.h"
#include "ULIS/Base/ULIS.Base.PerformanceOptions.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/FX/Noise/ULIS.FX.Noise.ValueNoise.h"
#include "ULIS/FX/Noise/ULIS.FX.Noise.WhiteNoise.h"
#include "ULIS/FX/Noise/ULIS.FX.Noise.BrownianNoise.h"
#include "ULIS/FX/Noise/ULIS.FX.Noise.VoronoiNoise.h"

namespace ULIS {
/////////////////////////////////////////////////////
// TFXContext
template< uint32 _SH >
class TFXContext
{
public:
    static void WhiteNoise( TBlock< _SH >* iBlock, int iSeed = -1, const FPerformanceOptions& iPerformanceOptions= FPerformanceOptions() )
    {
        assert( iBlock );
        TWhiteNoiseGenerator< _SH >::Run( iBlock, iSeed, iPerformanceOptions );
    }

    static void ValueNoise( TBlock< _SH >* iBlock, float iFrequency, int iSeed = -1, const FPerformanceOptions& iPerformanceOptions= FPerformanceOptions() )
    {
        assert( iBlock );
        TValueNoiseGenerator< _SH >::Run( iBlock, iFrequency, iSeed, iPerformanceOptions );
    }

    static  void  BrownianNoise( TBlock< _SH >* iBlock, float iFrequency = 0.22f, float iFrequencyMult = 1.8f, float iAmplitudeMult = 0.35f, uint8 iNumLayers = 5, int iSeed = -1, const FPerformanceOptions& iPerformanceOptions= FPerformanceOptions(), bool iCallInvalidCB = true )
    {
        assert( iBlock );
        TBrownianNoiseGenerator< _SH >::Run( iBlock, iFrequency, iFrequencyMult, iAmplitudeMult, iNumLayers, iSeed, iPerformanceOptions );
    }

    static void VoronoiNoise( TBlock< _SH >* iBlock, uint32 iCount, int iSeed = -1, const FPerformanceOptions& iPerformanceOptions= FPerformanceOptions() )
    {
        assert( iBlock );
        TVoronoiNoiseGenerator< _SH >::Run( iBlock, iCount, iSeed, iPerformanceOptions );
    }

};

} // namespace ULIS

