#pragma once

// Copyright (c) 2018-2019 David Burkett
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

#include <mw/core/serialization/ByteBuffer.h>
#include <mw/core/serialization/Serializer.h>

namespace Traits
{
    class ISerializable
    {
    public:
        virtual ~ISerializable() = default;

        //
        // Appends serialized bytes to Serializer
        //
        virtual Serializer& Serialize(Serializer& serializer) const = 0;
    };
}