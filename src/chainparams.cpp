// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the GPL3 software license, see the accompanying
// file COPYING or http://www.gnu.org/licenses/gpl.html.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
* Main network
*/

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
* What makes a good checkpoint block?
* + Is surrounded by blocks with reasonable timestamps
*   (no blocks before with a timestamp after, none after with
*    timestamp before)
* + Contains no strange transactions
*/
static Checkpoints::MapCheckpoints mapCheckpoints =
boost::assign::map_list_of
(  0, uint256("000027036a0c08bcc5705a8025481ddd6905fd6bd03567019f1844034b09b0c7"))
;
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1508535053, // * UNIX timestamp of last checkpoint block
    0,     // * total number of transactions between genesis and last checkpoint
    //   (the tx=... number in the SetBestChain debug.log lines)
    0       // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
boost::assign::map_list_of
(  0, uint256("0000580a864fa09315c854185d7bbbb78260a69bf5a087e690b1c9c2526fe4da"))
;
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1508535054,
    0,
    0
};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
boost::assign::map_list_of
( 0, uint256("0000be40e539f5f59e2a77321defe531f9b6d590370b73e49003432dbba32618"))
;
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1508535055,
    0,
    0
};

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
        * The message start string is designed to be unlikely to occur in normal data.
        * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        * a large 4-byte int at any alignment.
        */
        pchMessageStart[0] = 0x11;
        pchMessageStart[1] = 0x22;
        pchMessageStart[2] = 0x33;
        pchMessageStart[3] = 0x44;
        nDefaultPort = 6161;
        bnProofOfWorkLimit = ~uint256(0) >> 16;
        bnInitialHashTarget = ~uint256(0) >> 16;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 24 * 60 * 60; // one day
        nTargetSpacing = 8 * 60;           // 8 minutes. affects network code only

        // ppcoin: PoS spacing = nStakeTargetSpacing
        //         PoW spacing = depends on how much PoS block are between last two PoW blocks, with maximum value = nTargetSpacingMax
        nCoinbaseMaturity = 32;                       // coinbase transaction outputs can only be spent after this number of new blocks
        nStakeTargetSpacing = 8 * 60;                // 8 minutes
        nTargetSpacingMax = 12 * nStakeTargetSpacing; // ~2 hours
        nStakeMinAge = 60 * 60;             // minimum age for coin age
        nStakeMaxAge = 60 * 60 * 24 * 180;             // stake age of full weight
        nStakeModifierInterval = 60;         // time to elapse before new modifier is computed
        nMaxTipAge = 14 * 24 * 60 * 60;

        /**
        * Build the genesis block. Note that the output of the genesis coinbase cannot
        * be spent as it did not originally exist in the database.
        *
        * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
        *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
        *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
        *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
        *   vMerkleTree: 4a5e1e
        */
        const char* pszTimestamp = "Enterneko 21/10/2017";
        std::vector<CTxIn> vin;
        vin.resize(1);
        vin[0].scriptSig = CScript() << 486604799 << CScriptNum(9999) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        std::vector<CTxOut> vout;
        vout.resize(1);
        vout[0].nValue = 0;
        vout[0].scriptPubKey.clear();
        CMutableTransaction txNew(1, 1508535050, vin, vout, 0);
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1508535053;
        genesis.nBits    = 0x1f00ffff;
        genesis.nNonce   = 24340;

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("000027036a0c08bcc5705a8025481ddd6905fd6bd03567019f1844034b09b0c7"));
        assert(genesis.hashMerkleRoot == uint256("8db586d27703f1736c8c41322745ed82acd81fa7fab0e7215b7e1b0899277496"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(33);   // neko: addresses begin with 'E'
        base58Prefixes[SCRIPT_ADDRESS] = list_of(92);   // neko: addresses begin with 'e'
        base58Prefixes[SECRET_KEY] =     list_of(128);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        vSeeds.push_back(CDNSSeedData("enterneko.com", "seed.enterneko.com"));
        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nBIP34Height = 0;
        nBIP65Height = 0;
        nBIP66Height = 0;
        nMMHeight = 0;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
* Testnet (v3)
*/
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xbb;
        pchMessageStart[1] = 0xf1;
        pchMessageStart[2] = 0xc9;
        pchMessageStart[3] = 0xef;
        nDefaultPort = 6163;
        bnProofOfWorkLimit = ~uint256(0) >> 5;
        bnInitialHashTarget = ~uint256(0) >> 5;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nCoinbaseMaturity = 1;
        nStakeMinAge = 60;        // test net min age is 1 day
        nStakeModifierInterval = 60 * 20;   // test net modifier interval is 20 minutes
        nMaxTipAge = 0x7fffffff;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nNonce = 26039;
        genesis.nTime    = 1508535054;
        genesis.nBits    = 0x1f00ffff;
        if (false) {

            /*
            new mainnet genesis hash: CBlock(hash=0000580a864fa09315c854185d7bbbb78260a69bf5a087e690b1c9c2526fe4da, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=8db586d27703f1736c8c41322745ed82acd81fa7fab0e7215b7e1b0899277496, nTime=1508535054, nBits=1f00ffff, nNonce=26039, vtx=1)
            Coinbase(hash=8db586d277, nTime=1508535050, ver=1, vin.size=1, vout.size=1, nLockTime=0)
            CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d020f2714456e7465726e656b6f2032312f31302f32303137)
            CTxOut(nValue=0.000000, scriptPubKey=)

            vMerkleTree:  8db586d27703f1736c8c41322745ed82acd81fa7fab0e7215b7e1b0899277496

            */
            LogPrintf("%s\n","recalculating params for mainnet.\n");
            LogPrintf("old mainnet genesis nonce: %s\n", genesis.ToString().c_str());
            uint256 hashTarget = uint256().SetCompact(0x1f00ffff);
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce = 0; genesis.GetHash() > hashTarget; genesis.nNonce++){ }

            LogPrintf("new mainnet genesis hash: %s\n", genesis.ToString().c_str());

        }

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0000580a864fa09315c854185d7bbbb78260a69bf5a087e690b1c9c2526fe4da"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(239);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nBIP34Height = 0;
        nBIP65Height = 0;
        nBIP66Height = 0;
        nMMHeight = 0;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
* Regression test
*/
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xcb;
        pchMessageStart[1] = 0xf2;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xef;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        //genesis.nTime = 1296688602;
        //genesis.nBits = 0x207fffff;
        //genesis.nNonce = 2;
        nMaxTipAge = 24 * 60 * 60;

        genesis.nTime    = 1508535055;
        genesis.nNonce = 41490;
        genesis.nBits    = 0x1f00ffff;
        if (false) {

            /*

            new mainnet genesis hash: CBlock(hash=0000ab5744714f3d85eec0f8eabee1cebb851fe39a335ec8f432f7303b42ea23, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=8db586d27703f1736c8c41322745ed82acd81fa7fab0e7215b7e1b0899277496, nTime=1508535055, nBits=1f00ffff, nNonce=41490, vtx=1)
            Coinbase(hash=8db586d277, nTime=1508535050, ver=1, vin.size=1, vout.size=1, nLockTime=0)
            CTxIn(COutPoint(0000000000, 4294967295), coinbase 04ffff001d020f2714456e7465726e656b6f2032312f31302f32303137)
            CTxOut(nValue=0.000000, scriptPubKey=)

            vMerkleTree:  8db586d27703f1736c8c41322745ed82acd81fa7fab0e7215b7e1b0899277496


            */
            LogPrintf("%s\n","recalculating params for mainnet.\n");
            LogPrintf("old mainnet genesis nonce: %s\n", genesis.ToString().c_str());
            uint256 hashTarget = uint256().SetCompact(0x1f00ffff);
            // deliberately empty for loop finds nonce value.
            for(genesis.nNonce = 0; genesis.GetHash() > hashTarget; genesis.nNonce++){ }

            LogPrintf("new mainnet genesis hash: %s\n", genesis.ToString().c_str());

        }
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 6164;
        assert(hashGenesisBlock == uint256("0000ab5744714f3d85eec0f8eabee1cebb851fe39a335ec8f432f7303b42ea23"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
* Unit test
*/
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 6665;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams==&unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
        return mainParams;
        case CBaseChainParams::TESTNET:
        return testNetParams;
        case CBaseChainParams::REGTEST:
        return regTestParams;
        case CBaseChainParams::UNITTEST:
        return unitTestParams;
        default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
    return false;

    SelectParams(network);
    return true;
}
