#ifndef BECH32_H
#define BECH32_H

#include <string>
#include <vector>
#include <cassert>

typedef std::vector<uint8_t> data;

enum class Encoding {
    INVALID, //!< Failed decoding

    BECH32,  //!< Bech32 encoding as defined in BIP173
    BECH32M, //!< Bech32m encoding as defined in BIP350
};

template<typename V>
inline V Cat(V v1, V&& v2);

/** Concatenate two vectors. */
template<typename V>
inline V Cat(V v1, const V& v2);

template<int frombits, int tobits, bool pad, typename O, typename I>
bool ConvertBits(const O& outfn, I it, I end);

/** Expand a HRP for use in checksum computation. */
data ExpandHRP(const std::string& hrp);

uint32_t EncodingConstant(Encoding encoding);

uint32_t PolyMod(const data& v);

/** Create a checksum. */
data CreateChecksum(Encoding encoding, const std::string& hrp, const data& values);

/** Encode a Bech32 or Bech32m string. */
std::string EncodeBech32SUB(Encoding encoding, const std::string& hrp, const data& values);

std::string EncodeBech32(Encoding encoding, const std::string& hrp, const data& witprog);

#endif