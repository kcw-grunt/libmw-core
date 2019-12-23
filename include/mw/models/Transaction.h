#pragma once

// Copyright (c) 2018-2019 David Burkett
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

#include <mw/crypto/Crypto.h>
#include <mw/models/crypto/Hash.h>
#include <mw/models/crypto/BigInteger.h>
#include <mw/models/crypto/BlindingFactor.h>
#include <mw/models/TransactionBody.h>
#include <mw/traits/Printable.h>
#include <mw/traits/Serializable.h>
#include <mw/traits/Hashable.h>

#include <json/json.h>
#include <memory>
#include <vector>

////////////////////////////////////////
// TRANSACTION
////////////////////////////////////////
class Transaction : public Traits::IPrintable, public Traits::ISerializable, public Traits::IHashable
{
public:
    using CPtr = std::shared_ptr<const Transaction>;

    //
    // Constructors
    //
    Transaction(BlindingFactor&& offset, TransactionBody&& transactionBody)
        : m_offset(std::move(offset)), m_body(std::move(transactionBody))
    {
        Serializer serializer;
        Serialize(serializer);

        m_hash = Crypto::Blake2b(serializer.GetBytes());
    }

    Transaction(const Transaction& transaction) = default;
    Transaction(Transaction&& transaction) noexcept = default;
    Transaction() = default;

    //
    // Destructor
    //
    virtual ~Transaction() = default;

    //
    // Operators
    //
    Transaction& operator=(const Transaction& transaction) = default;
    Transaction& operator=(Transaction&& transaction) noexcept = default;
    bool operator<(const Transaction& transaction) const { return GetHash() < transaction.GetHash(); }
    bool operator==(const Transaction& transaction) const { return GetHash() == transaction.GetHash(); }
    bool operator!=(const Transaction& transaction) const { return GetHash() != transaction.GetHash(); }

    //
    // Getters
    //
    const BlindingFactor& GetOffset() const { return m_offset; }
    const TransactionBody& GetBody() const { return m_body; }
    const std::vector<TransactionInput>& GetInputs() const { return m_body.GetInputs(); }
    const std::vector<TransactionOutput>& GetOutputs() const { return m_body.GetOutputs(); }
    const std::vector<TransactionKernel>& GetKernels() const { return m_body.GetKernels(); }

    //
    // Serialization/Deserialization
    //
    virtual Serializer& Serialize(Serializer& serializer) const override final;
    static Transaction Deserialize(ByteBuffer& byteBuffer);
    Json::Value ToJSON() const;
    static Transaction FromJSON(const Json::Value& transactionJSON);

    //
    // Traits
    //
    virtual std::string Format() const override final { return GetHash().Format(); }
    virtual Hash GetHash() const override final { return m_hash; }

private:
    // The kernel "offset" k2 excess is k1G after splitting the key k = k1 + k2.
    BlindingFactor m_offset;

    // The transaction body.
    TransactionBody m_body;

    mutable Hash m_hash;
};