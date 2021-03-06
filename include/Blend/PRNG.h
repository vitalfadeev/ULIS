// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         PRNG.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Blend PRNG entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend PRNG for pseudo random modes like Dissolve
/*! Reset the PRNG Seed for pseudo random Blend modes like Dissolve. */
ULIS_API void ResetBlendPRNGSeed();

/*! Set the PRNG Seed for pseudo random Blend modes like Dissolve. */
ULIS_API void SetBlendPRNGSeed( uint32 iVal );

/*! Get the PRNG Seed for pseudo random Blend modes like Dissolve. */
ULIS_API uint32 GetBlendPRNGSeed();

/*! Generate a pseudo random number for Blend modes like Dissolve. */
ULIS_API uint32 GenerateBlendPRNG();

ULIS_NAMESPACE_END

