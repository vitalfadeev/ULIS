// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         TilePool.ippp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TilePool class.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Data/Sparse/TilePool.h"
#include "Data/Sparse/Tile.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Thread/ThreadPool.h"
#include "Clear/Clear.h"
#include <thread>
#include <algorithm>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// TTilePool
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
template< uint8 _MICRO, uint8 _MACRO > TTilePool< _MICRO, _MACRO >::~TTilePool() {
    PurgeAllNow();
    mThreadPool->WaitForCompletion();
    delete  mThreadPool;
    delete  mHost;
    delete  mEmptyTile;
}

template< uint8 _MICRO, uint8 _MACRO > TTilePool< _MICRO, _MACRO >::TTilePool( tFormat iFormat
                                                                             , FColorProfile* iProfile
                                                                             , uint64 iRAMUsageCapTarget
                                                                             , uint64 iSWAPUsageCapTarget
                                                                             , uint32 iDesiredPoolWorkers
                                                                             , uint32 iTimeOutMS )
    : mPixelDim                                 ( pow( 2, _MICRO )      )
    , mTileSize                                 ( mPixelDim, mPixelDim  )
    , mTileFormat                               ( iFormat               )
    , mEmptyTile                                ( nullptr               )
    , mEmptyHash                                ( 0                     )
    , mSWAPUsageCapTarget                       ( iSWAPUsageCapTarget   )
    , mRAMUsageCapTarget                        ( iRAMUsageCapTarget    )
    , mTileColorProfile                         ( iProfile              )
    , mTilesScheduledForClear_slist             ()
    , mFreshTilesAvailableForQuery_slist        ()
    , mDirtyHashedTilesCurrentlyInUse_dlist     ()
    , mCorrectlyHashedTilesCurrentlyInUse_umap  ()
    , mDirtyTaskIterator                        ( mDirtyHashedTilesCurrentlyInUse_dlist.end() )
    , mThreadPool                               ( nullptr               )
    , mHost                                     ( nullptr               )
    , mTimeOutMS                                ( iTimeOutMS            )
    , mBytesPerTile                             ( 0                     )
    , mCurrentRAMUSage                          ( 0                     )
    , mTickForbidden                            ( false                 )
    , mNumTilesScheduledForClear                ( 0                     )
    , mNumFreshTilesAvailableForQuery           ( 0                     )
    , mAllocatorCleanerWorker_thread            ( nullptr               )
    , mSanitizerCompressorWorker_thread         ( nullptr               )
    , mNumTilesScheduledForClear_atomic         ( 0                     )
    , mNumFreshTilesAvailableForQuery_atomic    ( 0                     )
{
    mThreadPool = new FThreadPool( iDesiredPoolWorkers );
    mHost = new FHostDeviceInfo( FHostDeviceInfo::Detect() );

    mEmptyTile = new FBlock( mPixelDim, mPixelDim, iFormat, iProfile );
    ClearRaw( mEmptyTile, ULIS2_NOCB );
    mEmptyHash = mEmptyTile->CRC32();
    mBytesPerTile = mEmptyTile->BytesTotal();
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
template< uint8 _MICRO, uint8 _MACRO > const FVec2I&        TTilePool< _MICRO, _MACRO >::TileSize()                                 const   { return  mTileSize;                                        }
template< uint8 _MICRO, uint8 _MACRO > uint32               TTilePool< _MICRO, _MACRO >::EmptyHash()                                const   { return  mEmptyHash;                                       }
template< uint8 _MICRO, uint8 _MACRO > const FBlock*        TTilePool< _MICRO, _MACRO >::EmptyTile()                                const   { return  mEmptyTile;                                       }
template< uint8 _MICRO, uint8 _MACRO > tFormat              TTilePool< _MICRO, _MACRO >::TileFormat()                               const   { return  mTileFormat;                                      }
template< uint8 _MICRO, uint8 _MACRO > const FFormatInfo&   TTilePool< _MICRO, _MACRO >::TileFormatInfo()                           const   { return  mEmptyTile->FormatInfo();                         }
template< uint8 _MICRO, uint8 _MACRO > const FColorProfile* TTilePool< _MICRO, _MACRO >::TileColorProfile()                         const   { return  mTileColorProfile;                                }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::CurrentRAMUsage()                          const   { return  mCurrentRAMUSage;                                 }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::CurrentSwapUsage()                         const   { return  0;                                                }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::RAMUsageCapTarget()                        const   { return  mRAMUsageCapTarget;                               }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::SWAPUsageCapTarget()                       const   { return  mSWAPUsageCapTarget;                              }
template< uint8 _MICRO, uint8 _MACRO > uint64               TTilePool< _MICRO, _MACRO >::CurrentTotalMemoryUsage()                  const   { return  mCurrentRAMUSage;                                 }
template< uint8 _MICRO, uint8 _MACRO > long long            TTilePool< _MICRO, _MACRO >::TimeOutMS()                                const   { return  mTimeOutMS.count();                               }
template< uint8 _MICRO, uint8 _MACRO > void                 TTilePool< _MICRO, _MACRO >::SetRAMUsageCapTarget( uint64 iValue )              { mRAMUsageCapTarget  = iValue;                             }
template< uint8 _MICRO, uint8 _MACRO > void                 TTilePool< _MICRO, _MACRO >::SetSWAPUsageCapTarget( uint64 iValue )             { mSWAPUsageCapTarget = iValue;                             }
template< uint8 _MICRO, uint8 _MACRO > void                 TTilePool< _MICRO, _MACRO >::SetTimeOutMS( uint32 iValue )                      { mTimeOutMS = std::chrono::milliseconds( iValue );         }
template< uint8 _MICRO, uint8 _MACRO > size_t               TTilePool< _MICRO, _MACRO >::NumTilesScheduledForClear()                const   { return  mNumTilesScheduledForClear;                       }
template< uint8 _MICRO, uint8 _MACRO > size_t               TTilePool< _MICRO, _MACRO >::NumFreshTilesAvailableForQuery()           const   { return  mNumFreshTilesAvailableForQuery;                  }
template< uint8 _MICRO, uint8 _MACRO > size_t               TTilePool< _MICRO, _MACRO >::NumDirtyHashedTilesCurrentlyInUse()        const   { return  mDirtyHashedTilesCurrentlyInUse_dlist.size();     }
template< uint8 _MICRO, uint8 _MACRO > size_t               TTilePool< _MICRO, _MACRO >::NumCorrectlyHashedTilesCurrentlyInUse()    const   { return  mCorrectlyHashedTilesCurrentlyInUse_umap.size();  }
template< uint8 _MICRO, uint8 _MACRO > size_t               TTilePool< _MICRO, _MACRO >::NumRegisteredTiledBlocks()                 const   { return  mRegisteredTiledBlocks.size();                    }
//----------------------------------------------------------------------------- Core API
template< uint8 _MICRO, uint8 _MACRO >
void
TTilePool< _MICRO, _MACRO >::Tick() {
    if( mTickForbidden )
        return;

    mTickForbidden = true;

    int     num_task = 5;
    auto    micro_delta = mTimeOutMS / num_task;
    auto    span_del1   = micro_delta * 1;
    auto    span_del2   = micro_delta * 2;
    auto    span_new    = micro_delta * 3;
    auto    span_clear  = micro_delta * 4;
    auto    span_wipe   = micro_delta * 5;
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    { // Del
        while( !( mTilesScheduledForClear_slist.empty() ) ) {
            if( mCurrentRAMUSage > mRAMUsageCapTarget ) {
                auto ptr = mTilesScheduledForClear_slist.front();
                mTilesScheduledForClear_slist.pop_front();
                --mNumTilesScheduledForClear;
                delete  ptr;
                mCurrentRAMUSage -= mBytesPerTile;
            }

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - start_time );
            if( elapsed > span_del1 )
                break;
        }

        while( !( mFreshTilesAvailableForQuery_slist.empty() ) ) {
            if( mCurrentRAMUSage > mRAMUsageCapTarget ) {
                auto ptr = mFreshTilesAvailableForQuery_slist.front();
                mFreshTilesAvailableForQuery_slist.pop_front();
                --mNumFreshTilesAvailableForQuery;
                delete  ptr;
                mCurrentRAMUSage -= mBytesPerTile;
            }

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - start_time );
            if( elapsed > span_del2 )
                break;
        }
    }

    { // Alloc
        while( true ) {
            if( mCurrentRAMUSage < mRAMUsageCapTarget ) {
                mTilesScheduledForClear_slist.emplace_front( new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile ) );
                ++mNumTilesScheduledForClear;
                mCurrentRAMUSage += mBytesPerTile;
            }

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - start_time );
            if( elapsed > span_new )
                break;
        }
    }

    { // Clear
        while( !( mTilesScheduledForClear_slist.empty() ) ) {
            auto ptr = mTilesScheduledForClear_slist.front();
            mTilesScheduledForClear_slist.pop_front();
            --mNumTilesScheduledForClear;
            ClearRaw( ptr, false );


            mFreshTilesAvailableForQuery_slist.emplace_front( ptr );
            ++mNumFreshTilesAvailableForQuery;
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - start_time );
            if( elapsed > span_clear )
                break;
        }
    }

    { // Whipe
        while( !( mDirtyHashedTilesCurrentlyInUse_dlist.empty() ) ) {
            mDirtyTaskIterator = mDirtyTaskIterator == mDirtyHashedTilesCurrentlyInUse_dlist.end() ? mDirtyHashedTilesCurrentlyInUse_dlist.begin() : mDirtyTaskIterator;
            auto ptr = *mDirtyTaskIterator;
            if( ptr->mRefCount == 0 ) {
                auto element_to_erase = mDirtyTaskIterator;
                mTilesScheduledForClear_slist.push_front( ptr->mBlock );
                ++mNumTilesScheduledForClear;
                delete ptr;
                ptr = nullptr;
                mDirtyTaskIterator = mDirtyTaskIterator == mDirtyHashedTilesCurrentlyInUse_dlist.end() ? mDirtyHashedTilesCurrentlyInUse_dlist.begin() : std::next( mDirtyTaskIterator );
                mDirtyHashedTilesCurrentlyInUse_dlist.erase( element_to_erase );
            }
            else
            {
                if( ptr->mDirty ) {
                    ptr->mHash = ptr->mBlock->CRC32();
                    if( ptr->mHash == 1799349750 ) {
                        auto dummy = 0;
                    }
                    ptr->mDirty = false;
                }
                mDirtyTaskIterator = std::next( mDirtyTaskIterator );
                mDirtyTaskIterator = mDirtyTaskIterator == mDirtyHashedTilesCurrentlyInUse_dlist.end() ? mDirtyHashedTilesCurrentlyInUse_dlist.begin() : mDirtyTaskIterator;
            }

            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( std::chrono::steady_clock::now() - start_time );
            if( elapsed > span_wipe )
                break;
        }
    }

    mTickForbidden = false;
}


template< uint8 _MICRO, uint8 _MACRO >
void
TTilePool< _MICRO, _MACRO >::PurgeAllNow() {
    mTickForbidden = true;
    // Purge Scheduled For Clear
    for( auto it : mTilesScheduledForClear_slist            ) delete it;
    for( auto it : mFreshTilesAvailableForQuery_slist       ) delete it;

    for( auto it : mDirtyHashedTilesCurrentlyInUse_dlist    ) {
        delete it->mBlock;
        delete it;
    }

    for( auto it : mCorrectlyHashedTilesCurrentlyInUse_umap ) {
        delete it.second->mBlock;
        delete it.second;
    }

    for( auto it : mRegisteredTiledBlocks ) {
        delete it;
    }

    mTilesScheduledForClear_slist.clear();
    mFreshTilesAvailableForQuery_slist.clear();
    mDirtyHashedTilesCurrentlyInUse_dlist.clear();
    mCorrectlyHashedTilesCurrentlyInUse_umap.clear();
    mRegisteredTiledBlocks.clear();
    mCurrentRAMUSage = 0;
    SetRAMUsageCapTarget( 0 );
    SetSWAPUsageCapTarget( 0 );
    mNumTilesScheduledForClear = 0;
    mNumFreshTilesAvailableForQuery = 0;
    mTickForbidden = false;
    mDirtyTaskIterator = mDirtyHashedTilesCurrentlyInUse_dlist.end();
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTilePool< _MICRO, _MACRO >::AllocateNow( uint32 iNum ) {
    mTickForbidden = true;
    for( uint32 i = 0; i < iNum; ++i ) {
        mTilesScheduledForClear_slist.emplace_front( new FBlock( mTileSize.x, mTileSize.y, mTileFormat, mTileColorProfile ) );
        ++mNumTilesScheduledForClear;
        mCurrentRAMUSage += mBytesPerTile;
    }
    mTickForbidden = false;
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTilePool< _MICRO, _MACRO >::ClearNow( uint32 iNum ) {
    mTickForbidden = true;

    if( iNum > mNumTilesScheduledForClear )
        AllocateNow( iNum - mNumTilesScheduledForClear + 1 );

    for( uint32 i = 0; i < iNum; ++i ) {
        auto ptr = mTilesScheduledForClear_slist.front();
        mTilesScheduledForClear_slist.pop_front();
        --mNumTilesScheduledForClear;
        ClearRaw( ptr, false );
        mFreshTilesAvailableForQuery_slist.emplace_front( ptr );
        ++mNumFreshTilesAvailableForQuery;
    }

    mTickForbidden = false;
}

template< uint8 _MICRO, uint8 _MACRO >
FTileElement*
TTilePool< _MICRO, _MACRO >::QueryFreshTile() {
    mTickForbidden = true;
    if( !mNumFreshTilesAvailableForQuery )
        ClearNow( 1 );
    auto ptr = mFreshTilesAvailableForQuery_slist.front();
    mFreshTilesAvailableForQuery_slist.pop_front();
    --mNumFreshTilesAvailableForQuery;
    FTileElement* tile = new FTileElement( ptr );
    mDirtyHashedTilesCurrentlyInUse_dlist.push_back( tile );
    mTickForbidden = false;
    return  tile;
}

template< uint8 _MICRO, uint8 _MACRO >
FTileElement*
TTilePool< _MICRO, _MACRO >::PerformRedundantHashMergeReturnCorrect( FTileElement* iElem ) {
    ULIS2_ASSERT( iElem,                    "Bad Elem Query during Hash Merge Check" );
    ULIS2_ASSERT( iElem->mDirty == false,   "Bad Elem Query during Hash Merge Check" );

    // If the hashed tile is empty we return null ptr and decrease refcount
    if( iElem->mHash == mEmptyHash ) {
        iElem->DecreaseRefCount();
        return  nullptr;
    }

    // Find the hashed tile in the map if not empty
    auto it = mCorrectlyHashedTilesCurrentlyInUse_umap.find( iElem->mHash );

    // If the hashed tile isn't in correct map yet
    // we copy it and insert in correct
    if( it == mCorrectlyHashedTilesCurrentlyInUse_umap.end() ) {
        if( !mNumFreshTilesAvailableForQuery )
            ClearNow( 1 );
        auto ptr = mFreshTilesAvailableForQuery_slist.front();
        mFreshTilesAvailableForQuery_slist.pop_front();
        --mNumFreshTilesAvailableForQuery;
        FTileElement* tile = new FTileElement( ptr );
        tile->mDirty = false;
        tile->mHash = iElem->mHash;
        CopyRaw( iElem->mBlock, tile->mBlock, false );
        mCorrectlyHashedTilesCurrentlyInUse_umap[ tile->mHash ] = tile;
        iElem->DecreaseRefCount();
        tile->IncreaseRefCount();
        return  tile;
    }

    // If it's already the same correct hash, do nothing
    if( it->second == iElem )
        return  iElem;

    // Otherwise, perform merge and decrease ref in dirty.
    it->second->IncreaseRefCount();
    iElem->DecreaseRefCount();
    return  it->second;
}

template< uint8 _MICRO, uint8 _MACRO >
FTileElement*
TTilePool< _MICRO, _MACRO >::PerformDataCopyForImminentMutableChangeIfNeeded( FTileElement* iElem ) {
    ULIS2_ASSERT( iElem,                    "Bad Elem Query during Hash Merge Check" );

    if( !( iElem->mDirty ) ) {
        auto it = mCorrectlyHashedTilesCurrentlyInUse_umap.find( iElem->mHash );
        if( it == mCorrectlyHashedTilesCurrentlyInUse_umap.end() ) {
            if( iElem->mRefCount > 1 ) {
                FTileElement* tile = QueryFreshTile();
                CopyRaw( iElem->mBlock, tile->mBlock, false );
                iElem->DecreaseRefCount();
                tile->IncreaseRefCount();
                return  tile;
            } else {
                iElem->mDirty = true;
                return  iElem;
            }
        } else {
            if( it->second == iElem ) {
                if( iElem->mRefCount > 1 ) {
                    FTileElement* tile = QueryFreshTile();
                    CopyRaw( iElem->mBlock, tile->mBlock, false );
                    iElem->DecreaseRefCount();
                    tile->IncreaseRefCount();
                    return  tile;
                } else {
                    mDirtyHashedTilesCurrentlyInUse_dlist.push_back( iElem );
                    mCorrectlyHashedTilesCurrentlyInUse_umap.erase( it );
                    iElem->mDirty = true;
                    return  iElem;
                }
            } else {
                if( iElem->mRefCount > 1 ) {
                    FTileElement* tile = QueryFreshTile();
                    CopyRaw( iElem->mBlock, tile->mBlock, false );
                    iElem->DecreaseRefCount();
                    tile->IncreaseRefCount();
                    return  tile;
                } else {
                    iElem->mDirty = true;
                    return  iElem;
                }
            }
        }
    }
    else
    {
        if( iElem->mRefCount > 1 ) {
            FTileElement* tile = QueryFreshTile();
            CopyRaw( iElem->mBlock, tile->mBlock, false );
            iElem->DecreaseRefCount();
            tile->IncreaseRefCount();
            return  tile;
        }
        else
        {
            return  iElem;
        }
    }
}

template< uint8 _MICRO, uint8 _MACRO >
TTiledBlock< _MICRO, _MACRO >*
TTilePool< _MICRO, _MACRO >::CreateNewTiledBlock() {
    mRegisteredTiledBlocks.emplace_back( new TTiledBlock< _MICRO, _MACRO >( this ) );
    return  mRegisteredTiledBlocks.back();
}

template< uint8 _MICRO, uint8 _MACRO >
void
TTilePool< _MICRO, _MACRO >::RequestTiledBlockDeletion( TTiledBlock< _MICRO, _MACRO >* iBlock ) {
    auto it = std::find( mRegisteredTiledBlocks.begin(), mRegisteredTiledBlocks.end(), iBlock );
    if( it != mRegisteredTiledBlocks.end() ) {
        delete it;
        mRegisteredTiledBlocks.erase( it );
    }
}

ULIS2_NAMESPACE_END
