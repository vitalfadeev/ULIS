// Copyright � 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS2.
* @copyright    Copyright � 2018-2019 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>
using namespace ::ul2;

int
main()
{
    FBlock block( 256, 256, ULIS2_FORMAT_LabA32 );
    FPixel src( ULIS2_FORMAT_RGB8 );
    src.SetR8( 255 );
    src.SetG8( 255 );
    src.SetB8( 255 );
    FThreadPool pool;
    Fill( pool, &block, src );
    FPixelProxy prox = block.PixelProxy( 0, 0 );
    std::cout << ConvType< uint32, float >( prox.L32() ) << std::endl;
    std::cout << ConvType< uint32, float >( prox.a32() ) << std::endl;
    std::cout << ConvType< uint32, float >( prox.b32() ) << std::endl;
    std::cout << ConvType< uint32, float >( prox.A32() ) << std::endl;
    auto dummy = 0;
}

