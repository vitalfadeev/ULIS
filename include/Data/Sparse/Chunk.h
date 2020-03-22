// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Chunk.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TTiledBlock class.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Copy/Copy.h"
#include "Raster/Sparse/DrawSparse.h"
#include <static_math/static_math.h>

ULIS2_NAMESPACE_BEGIN
static const FPixelValue default_wireframe_debug_color = FPixelValue( ULIS2_FORMAT_RGB8, { 40, 80, 220 } );
static const FPixelValue dirty_wireframe_debug_color = FPixelValue( ULIS2_FORMAT_RGB8, { 255, 0, 0 } );
static const FPixelValue correct_wireframe_debug_color = FPixelValue( ULIS2_FORMAT_RGB8, { 0, 255, 0 } );
/////////////////////////////////////////////////////
// Enums
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------- eChunkType
enum class eChunkType : uint8
{
      kData
    , kRoot
    , kQuadree
};

//----------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------- eScale
enum eMicro {
    MICRO_8   = 3,
    MICRO_16  = 4,
    MICRO_32  = 5,
    MICRO_64  = 6,
    MICRO_128 = 7,
    MICRO_256 = 8,
};

enum eMacro {
    MACRO_2   = 1,
    MACRO_4   = 2,
    MACRO_8   = 3,
    MACRO_16  = 4,
    MACRO_32  = 5,
    MACRO_64  = 6,
    MACRO_128 = 7,
    MACRO_256 = 8,
};
/////////////////////////////////////////////////////
/// FDecl
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TAbstractChunk;
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TRootChunk;
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TDataChunk;
template< uint8 _MICRO, uint8 _MACRO, uint8 _LOCAL > class TQuadtreeChunk;

/////////////////////////////////////////////////////
/// TAbstractChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class ULIS2_API TAbstractChunk
{
    typedef  TAbstractChunk< _MICRO, _MACRO, _LOCAL >   tSelf;
    typedef TTilePool< _MICRO, _MACRO >                 tTilePool;

public:
    // Construction / Destruction
    virtual ~TAbstractChunk() {}
    TAbstractChunk() {}

public:
    // Public API
    virtual  eChunkType  Type()  const = 0;
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const = 0;
    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) = 0;
    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) = 0;

protected:
    // Protected Data Members
    static constexpr uint8  micro_threshold                     = _MICRO;
    static constexpr uint8  macro_threshold                     = _MACRO;
    static constexpr uint8  local_threshold                     = _LOCAL;
    static constexpr uint8  sub_threshold                       = _LOCAL - 1;
    static constexpr uint16 micro_chunk_size_as_pixels          = ::smath::pow( 2, micro_threshold );
    static constexpr uint16 local_chunk_size_as_micro_chunks    = ::smath::pow( 2, local_threshold );
    static constexpr uint32 local_chunk_size_as_pixels          = micro_chunk_size_as_pixels * local_chunk_size_as_micro_chunks;
    static constexpr uint32 local_chunk_halfsize_as_pixels      = local_chunk_size_as_pixels / 2;
    static constexpr bool   bed                                 = _LOCAL == 1;
};

/////////////////////////////////////////////////////
/// TAbstractChunk
template< uint8 _MICRO, uint8 _MACRO >
class ULIS2_API TAbstractChunk< _MICRO, _MACRO, 0 >
{
    typedef  TAbstractChunk< _MICRO, _MACRO, 0 >    tSelf;
    typedef TTilePool< _MICRO, _MACRO >             tTilePool;

public:
    // Construction / Destruction
    virtual ~TAbstractChunk() {}
    TAbstractChunk() {}

public:
    // Public API
    virtual  eChunkType  Type()  const = 0;
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const = 0;
    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) = 0;
    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) = 0;

protected:
    // Protected Data Members
    static constexpr uint8  micro_threshold                     = _MICRO;
    static constexpr uint8  macro_threshold                     = _MACRO;
    static constexpr uint8  local_threshold                     = 0;
    static constexpr uint8  sub_threshold                       = 0;
    static constexpr uint16 micro_chunk_size_as_pixels          = ::smath::pow( 2, micro_threshold );
    static constexpr uint16 local_chunk_size_as_micro_chunks    = ::smath::pow( 2, local_threshold );
    static constexpr uint32 local_chunk_size_as_pixels          = micro_chunk_size_as_pixels * local_chunk_size_as_micro_chunks;
    static constexpr uint32 local_chunk_halfsize_as_pixels      = local_chunk_size_as_pixels / 2;
    static constexpr bool   bed                                 = false;
};

/////////////////////////////////////////////////////
/// TRootChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class ULIS2_API TRootChunk : public TAbstractChunk< _MICRO, _MACRO, _LOCAL >
{
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >    tSuperClass;
    typedef  TRootChunk<        _MICRO, _MACRO, _LOCAL >    tSelf;
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >    tChild;
    typedef  TDataChunk<        _MICRO, _MACRO, _LOCAL >    tDataChild;
    typedef  TQuadtreeChunk<    _MICRO, _MACRO, _LOCAL >    tQuadtreeChild;
    typedef TTilePool< _MICRO, _MACRO >                     tTilePool;

public:
    // Construction / Destruction
    virtual ~TRootChunk() {
        if( mChild )
            delete  mChild;
    }
    TRootChunk()
        : mChild( nullptr )
    {}

public:
    // Public API
    virtual  eChunkType  Type()  const override { return  eChunkType::kRoot; }
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const override {
        return  mChild ? iPool->EmptyTile() : mChild->QueryConstBlockAtPixelCoordinates( iPool, iPos );
    }

    void ReplaceChild( tChild* iNewChild ) {
        if( mChild )
            delete  mChild;
        mChild = iNewChild;
    }

    void PerformRootSubdivisionForImminentMutableChangeIfNeeded( tTilePool* iPool ) {
        if( mChild == nullptr )
            mChild = bed ? dynamic_cast< tChild* >( new tDataChild( iPool->QueryFreshTile() ) ) : dynamic_cast< tChild* >( new tQuadtreeChild() );
        else
            if( mChild->Type() == eChunkType::kData && !bed )
                ReplaceChild( new tQuadtreeChild( dynamic_cast< tDataChild* >( mChild )->PointedData() ) );
    }

    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) override {
        PerformRootSubdivisionForImminentMutableChangeIfNeeded( iPool );
        return  mChild->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( iPool, iPos );
    }

    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) override {
        auto size = round( local_chunk_size_as_pixels * iScale );
        DrawRectOutlineNoAA( iDst, default_wireframe_debug_color, FRect( iPos.x, iPos.y, size, size ) );
        if( mChild )
            mChild->DrawDebugWireframe( iDst, iPos, iScale );
    }

private:
    // Private Data Members
    tChild* mChild;
};


/////////////////////////////////////////////////////
/// TDataChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class ULIS2_API TDataChunk : public TAbstractChunk< _MICRO, _MACRO, _LOCAL >
{
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >    tSuperClass;
    typedef  TDataChunk<        _MICRO, _MACRO, _LOCAL >    tSelf;
    typedef TTilePool< _MICRO, _MACRO >                 tTilePool;

public:
    // Construction / Destruction
    virtual ~TDataChunk() {
        mPtr->mRefCount--;
    }

    TDataChunk()
        : mPtr( nullptr )
    {}

    TDataChunk( FTileElement* iDistributedValue )
        : mPtr( iDistributedValue )
    {
        mPtr->mRefCount++;
    }

public:
    // Public API
    virtual  eChunkType  Type()  const override { return  eChunkType::kData; }
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const override {
        return  mPtr->mBlock;
    }

    void PerformDataCopyForImminentMutableChangeIfNeeded( tTilePool* iPool ) {
        if( mPtr->mRefCount > 1 ) {
            FTileElement* tile = iPool->QueryFreshTile();
            CopyRaw( mPtr->mBlock, tile->mBlock, false );
            mPtr->mRefCount--;
            mPtr = tile;
            mPtr->mRefCount++;
        }
    }

    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) override {
        PerformDataCopyForImminentMutableChangeIfNeeded( iPool );
        mPtr->mDirty = true;
        return  &mPtr;
    }

    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) override {
        auto size = round( local_chunk_size_as_pixels * iScale );
        if( mPtr->mDirty )
            DrawRectOutlineNoAA( iDst, dirty_wireframe_debug_color, FRect( iPos.x, iPos.y, size, size ) );
        else
            DrawRectOutlineNoAA( iDst, correct_wireframe_debug_color, FRect( iPos.x, iPos.y, size, size ) );
    }

    FTileElement* PointedData() {
        return  mPtr;
    }

private:
    // Private Data Members
    FTileElement* mPtr;
};

/////////////////////////////////////////////////////
/// TQuadtreeChunk
template< uint8 _MICRO
        , uint8 _MACRO
        , uint8 _LOCAL >
class ULIS2_API TQuadtreeChunk : public TAbstractChunk< _MICRO, _MACRO, _LOCAL >
{
    typedef  TAbstractChunk<    _MICRO, _MACRO, _LOCAL >                        tSuperClass;
    typedef  TQuadtreeChunk<    _MICRO, _MACRO, _LOCAL >                        tSelf;
    typedef  TAbstractChunk<    _MICRO, _MACRO, tSuperClass::sub_threshold >    tSubAbstractChunk;
    typedef  TQuadtreeChunk<    _MICRO, _MACRO, tSuperClass::sub_threshold >    tSubQuadtreeChunk;
    typedef  TDataChunk<        _MICRO, _MACRO, tSuperClass::sub_threshold >    tSubDataChunk;
    typedef TTilePool< _MICRO, _MACRO >                                         tTilePool;

public:
    // Construction / Destruction
    virtual ~TQuadtreeChunk() {
        for( int i = 0; i < 4; ++i )
            if( mQuad[i] )
                delete  mQuad[i];
    }

    TQuadtreeChunk()
        : mQuad{ nullptr, nullptr, nullptr, nullptr }
    {
    }

    TQuadtreeChunk( FTileElement* iDistributedValue )
        : mQuad{ nullptr, nullptr, nullptr, nullptr }
    {
        for( int i = 0; i < 4; ++i )
            mQuad[i] = new  tSubDataChunk( iDistributedValue );
        iDistributedValue->mRefCount += 4;
    }

private:
    // Coordinates API
    FVec2I64  SubChunkCoordinatesFromLocalPixelCoordinates( const FVec2I64& iPos )  const { return  iPos / local_chunk_halfsize_as_pixels; }
    FVec2I64  LocalPixelCoordinatesFromSubChunkCoordinates( const FVec2I64& iPos )  const { return  iPos * local_chunk_halfsize_as_pixels; }

    uint8 IndexFromSubChunkCoordinates( const FVec2I64& iPos )  const {
        return  iPos.y * 2  + iPos.x;
    }

    FVec2I64 SubChunkCoordinatesFromIndex( uint8 iIndex )  const {
        return  FVec2I64( iIndex & 0x1, ( iIndex & 0x2 ) >> 1 );
    }

public:
    // Public API
    virtual  eChunkType  Type()  const override { return  eChunkType::kRoot; }
    virtual  const FBlock* QueryConstBlockAtPixelCoordinates( const tTilePool* iPool, const FVec2I64& iPos ) const override {
        uint8 index = IndexFromSubChunkCoordinates( SubChunkCoordinatesFromLocalPixelCoordinates( iPos ) );
        return  mQuad[index] == nullptr ? iPool->EmptyTile() : mQuad[index]->QueryConstBlockAtPixelCoordinates( iPool, iPos % local_chunk_halfsize_as_pixels );
    }

    void ReplaceElement( uint8 iIndex, tSubAbstractChunk* iValue ) {
        if( mQuad[iIndex] )
            delete  mQuad[iIndex];
        mQuad[iIndex] = iValue;
    }

    void PerformElementSubdivisionForImminentMutableChangeIfNeeded( uint8 iIndex, tTilePool* iPool ) {
        if( mQuad[iIndex] == nullptr )
            mQuad[iIndex] = bed ? dynamic_cast< tSubAbstractChunk* >( new tSubDataChunk( iPool->QueryFreshTile() ) ) : dynamic_cast< tSubAbstractChunk* >( new tSubQuadtreeChunk() );
        else
            if( mQuad[iIndex]->Type() == eChunkType::kData && !bed )
                ReplaceElement( iIndex, new tSubQuadtreeChunk( dynamic_cast< tSubDataChunk* >( mQuad[iIndex] )->PointedData() ) );
    }

    virtual  FTileElement** QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( tTilePool* iPool, const FVec2I64& iPos ) override {
        uint8 index = IndexFromSubChunkCoordinates( SubChunkCoordinatesFromLocalPixelCoordinates( iPos ) );
        PerformElementSubdivisionForImminentMutableChangeIfNeeded( index, iPool );
        return  mQuad[index]->QueryOneMutableTileElementForImminentDirtyOperationAtPixelCoordinates( iPool, iPos % local_chunk_halfsize_as_pixels );
    }

    virtual  void DrawDebugWireframe( FBlock* iDst, const FVec2I64& iPos, float iScale ) override {
        auto size  = round( local_chunk_size_as_pixels * iScale );
        auto hsize = round( local_chunk_halfsize_as_pixels * iScale );
        DrawRectOutlineNoAA( iDst, default_wireframe_debug_color, FRect( iPos.x, iPos.y, size, size ) );
        for( int i = 0; i < 4; ++i )
            if( mQuad[i] )
                mQuad[i]->DrawDebugWireframe( iDst, iPos + SubChunkCoordinatesFromIndex( i ) * hsize, iScale );
    }

private:
    // Private Data Members
    tSubAbstractChunk* mQuad[4];
};

ULIS2_NAMESPACE_END

