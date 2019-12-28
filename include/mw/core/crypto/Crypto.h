#pragma once

// Copyright (c) 2018-2019 David Burkett
// Distributed under the MIT software license, see the accompanying
// file LICENSE or http://www.opensource.org/licenses/mit-license.php.

#include <mw/core/common/ImportExport.h>
#include <mw/core/common/Secure.h>
#include <mw/core/models/crypto/BigInteger.h>
#include <mw/core/models/crypto/Commitment.h>
#include <mw/core/models/crypto/RangeProof.h>
#include <mw/core/models/crypto/BlindingFactor.h>
#include <mw/core/models/crypto/Signature.h>
#include <mw/core/models/crypto/ProofMessage.h>
#include <mw/core/models/crypto/RewoundProof.h>
#include <mw/core/models/crypto/Hash.h>
#include <mw/core/models/crypto/PublicKey.h>
#include <mw/core/models/crypto/SecretKey.h>
#include <mw/core/models/crypto/ScryptParameters.h>

#ifdef MW_CRYPTO
#define CRYPTO_API EXPORT
#else
#define CRYPTO_API IMPORT
#endif

//
// Exported class that serves as a lightweight, easy-to-use wrapper for secp256k1-zkp and other crypto dependencies.
//
class CRYPTO_API Crypto
{
public:
    //
    // Uses Blake2b to hash the given input into a 32 byte hash.
    //
    static BigInt<32> Blake2b(const std::vector<uint8_t>& input);

    //
    // Uses Blake2b to hash the given input into a 32 byte hash using a key.
    //
    static BigInt<32> Blake2b(
        const std::vector<uint8_t>& key,
        const std::vector<uint8_t>& input
    );

    //
    // Uses SHA256 to hash the given input into a 32 byte hash.
    //
    static BigInt<32> SHA256(const std::vector<uint8_t>& input);

    //
    // Uses SHA512 to hash the given input into a 64 byte hash.
    //
    static BigInt<64> SHA512(const std::vector<uint8_t>& input);

    //
    // Uses RipeMD160 to hash the given input into a 20 byte hash.
    //
    static BigInt<20> RipeMD160(const std::vector<uint8_t>& input);

    //
    //
    //
    static BigInt<32> HMAC_SHA256(
        const std::vector<uint8_t>& key,
        const std::vector<uint8_t>& data
    );

    //
    //
    //
    static BigInt<64> HMAC_SHA512(
        const std::vector<uint8_t>& key,
        const std::vector<uint8_t>& data
    );

    //
    // Creates a pedersen commitment from a value with a zero blinding factor.
    //
    static Commitment CommitTransparent(const uint64_t value);

    //
    // Creates a pedersen commitment from a value with the supplied blinding factor.
    //
    static Commitment CommitBlinded(
        const uint64_t value,
        const BlindingFactor& blindingFactor
    );

    //
    // Adds the homomorphic pedersen commitments together.
    //
    static Commitment AddCommitments(
        const std::vector<Commitment>& positive,
        const std::vector<Commitment>& negative
    );

    //
    // Takes a vector of blinding factors and calculates an additional blinding value that adds to zero.
    //
    static BlindingFactor AddBlindingFactors(
        const std::vector<BlindingFactor>& positive,
        const std::vector<BlindingFactor>& negative
    );

    //
    //
    //
    static RangeProof GenerateRangeProof(
        const uint64_t amount,
        const SecretKey& key,
        const SecretKey& privateNonce,
        const SecretKey& rewindNonce,
        const ProofMessage& proofMessage
    );

    //
    //
    //
    static std::unique_ptr<RewoundProof> RewindRangeProof(
        const Commitment& commitment,
        const RangeProof& rangeProof,
        const SecretKey& nonce
    );

    //
    //
    //
    static bool VerifyRangeProofs(
        const std::vector<std::pair<Commitment, RangeProof>>& rangeProofs
    );

    //
    //
    //
    static bool VerifyKernelSignatures(
        const std::vector<const Signature*>& signatures,
        const std::vector<const Commitment*>& publicKeys,
        const std::vector<const Hash*>& messages
    );

    //
    //
    //
    static uint64_t SipHash24(
        const uint64_t k0,
        const uint64_t k1,
        const std::vector<uint8_t>& data
    );

    //
    // Encrypts the input with AES256 using the given key.
    //
    static std::vector<uint8_t> AES256_Encrypt(
        const SecureVector& input,
        const SecretKey& key,
        const BigInt<16>& iv
    );

    //
    // Decrypts the input with AES256 using the given key.
    //
    static SecureVector AES256_Decrypt(
        const std::vector<uint8_t>& ciphertext,
        const SecretKey& key,
        const BigInt<16>& iv
    );

    //
    // Uses Scrypt to hash the given password and the given salt. It then blake2b hashes the output.
    // The returned hash will be a 32 byte SecretKey.
    //
    static SecretKey PBKDF(
        const SecureString& password,
        const std::vector<uint8_t>& salt,
        const ScryptParameters& parameters
    );

    //
    // Calculates the 33 byte public key from the 32 byte private key using curve secp256k1.
    //
    static PublicKey CalculatePublicKey(const SecretKey& privateKey);

    //
    // Adds a number of public keys together.
    // Returns the combined public key if successful.
    //
    static PublicKey AddPublicKeys(const std::vector<PublicKey>& publicKeys);

    //
    // Hashes the message and signs it using the secret key.
    // If successful, returns a compact Signature.
    //
    static CompactSignature::UPtr SignMessage(
        const SecretKey& secretKey,
        const PublicKey& publicKey,
        const std::string& message
    );

    //
    // Verifies the signature is a valid signature for the hash of the message.
    // Returns true if valid.
    //
    static bool VerifyMessageSignature(
        const CompactSignature& signature,
        const PublicKey& publicKey,
        const std::string& message
    );

    //
    // Builds one party's share of a Schnorr signature.
    // Returns a CompactSignature if successful.
    //
    static CompactSignature::UPtr CalculatePartialSignature(
        const SecretKey& secretKey,
        const SecretKey& secretNonce,
        const PublicKey& sumPubKeys,
        const PublicKey& sumPubNonces,
        const Hash& message
    );

    //
    // Verifies one party's share of a Schnorr signature.
    // Returns true if valid.
    //
    static bool VerifyPartialSignature(
        const CompactSignature& partialSignature,
        const PublicKey& publicKey,
        const PublicKey& sumPubKeys,
        const PublicKey& sumPubNonces,
        const Hash& message
    );

    //
    // Combines multiple partial signatures to build the final aggregate signature.
    // Returns the raw aggregate signature.
    //
    static Signature::UPtr AggregateSignatures(
        const std::vector<CompactSignature>& signatures,
        const PublicKey& sumPubNonces
    );

    //
    // Verifies that the signature is a valid signature for the message.
    //
    static bool VerifyAggregateSignature(
        const Signature& aggregateSignature,
        const PublicKey sumPubKeys,
        const Hash& message
    );

    //
    // Calculates the blinding factor x' = x + SHA256(xG+vH | xJ), used in the switch commitment x'G+vH.
    //
    static SecretKey BlindSwitch(
        const SecretKey& secretKey,
        const uint64_t amount
    );

    //
    // Adds 2 private keys together.
    //
    static SecretKey AddPrivateKeys(
        const SecretKey& secretKey1,
        const SecretKey& secretKey2
    );

    static SecretKey GenerateSecureNonce();
};