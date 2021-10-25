// Copyright (c) 2017-2021 The Dash Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_LLMQ_SNAPSHOT_H
#define BITCOIN_LLMQ_SNAPSHOT_H

#include <evo/evodb.h>
#include <evo/simplifiedmns.h>
#include <saltedhasher.h>
#include <serialize.h>

namespace llmq
{

class CQuorumSnapshot
{
public:
    std::vector<bool> activeQuorumMembers;
    int mnSkipListMode;
    std::vector<int> mnSkipList;

    CQuorumSnapshot() = default;
    explicit CQuorumSnapshot(std::vector<bool> _activeQuorumMembers, int _mnSkipListMode, std::vector<int> _mnSkipList) :
            activeQuorumMembers(std::move(_activeQuorumMembers)),
            mnSkipListMode(_mnSkipListMode),
            mnSkipList(std::move(_mnSkipList))
    {
    }

    template <typename Stream, typename Operation>
    inline void SerializationOpBase(Stream& s, Operation ser_action)
    {
        READWRITE(mnSkipListMode);
    }

    template<typename Stream>
    void Serialize(Stream& s) const
    {
        const_cast<CQuorumSnapshot*>(this)->SerializationOpBase(s, CSerActionSerialize());

        WriteCompactSize(s, activeQuorumMembers.size());
        WriteFixedBitSet(s, activeQuorumMembers, activeQuorumMembers.size());
        WriteCompactSize(s, mnSkipList.size());
        for (const auto& obj : mnSkipList) {
            s << obj;
        }
    }

    template<typename Stream>
    void Unserialize(Stream& s) {
        SerializationOpBase(s, CSerActionUnserialize());

        size_t cnt = {};
        cnt = ReadCompactSize(s);
        ReadFixedBitSet(s, activeQuorumMembers, cnt);
        cnt = ReadCompactSize(s);
        mnSkipList.resize(cnt);
        for (size_t i = 0; i < cnt; i++) {
            int obj;
            s >> obj;
            mnSkipList.push_back(obj);
        }
    }

    void ToJson(UniValue& obj) const;
};

class CGetQuorumRotationInfo
{
public:
    uint baseBlockHashesNb;
    std::vector<uint256> baseBlockHashes;
    uint256 blockRequestHash;

    SERIALIZE_METHODS(CGetQuorumRotationInfo, obj)
    {
        READWRITE(obj.baseBlockHashesNb, obj.baseBlockHashes, obj.blockRequestHash);
    }
};

class CQuorumRotationInfo
{
public:
    int creationHeight;
    CQuorumSnapshot quorumSnaphotAtHMinusC;
    CQuorumSnapshot quorumSnaphotAtHMinus2C;
    CQuorumSnapshot quorumSnaphotAtHMinus3C;
    CSimplifiedMNListDiff mnListDiffTip;
    CSimplifiedMNListDiff mnListDiffAtHMinusC;
    CSimplifiedMNListDiff mnListDiffAtHMinus2C;
    CSimplifiedMNListDiff mnListDiffAtHMinus3C;

    SERIALIZE_METHODS(CQuorumRotationInfo, obj)
    {
        READWRITE(obj.creationHeight,
                  obj.quorumSnaphotAtHMinusC,
                  obj.quorumSnaphotAtHMinus2C,
                  obj.quorumSnaphotAtHMinus3C,
                  obj.mnListDiffTip,
                  obj.mnListDiffAtHMinusC,
                  obj.mnListDiffAtHMinus2C,
                  obj.mnListDiffAtHMinus3C);
    }

    CQuorumRotationInfo() = default;
    explicit CQuorumRotationInfo(const CQuorumRotationInfo& dmn) {}

    void ToJson(UniValue& obj) const;
};

bool BuildQuorumRotationInfo(const CGetQuorumRotationInfo& request, CQuorumRotationInfo& quorumRotationInfoRet, std::string& errorRet);
uint256 GetLastBaseBlockHash(const std::vector<const CBlockIndex*>& baseBlockIndexes, const CBlockIndex* blockIndex);

class CQuorumSnapshotManager
{
private:
    CCriticalSection cs;

    CEvoDB& evoDb;

    std::unordered_map<uint256, CQuorumSnapshot, StaticSaltedHasher> quorumSnapshotCache;

public:
    explicit CQuorumSnapshotManager(CEvoDB& _evoDb);

    bool GetSnapshotForBlock(const Consensus::LLMQType llmqType, const CBlockIndex* pindex, CQuorumSnapshot& snapshot);
    void StoreSnapshotForBlock(const Consensus::LLMQType llmqType, const CBlockIndex* pindex, const CQuorumSnapshot& snapshot);
};

extern std::unique_ptr<CQuorumSnapshotManager> quorumSnapshotManager;

} // namespace llmq

#endif //BITCOIN_LLMQ_SNAPSHOT_H
