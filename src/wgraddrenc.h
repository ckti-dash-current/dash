// Copyright (c) 2017 The Bitcoin developers
// Copyright (c) 2018 The ION Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef BITCOIN_WGRADDRENC_H
#define BITCOIN_WGRADDRENC_H

#include "script/standard.h"
#include "consensus/tokengroups.h"

#include <string>
#include <vector>

class CChainParams;
//class CTokenGroupID;

enum WgrAddrType : uint8_t
{
    PUBKEY_TYPE = 0,
    SCRIPT_TYPE = 1,
    GROUP_TYPE = 2,
};

std::string EncodeWgrAddr(const CTxDestination &, const CChainParams &);
std::string EncodeWgrAddr(const std::vector<uint8_t> &id, const WgrAddrType addrtype, const CChainParams &params);

struct WgrAddrContent
{
    WgrAddrType type;
    std::vector<uint8_t> hash;
};

CTxDestination DecodeWgrAddr(const std::string &addr, const CChainParams &params);
WgrAddrContent DecodeWgrAddrContent(const std::string &addr, const CChainParams &params);
CTxDestination DecodeWgrAddrDestination(const WgrAddrContent &content);

std::vector<uint8_t> PackWgrAddrContent(const WgrAddrContent &content);
#endif
