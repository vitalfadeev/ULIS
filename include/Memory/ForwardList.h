// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         ForwardList.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the TForwardList class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Memory.h"
#include "Math/Math.h"
#include <utility>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TForwardList
/// @brief      The TForwardList class provides a simple dynamic singly
///             linked list class for ULIS interfaces.
/// @details    The need of a custom TForwardList class rose from the fact
///             that std:: classes cannot be exported easily accross dll
///             boundaries when ULIS is compiled as a shared library.
template< typename T >
class TForwardList
{
private:
    /// @class      TForwardListNode
    /// @brief      The TForwardListNode class provides a simple node class
    ///             fot TForwardList
    /// @details    TForwardListNode is really a simple way to store nodes
    ///             for singly linked lists, it holds the element and a pointer
    ///             to the next node.
    template< typename U >
    struct TForwardListNode
    {
        TForwardListNode( TForwardListNode< U >* iNext, const T& iValue )
            : mNext( iNext )
            , mValue( iValue )
        {}

        TForwardListNode( TForwardListNode< U >* iNext, T&& iValue )
            : mNext( iNext )
            , mValue( std::forward< U >( iValue ) )
        {}

        template< class... Args >
        TForwardListNode( TForwardListNode< U >* iNext, Args&& ... args )
            : mNext( iNext )
            , mValue( std::forward< Args >(args)... )
        {}

        TForwardListNode< U >* mNext;
        U mValue;
    };

    typedef TForwardListNode< T > tNode;

public:
    /*! Destroy the list and cleanup memory. */
    ~TForwardList< T >()
    {
        Clear();
    }

    /*! Default constructor ( empty ). */
    TForwardList< T >()
        : mFront( nullptr )
        , mBack( nullptr )
        , mSize( 0 )
    {}

    /*! Copy constructor, explicitely removed. */
    TForwardList< T >( const TForwardList< T >& iOther ) = delete;

    /*! Copy Assignment Operator, explicitely removed. */
    TForwardList< T >& operator=( const TForwardList< T >& iOther ) = delete;

    /*! Move Assignment Operator, explicitely removed. */
    TForwardList< T >& operator=( TForwardList< T >&& iOther ) = delete;

    /*!
        Move constructor.
        The first object is left in a valid but empty state.
        The second object steals the state.
    */
    TForwardList< T >( TForwardList< T >&& iOther )
        : mFront( iOther.mFront )
        , mBack( iOther.mBack )
        , mSize( iOther.mSize )
    {
        iOther.mFront = nullptr;
        iOther.mBack = nullptr;
        iOther.mSize = 0;
    }

    /*!
        Access component at front.
        The behaviour is undefined if the list is empty.
    */
    T& Front() {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( mFront, "Bad state" );
        return  mFront->mValue;
    }

    /*!
        Access const component at front.
        The behaviour is undefined if the list is empty.
    */
    const T& Front() const {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( mFront, "Bad state" );
        return  mFront->mValue;
    }

    /*!
        Access component at back.
        The behaviour is undefined if the list is empty.
    */
    T& Back() {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( mBack, "Bad state" );
        return  mBack->mValue;
    }

    /*!
        Access const component at back.
        The behaviour is undefined if the list is empty.
    */
    const T& Back() const {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( mBack, "Bad state" );
        return  mBack->mValue;
    }

    /*! Returns wether the list is empty or not. */
    bool IsEmpty() const {
        return  mSize == 0;
    }

    /*! Returns the list size. */
    uint64 Size() const {
        return  mSize;
    }

    /*!
        Clear the list, deallocating everything.
    */
    void Clear() {
        tNode* node = mFront;
        while( node ) {
            tNode* next = node->mNext;
            delete  node;
            node = next;
        }
        mSize = 0;
        mFront = nullptr;
        mBack = nullptr;
    }

    /*!
        PushFront, insert a new element at the front of the list.
    */
    void PushFront( const T& iValue ) {
        tNode* node = new tNode( mFront, iValue );
        mFront = node;
        mSize++;

        if( !mBack )
            mBack = mFront;
    }

    /*!
        PushFront, insert a new element at the front of the list.
    */
    void PushFront( T&& iValue ) {
        tNode* node = new tNode( mFront, std::forward< T >( iValue ) );
        mFront = node;
        mSize++;

        if( !mBack )
            mBack = mFront;
    }

    /*!
        EmplaceFront, emplace a new element at front end of the list.
    */
    template< class... Args >
    void EmplaceFront( Args&& ... args ) {
        tNode* node = new tNode( mFront, std::forward< Args >(args)... );
        mFront = node;
        mSize++;

        if( !mBack )
            mBack = mFront;
    }

    /*!
        PopFront, pop the front element.
        The behaviour is undefined is the list is empty.
    */
    void PopFront() {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        tNode* next = mFront->mNext;
        delete  mFront;
        mFront = next;
        mSize--;
        if( mSize <= 1 ) {
            mBack = mFront;
        }
    }

    /*!
        PushBack, insert a new element at the back of the list.
    */
    void PushBack( const T& iValue ) {
        tNode* node = new tNode( nullptr, iValue );
        mSize++;

        if( mBack ) {
            mBack->mNext = node;
        } else {
            mFront = node;
            mBack = node;
        }
    }

    /*!
        PushBack, insert a new element at the back of the list.
    */
    void PushBack( T&& iValue ) {
        tNode* node = new tNode( mFront, std::forward< T >( iValue ) );
        mSize++;

        if( mBack ) {
            mBack->mNext = node;
        } else {
            mFront = node;
            mBack = node;
        }
    }

    /*!
        EmplaceBack, emplace a new element at back of the list.
    */
    template< class... Args >
    void EmplaceBack( Args&& ... args ) {
        tNode* node = new tNode( mFront, std::forward< Args >(args)... );
        mSize++;

        if( mBack ) {
            mBack->mNext = node;
        } else {
            mFront = node;
            mBack = node;
        }
    }

    /*!
        Access component at index.
        The behaviour is undefined if the list has no size.
        The behaviour is undefined if the index is greater than the size.
        The behaviour is undefined if the underlying buffer is NULL, which
        indicates a corrupted states anyway.
    */
    T& operator[]( uint64 iIndex ) {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( iIndex < mSize, "Bad Index" );
        tNode* node = mFront;
        for( uint64 i = 0; i < mSize; ++i )
            node = node->mNext;
        return  node->mValue;
    }

    /*!
        Access const component at index.
        The behaviour is undefined if the list has no size.
        The behaviour is undefined if the index is greater than the size.
        The behaviour is undefined if the underlying buffer is NULL, which
        indicates a corrupted states anyway.
    */
    const T& operator[]( uint64 iIndex ) const {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( iIndex < mSize, "Bad Index" );
        tNode* node = mFront;
        for( uint64 i = 0; i < mSize; ++i )
            node = node->mNext;
        return  node->mValue;
    }

    /*!
        Access component at index. Alias of operator[]()

        \sa operator[]()
    */
    T& At( uint64 iIndex ) {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( iIndex < mSize, "Bad Index" );
        tNode* node = mFront;
        for( uint64 i = 0; i < mSize; ++i )
            node = node->mNext;
        return  node->mValue;
    }

    /*!
        Access component at index. Alias of operator[]()

        \sa operator[]() const
    */
    const T& At( uint64 iIndex ) const {
        ULIS_ASSERT( mSize > 0, "Bad call, list is empty" );
        ULIS_ASSERT( iIndex < mSize, "Bad Index" );
        tNode* node = mFront;
        for( uint64 i = 0; i < mSize; ++i )
            node = node->mNext;
        return  node->mValue;
    }

private:
    tNode* mFront; ///< The head of the list, start iterating from there.
    tNode* mBack; ///< The back of the list. Although traversal is one way, this is here to allow easy push back on the list.
    uint64 mSize; ///< The list size.
};

ULIS_NAMESPACE_END

