/*************************************************************************
*
*   ULIS
*__________________
*
* MainWindow.h
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#include "MainWindow.h"
#include "TriVignette.h"
#include <QApplication>
#include <QScrollArea>
#include <QStyle>
#include <ULIS_CORE>

/////////////////////////////////////////////////////
// cMainWindow
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
cMainWindow::cMainWindow( QWidget* iParent )
    : tSuperClass   ( iParent )
    , mScrollArea   ( nullptr )
    , mCanvas       ( nullptr )
    , mUnderSource  ( nullptr )
    , mOverSource   ( nullptr )
{
    Init();
    Build();
}


cMainWindow::~cMainWindow()
{
    Destroy();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
//virtual
void
cMainWindow::resizeEvent(QResizeEvent *event)
{
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
cMainWindow::Init()
{
    mScrollArea = new  QScrollArea( this );
    mCanvas = new QWidget();

    {
        QImage* tmp = new QImage( "Resources/ULIS_GUI_Blend/Under.png" );
        mUnderSource = ::ULIS::FMakeContext::MakeBlockFromDataPerformCopy( tmp->width(), tmp->height(), tmp->bits(), ::ULIS::FBlockRGBA8::TypeId() );
        delete  tmp;
    }

    {
        QImage* tmp = new QImage( "Resources/ULIS_GUI_Blend/Over.png" );
        mOverSource = ::ULIS::FMakeContext::MakeBlockFromDataPerformCopy( tmp->width(), tmp->height(), tmp->bits(), ::ULIS::FBlockRGBA8::TypeId() );
        delete  tmp;
    }

    int num_blend_modes = (int)::ULIS::eBlendingMode::kNumBlendingModes;
    for( int i = 0; i < num_blend_modes; ++i )
    {
        ::ULIS::IBlock* ulis_display = ::ULIS::FMakeContext::CopyBlock( mUnderSource );
        ::ULIS::FBlendingContext::Blend( mOverSource, ulis_display, ::ULIS::eBlendingMode(i), 1.f );
        QString ps_filename( ::ULIS::kwBlendingMode[i] );
        QString path = "Resources/ULIS_GUI_Blend/PhotoshopResults/" + ps_filename + ".png";
        QImage* tmp = new QImage( path );
        ::ULIS::IBlock* ps_display = ::ULIS::FMakeContext::MakeBlockFromDataPerformCopy( tmp->width(), tmp->height(), tmp->bits(), ::ULIS::FBlockRGBA8::TypeId() );
        delete  tmp;

        vignettes_list.push_back( new cTriVignette( ulis_display, ps_display, mCanvas ) );
    }
}


void
cMainWindow::Build()
{
    setFixedWidth( 600 + qApp->style()->pixelMetric(QStyle::PM_ScrollBarExtent) );
    mScrollArea->setVerticalScrollBarPolicy(    Qt::ScrollBarPolicy::ScrollBarAlwaysOn );
    mScrollArea->setHorizontalScrollBarPolicy(  Qt::ScrollBarPolicy::ScrollBarAlwaysOff );
    mScrollArea->setWidget( mCanvas );
}


void
cMainWindow::Compose()
{
    int w = width();
    int h = height();
    mScrollArea->resize( w, h );
    mCanvas->setGeometry( 0, 0, 600, 150 * vignettes_list.size() );
    for( int i = 0; i < vignettes_list.size(); ++i )
        vignettes_list[i]->setGeometry( 0, i * 150, 600, 150 );
}


void
cMainWindow::Destroy()
{
    for( int i = 0; i < vignettes_list.size(); ++i )
        delete vignettes_list[i];
    vignettes_list.clear();
    if( mCanvas )       delete  mCanvas;
    if( mScrollArea )   delete  mScrollArea;
    if( mUnderSource )  delete  mUnderSource;
    if( mOverSource )   delete  mOverSource;
}
