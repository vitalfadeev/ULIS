// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ul2;

int
main( int argc, char *argv[] ) {
    FThreadPool  threadPool;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = 0 | ULIS2_PERF_TSPEC | ULIS2_PERF_SSE42 | ULIS2_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
    FBlock* blockSRC = XLoadFromFile( &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB, path, ULIS2_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 1024, 1024, ULIS2_FORMAT_RGBA8 );
    Clear( &threadPool, ULIS2_BLOCKING, perfIntent, host, ULIS2_NOCB, blockDST, blockDST->Rect() );

    int size = 512;
    FVec2F A( 0, 0 );
    FVec2F B( size, 0 );
    FVec2F C( size, size );
    FVec2F D( 0, size/2 );
    float pi = FMaths::kPIf;
    float len0 = 150.f;
    float len1 = 140.f;
    FVec2F vacw  = FVec2F( cos( -pi/4 ), sin( -pi/4 ) )     * len0;
    FVec2F vaccw = FVec2F( cos( pi/4 ), sin( pi/4 ) )       * len1;
    FVec2F vbcw  = FVec2F( cos( pi/4 ), sin( pi/4 ) )       * len0;
    FVec2F vbccw = FVec2F( cos( 3*pi/4 ), sin( 3*pi/4 ) )   * len1;
    FVec2F vccw  = FVec2F( cos( 3*pi/4 ), sin( 3*pi/4 ) )   * len0;
    FVec2F vcccw = FVec2F( cos( -3*pi/4 ), sin( -3*pi/4 ) ) * len1;
    FVec2F vdcw  = FVec2F( cos( -3*pi/4 ), sin( -3*pi/4 ) ) * len0;
    FVec2F vdccw = FVec2F( cos( -pi/4 ), sin( -pi/4 ) )     * len1;
    FVec2F acw  = A + vacw;
    FVec2F accw = A + vaccw;
    FVec2F bcw  = B + vbcw;
    FVec2F bccw = B + vbccw;
    FVec2F ccw  = C + vccw;
    FVec2F cccw = C + vcccw;
    FVec2F dcw  = D + vdcw;
    FVec2F dccw = D + vdccw;
    FVec2F* table[] = { &A, &B, &C, &D, &acw, &accw, &bcw, &bccw, &ccw, &cccw, &dcw, &dccw };
    FRect bounds = FRect::FromMinMax( FMaths::VMin( acw.x, accw.x, bcw.x, bccw.x, ccw.x, cccw.x, dcw.x, dccw.x )
                                    , FMaths::VMin( acw.y, accw.y, bcw.y, bccw.y, ccw.y, cccw.y, dcw.y, dccw.y )
                                    , FMaths::VMax( acw.x, accw.x, bcw.x, bccw.x, ccw.x, cccw.x, dcw.x, dccw.x )
                                    , FMaths::VMax( acw.y, accw.y, bcw.y, bccw.y, ccw.y, cccw.y, dcw.y, dccw.y ) );
    FRect dstRect = blockDST->Rect();
    FVec2F dstSize( dstRect.w, dstRect.h );
    FVec2F objSize( bounds.w, bounds.h );
    FVec2F shift = ( dstSize - objSize ) / 2;
    for( auto i : table ) *i += shift;

    std::vector< FBezierLUTElement > lutAD;
    std::vector< FBezierLUTElement > lutBC;
    float threshold = 1.5;
    CubicBezierGenerateLinearLUT( &lutAD, A, accw, dcw, D, threshold );
    CubicBezierGenerateLinearLUT( &lutBC, B, bcw, cccw, C, threshold );
    auto fTraceBezier = []( const std::vector< FBezierLUTElement >& iLUT, FBlock * iDst, float iT ) {
        for( int i = 0; i < iLUT.size(); ++i ) {
            FVec2F P = iLUT[i].point;
            FPixelProxy prox00 = iDst->PixelProxy( P.x, P.y );
            FPixelProxy prox10 = iDst->PixelProxy( P.x+1, P.y );
            FPixelProxy prox11 = iDst->PixelProxy( P.x+1, P.y+1 );
            FPixelProxy prox01 = iDst->PixelProxy( P.x, P.y+1 );
            float r = sqrtf( iLUT[i].length / iLUT.back().length );
            prox00.SetRF( r );
            prox00.SetGF( iT );
            prox00.SetAlphaF( 1.0 );
            prox10.SetRF( r );
            prox10.SetGF( iT );
            prox10.SetAlphaF( 1.0 );
            prox11.SetRF( r );
            prox11.SetGF( iT );
            prox11.SetAlphaF( 1.0 );
            prox01.SetRF( r );
            prox01.SetGF( iT );
            prox01.SetAlphaF( 1.0 );
        }
    };

    struct FBezierWrapper {
        const FVec2F& P0;
        const FVec2F& P1;
        const FVec2F& P2;
        const FVec2F& P3;
    };

    FBezierWrapper AD { A, accw, dcw, D };
    FBezierWrapper BC { B, bcw, cccw, C };

    const int max = FMaths::Max( lutAD.size(), lutBC.size() );
    const float maxf = static_cast< float >( max );
    for( int i = 0; i < max; ++i ) {
        float v = i / maxf;
        int iv0 = static_cast< int >( floor( v * lutAD.size() ) );
        int iv1 = static_cast< int >( floor( v * lutBC.size() ) );
        FVec2F V0 = lutAD[iv0].point;
        FVec2F V1 = lutBC[iv1].point;
        FVec2F _vv0 = vacw * (  1 -v ) + vdccw * (v);
        FVec2F _vv1 = vbccw * ( 1 -v ) + vccw * (v);
        FVec2F _v0 = V0 + _vv0;
        FVec2F _v1 = V1 + _vv1;

        std::vector< FBezierLUTElement > lutTemp;
        CubicBezierGenerateLinearLUT( &lutTemp, V0, _v0, _v1, V1, threshold );
        fTraceBezier( lutTemp, blockDST, v );

    }

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockDST->DataPtr()
                                        , blockDST->Width()
                                        , blockDST->Height()
                                        , blockDST->BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();

    delete  label;
    delete  image;
    delete  widget;

    delete  blockDST;
    delete  blockSRC;

    return  exit_code;
}

