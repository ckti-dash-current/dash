// Copyright (c) 2017 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include <dstencode.h>
#include <key_io.h>
#include <wgraddrenc.h>
#include <chainparams.h>
//#include <config.h>
#include <script/standard.h>

std::string EncodeDestination(const CTxDestination &dst, const CChainParams &params/*, const Config &cfg*/)
{
    //return cfg.UseCashAddrEncoding() ? EncodeWgrAddr(dst, params) : EncodeLegacyAddr(dst, params);
    return EncodeLegacyAddr(dst, params);
}

CTxDestination DecodeDestination(const std::string &addr, const CChainParams &params)
{
    CTxDestination dst = DecodeWgrAddr(addr, params);
    if (IsValidDestination(dst))
    {
        return dst;
    }
    return DecodeLegacyAddr(addr, params);
}

bool IsValidDestinationString(const std::string &addr, const CChainParams &params)
{
    return IsValidDestination(DecodeDestination(addr, params));
}

std::string EncodeDestination(const CTxDestination &dst) { return EncodeDestination(dst, Params()/*, GetConfig()*/); }
CTxDestination DecodeDestination(const std::string &addr) { return DecodeDestination(addr, Params()); }
bool IsValidDestinationString(const std::string &addr) { return IsValidDestinationString(addr, Params()); }
