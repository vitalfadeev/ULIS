// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS3.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ul3;

int
main( int argc, char *argv[] ) {
    FThreadPool  threadPool;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = 0 | ULIS3_PERF_TSPEC | ULIS3_PERF_SSE42 | ULIS3_PERF_AVX2;

    std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
    FBlock* blockSRC = XLoadFromFile( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, path, ULIS3_FORMAT_RGBA8 );
    FBlock* blockDST = new  FBlock( 1200, 1000, ULIS3_FORMAT_RGBA8 );
    Clear( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockDST, blockDST->Rect() );

    float angle = FMaths::kPIf + FMaths::kPIf / 128;
    glm::mat3 rotation      = MakeRotationMatrix( angle );
    glm::mat3 translation   = MakeTranslationMatrix( 400, 400 );
    glm::mat3 scale         = MakeScaleMatrix( 4, 4 );
    FTransform2D transform( ComposeMatrix( scale, ComposeMatrix( rotation, translation ) ) );
    TransformAffine( &threadPool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, blockSRC, blockDST, blockSRC->Rect(), transform, INTERP_BILINEAR );
    DrawRectOutlineNoAA( blockDST, FPixelValue( ULIS3_FORMAT_RGB8, { 255, 0, 255 } ), blockSRC->Rect().TransformedAffine( transform ) );
    //DrawRectOutlineNoAA( blockDST, FPixelValue( ULIS3_FORMAT_RGB8, { 0, 127, 255 } ), TransformAffineMetrics( blockSRC->Rect(), transform, INTERP_BILINEAR ) );

    delete  blockSRC;

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

    return  exit_code;
}
