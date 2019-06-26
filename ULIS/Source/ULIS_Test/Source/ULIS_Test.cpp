/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS_Test.cpp
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#include <ULIS_CORE>


template< uint32_t _SH >
void
Process_Imp( ::ULIS::TBlock< _SH >* iBlock )
{
    return;
}


void
Process( ::ULIS::IBlock* iBlock )
{
    if( !iBlock ) return;

    uint32_t id = iBlock->Id();
    switch( id )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ::ULIS::ulis_types_reg[ n ]: Process_Imp< ::ULIS::ulis_types_reg[ n ] >( (::ULIS::TBlock< ::ULIS::ulis_types_reg[ n ] >*)iBlock ); break;
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
        default: return;
    }
}


int main()
{
    //::ULIS::PrintSpecs();

    ::ULIS::TPixelValue< ::ULIS::FBlockBGRA8::TypeId() > pixel;
    pixel.SetAlpha( 'A' );
    pixel[0] = 'R';
    pixel[1] = 'G';
    pixel[2] = 'B';
    for( int i = 0; i < pixel.NumChannels(); ++i )
    {
        //std::cout << pixel.RedirectedIndex( i ) << std::endl;
        //std::cout << (int)pixel[i] << std::endl;
        std::cout << pixel.GetRaw(i) << std::endl;
    }

    std::cout << pixel.R() << std::endl;
    std::cout << pixel.G() << std::endl;
    std::cout << pixel.B() << std::endl;
    std::cout << pixel.Alpha() << std::endl;

    ::ULIS::IBlock* block = new ::ULIS::FBlockBGRA8( 200, 200 );
    ::ULIS::IBlock* block2 = new ::ULIS::FBlockBGRA8( 300, 300 );
    std::cout << block->GetMD5Hash() << std::endl;
    std::cout << block2->GetMD5Hash() << std::endl;
    Process( block );
    delete block;

    return 0;
}

