// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2020 The PIVX developers
// Copyright (c) 2020-2020 The VSYNC developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "chainparamsseeds.h"
#include "consensus/merkle.h"
#include "util.h"
#include "utilstrencodings.h"

#include <boost/assign/list_of.hpp>

#include <assert.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.nVersion = nVersion;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of the genesis coinbase cannot
 * be spent as it did not originally exist in the database.
 *
 *  CBlock(hash=000005bd970b7d83eb879472fb48b2c01ed8155d7126ac3e0c201755c0c85c23, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=848a9da56df8cf4057ef911b7a1837048bea98c1e782b6eff779fd0325c77530, nTime=1594502306, nBits=1e0ffff0, nNonce=4438800, vtx=1)
 *    CTransaction(hash=848a9da56d, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *      CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295), coinbase 04ffff001d01044c4c323032302d30372d31312054686520776f726c64277320666972737420636172626f6e206372656469742067656e65726174696e6720626c6f636b636861696e206279205a696f4661627279)
 *      CTxOut(nValue=0.00000000, scriptPubKey=4104678afdb0fe5548271967f1a671)
 **/
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Vsync 27-08-2017";
    const CScript genesisOutputScript = CScript() << ParseHex("0421fb0665876637d9d79d03a24f383393838fb9e3340858a6d5a70b079f4af57cfff3ca00310be5300d532adf6261ba98ac70d24d943a6be333bec6d7a6d93013") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
	(     0, uint256("00000a47e5f67b18cc1bd58c9e50b5295370cc36df1245a2cd07bf6bb2486e72"))
    (299491, uint256("6edd1b17ab2855ad409c87662c17d418385f4bb27b9d6b6aa87ae0968eff5398"))
    (299492, uint256("384318efe10e5ca05d4270154b7f11b4269d888e6da24bb47f2206292803f081"))
    (1434249, uint256("566c66a2b4fc9a8465045970be8ab7f9d1b5cc5d44af496db2129534a36e9463"))
    (1434250, uint256("beb62c1c9c5e9802c62b3ff1dd5de734b3635f37868765b72d07f51980238aca"))
    (1438350, uint256("0758c1037c646714a222c0d70ba7d4ea38aa551e48ced0682d76138764ccd2f1"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1598195706, // * UNIX timestamp of last checkpoint block
    2667557,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the UpdateTip debug.log lines)
    2880        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256S("0x001"))
    ;
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1594502306,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256S("0x001"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1594502306,
    0,
    100};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";

        genesis = CreateGenesisBlock(1503858596, 525433, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000a47e5f67b18cc1bd58c9e50b5295370cc36df1245a2cd07bf6bb2486e72"));
        assert(genesis.hashMerkleRoot == uint256S("0x55e07427d0ca8579ca7296908f464699df9a54782d28bda012656c9d33a3a3c4"));

        consensus.fPowAllowMinDifficultyBlocks           = false;
        consensus.powLimit                               = ~UINT256_ZERO >> 20;    // VSYNC starting difficulty is 1 / 2^12
        consensus.posLimitV1                             = ~UINT256_ZERO >> 24;
        consensus.posLimitV2                             = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks                     = 43200;                  // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations                = 6;                      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity                      = 100;
        consensus.nFutureTimeDriftPoW                    = 7200;
        consensus.nFutureTimeDriftPoS                    = 180;
        consensus.nMasternodeCountDrift                  = 20;                     // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut                           = 10000000000 * COIN;
        consensus.nPoolMaxTransactions                   = 3;
        consensus.nProposalEstablishmentTime             = 60 * 60 * 24;           // must be at least a day old to make it into a budget
        consensus.nStakeMinAge                           = 60 * 60;
        consensus.nStakeMinDepth                         = 600;
        consensus.nTargetTimespan                        = 40 * 60;
        consensus.nTargetTimespanV2                      = 30 * 60;
        consensus.nTargetSpacing                         = 1 * 60;
        consensus.nTimeSlotLength                        = 15;

        // spork keys
        consensus.strSporkPubKey                         = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
        consensus.strSporkPubKeyOld                      = "";
        consensus.nTime_EnforceNewSporkKey               = 0;
        consensus.nTime_RejectOldSporkKey                = 0;

        // height-based activations
        consensus.height_last_ZC_AccumCheckpoint         = INT_MAX;
        consensus.height_last_ZC_WrappedSerials          = INT_MAX;
        consensus.height_start_InvalidUTXOsCheck         = 350000;
        consensus.height_start_ZC_InvalidSerials         = INT_MAX;
        consensus.height_start_ZC_SerialRangeCheck       = 1;
        consensus.height_ZC_RecalcAccumulators           = INT_MAX;

        // validation by-pass
        consensus.nVsyncBadBlockTime                      = 1520433443;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nVsyncBadBlockBits                      = 0x1c083484;    // Skip nBit validation of Block 259201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus                             = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                                                           "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                                                           "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                                                           "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                                                           "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                                                           "31438167899885040445364023527381951378636564391212010397122822120720357";

        consensus.ZC_MaxPublicSpendsPerTx                = 637;         // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx                      = 7;           // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations                = 20;
        consensus.ZC_MinMintFee                          = 1 * CENT;
        consensus.ZC_MinStakeDepth                       = 200;
        consensus.ZC_TimeStart                           = 1524672000;
        consensus.ZC_WrappedSerialsSupply                = 0 * COIN;    // zerocoin supply at height_last_ZC_WrappedSerials

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight          = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight     = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 259201;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 1;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 299492;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = INT_MAX;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = 1485050;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = INT_MAX;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 1485070;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 1485100;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight      = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock          = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock       = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock       = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock   = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock        = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock        = uint256S("0x1");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x21;
        pchMessageStart[1] = 0x55;
        pchMessageStart[2] = 0x0a;
        pchMessageStart[3] = 0x5a;
        nDefaultPort       = 65010;

        vSeeds.push_back(CDNSSeedData("vsyncseed.vsync.io", "vsyncseed.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node.vsync.io", "node.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node1.vsync.io", "node1.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node2.vsync.io", "node2.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node3.vsync.io", "node3.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node4.vsync.io", "node4.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node5.vsync.io", "node5.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node6.vsync.io", "node6.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node7.vsync.io", "node7.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node8.vsync.io", "node8.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node9.vsync.io", "node9.vsync.io"));
        vSeeds.push_back(CDNSSeedData("node10.vsync.io", "node10.vsync.io"));

        base58Prefixes[PUBKEY_ADDRESS]  = std::vector<unsigned char>(1, 70);        // Start with 'V' from https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS]  = std::vector<unsigned char>(1, 13);        // Start with '6' from https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1, 63);        // Start with 'S' from https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SECRET_KEY]      = std::vector<unsigned char>(1, 212);        // Start with '2' from https://en.bitcoin.it/wiki/List_of_address_prefixes

        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x13)(0x00)(0x00)(0x80).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
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
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        /**
         *  CBlock(hash=000005bd970b7d83eb879472fb48b2c01ed8155d7126ac3e0c201755c0c85c23, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=848a9da56df8cf4057ef911b7a1837048bea98c1e782b6eff779fd0325c77530, nTime=1594502306, nBits=1e0ffff0, nNonce=4438800, vtx=1)
         *    CTransaction(hash=848a9da56d, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *      CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295), coinbase 04ffff001d01044c4c323032302d30372d31312054686520776f726c64277320666972737420636172626f6e206372656469742067656e65726174696e6720626c6f636b636861696e206279205a696f4661627279)
         *      CTxOut(nValue=0.00000000, scriptPubKey=4104678afdb0fe5548271967f1a671)
         */
        genesis = CreateGenesisBlock(1594502306, 4438800, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000005bd970b7d83eb879472fb48b2c01ed8155d7126ac3e0c201755c0c85c23"));
        assert(genesis.hashMerkleRoot     == uint256S("0x848a9da56df8cf4057ef911b7a1837048bea98c1e782b6eff779fd0325c77530"));

        consensus.fPowAllowMinDifficultyBlocks           = false;
        consensus.powLimit                               = ~UINT256_ZERO >> 20;    // VSYNC starting difficulty is 1 / 2^12
        consensus.posLimitV1                             = ~UINT256_ZERO >> 24;
        consensus.posLimitV2                             = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks                     = 43200;                  // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations                = 6;                      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity                      = 100;
        consensus.nFutureTimeDriftPoW                    = 7200;
        consensus.nFutureTimeDriftPoS                    = 180;
        consensus.nMasternodeCountDrift                  = 20;                     // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut                           = 10191520 * COIN;
        consensus.nPoolMaxTransactions                   = 3;
        consensus.nProposalEstablishmentTime             = 60 * 60 * 24;           // must be at least a day old to make it into a budget
        consensus.nStakeMinAge                           = 60 * 60;
        consensus.nStakeMinDepth                         = 600;
        consensus.nTargetTimespan                        = 40 * 60;
        consensus.nTargetTimespanV2                      = 30 * 60;
        consensus.nTargetSpacing                         = 1 * 60;
        consensus.nTimeSlotLength                        = 15;

        // spork keys
        consensus.strSporkPubKey                         = "049704039ba627d532b2737bfa83c5d65e964e472207432c52114981001a369de959343e61435c540f832d5297708b062e832d47efa0624966c5283fdd6c38cd9d";
        consensus.strSporkPubKeyOld                      = "";
        consensus.nTime_EnforceNewSporkKey               = 0;
        consensus.nTime_RejectOldSporkKey                = 0;

        // height-based activations
        consensus.height_last_ZC_AccumCheckpoint         = 0;
        consensus.height_last_ZC_WrappedSerials          = 0;
        consensus.height_start_InvalidUTXOsCheck         = 999999999;
        consensus.height_start_ZC_InvalidSerials         = 0;
        consensus.height_start_ZC_SerialRangeCheck       = 999999999;
        consensus.height_ZC_RecalcAccumulators           = 999999999;

        // validation by-pass
        consensus.nVsyncBadBlockTime                      = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nVsyncBadBlockBits                      = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus                             = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                                                           "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                                                           "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                                                           "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                                                           "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                                                           "31438167899885040445364023527381951378636564391212010397122822120720357";

        consensus.ZC_MaxPublicSpendsPerTx                = 637;         // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx                      = 7;           // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations                = 20;
        consensus.ZC_MinMintFee                          = 1 * CENT;
        consensus.ZC_MinStakeDepth                       = 200;
        consensus.ZC_TimeStart                           = 1508214600;  // October 17, 2017 4:30:00 AM
        consensus.ZC_WrappedSerialsSupply                = 0 * COIN;    // zerocoin supply at height_last_ZC_WrappedSerials

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight          = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight     = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 5001;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 5010;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 5050;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 5100;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 5200;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 5300;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 5400;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight      = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock          = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock       = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock       = uint256S("0x000002c598d8323498d1e491f1d59ce4622e67d276bdd1398ff39cd7f67b63bf");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock   = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock        = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock        = uint256S("0x1");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0x8c;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xfa;
        pchMessageStart[3] = 0xe7;
        nDefaultPort       = 31815;

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.push_back(CDNSSeedData("cri.eco", "testnet.seed.cri.eco", true));                        // Official DNS Seeder
        vSeeds.push_back(CDNSSeedData("ziofabry.twt.it", "mainnet.vsync.seed.ziofabry.twt.it", true));   // Secondary DNS Seeder from ZioFabry

        base58Prefixes[PUBKEY_ADDRESS]  = std::vector<unsigned char>(1, 88);        // Start with 'c' from https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SCRIPT_ADDRESS]  = std::vector<unsigned char>(1, 11);        // Start with '5' from https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[STAKING_ADDRESS] = std::vector<unsigned char>(1,125);        // Start with 's' from https://en.bitcoin.it/wiki/List_of_address_prefixes
        base58Prefixes[SECRET_KEY]      = std::vector<unsigned char>(1,105);        // Start with 'j' from https://en.bitcoin.it/wiki/List_of_address_prefixes

        base58Prefixes[EXT_PUBLIC_KEY]  = boost::assign::list_of(0x04)(0x2C)(0x25)(0x34).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]  = boost::assign::list_of(0x04)(0x1C)(0x33)(0x2C).convert_to_container<std::vector<unsigned char> >();

        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE]   = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();   // 0x80000001

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));
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
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";

        /**
         *  CBlock(hash=000005bd970b7d83eb879472fb48b2c01ed8155d7126ac3e0c201755c0c85c23, ver=1, hashPrevBlock=0000000000000000000000000000000000000000000000000000000000000000, hashMerkleRoot=848a9da56df8cf4057ef911b7a1837048bea98c1e782b6eff779fd0325c77530, nTime=1594502306, nBits=1e0ffff0, nNonce=4438800, vtx=1)
         *    CTransaction(hash=848a9da56d, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *      CTxIn(COutPoint(0000000000000000000000000000000000000000000000000000000000000000, 4294967295), coinbase 04ffff001d01044c4c323032302d30372d31312054686520776f726c64277320666972737420636172626f6e206372656469742067656e65726174696e6720626c6f636b636861696e206279205a696f4661627279)
         *      CTxOut(nValue=0.00000000, scriptPubKey=4104678afdb0fe5548271967f1a671)
         */
        genesis = CreateGenesisBlock(1594502306, 4438800, 0x1e0ffff0, 1, 0 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x000005bd970b7d83eb879472fb48b2c01ed8155d7126ac3e0c201755c0c85c23"));
        assert(genesis.hashMerkleRoot     == uint256S("0x848a9da56df8cf4057ef911b7a1837048bea98c1e782b6eff779fd0325c77530"));

        consensus.fPowAllowMinDifficultyBlocks           = false;
        consensus.powLimit                               = ~UINT256_ZERO >> 20;    // VSYNC starting difficulty is 1 / 2^12
        consensus.posLimitV1                             = ~UINT256_ZERO >> 24;
        consensus.posLimitV2                             = ~UINT256_ZERO >> 20;
        consensus.nBudgetCycleBlocks                     = 43200;                  // approx. 1 every 30 days
        consensus.nBudgetFeeConfirmations                = 6;                      // Number of confirmations for the finalization fee
        consensus.nCoinbaseMaturity                      = 100;
        consensus.nFutureTimeDriftPoW                    = 7200;
        consensus.nFutureTimeDriftPoS                    = 180;
        consensus.nMasternodeCountDrift                  = 20;                     // num of MN we allow the see-saw payments to be off by
        consensus.nMaxMoneyOut                           = 10191520 * COIN;
        consensus.nPoolMaxTransactions                   = 3;
        consensus.nProposalEstablishmentTime             = 60 * 60 * 24;           // must be at least a day old to make it into a budget
        consensus.nStakeMinAge                           = 60 * 60;
        consensus.nStakeMinDepth                         = 600;
        consensus.nTargetTimespan                        = 40 * 60;
        consensus.nTargetTimespanV2                      = 30 * 60;
        consensus.nTargetSpacing                         = 1 * 60;
        consensus.nTimeSlotLength                        = 15;

        // spork keys

        /*
         *  {
         *      "PublicKey": "04887731e4049029f4d41bf745016960256bf903ccffdd2eb341e444946ccb87122ce46d12f3941fb643b6fa3a6e384d94db60aa670ed44ff016a5fdff1254fe46",
         *      "PrivateKey": "4Y739MmKdfhXwdrCDz52CgR9aK2Ctcm8zz1qBbpCTk95tZwzJHK"
         *  }
         */

        consensus.strSporkPubKey                         = "04887731e4049029f4d41bf745016960256bf903ccffdd2eb341e444946ccb87122ce46d12f3941fb643b6fa3a6e384d94db60aa670ed44ff016a5fdff1254fe46";
        consensus.strSporkPubKeyOld                      = "";
        consensus.nTime_EnforceNewSporkKey               = 0;
        consensus.nTime_RejectOldSporkKey                = 0;

        // height-based activations
        consensus.height_last_ZC_AccumCheckpoint         = 0;
        consensus.height_last_ZC_WrappedSerials          = 0;
        consensus.height_start_InvalidUTXOsCheck         = 999999999;
        consensus.height_start_ZC_InvalidSerials         = 0;
        consensus.height_start_ZC_SerialRangeCheck       = 999999999;
        consensus.height_ZC_RecalcAccumulators           = 999999999;

        // validation by-pass
        consensus.nVsyncBadBlockTime                      = 1471401614;    // Skip nBit validation of Block 259201 per PR #915
        consensus.nVsyncBadBlockBits                      = 0x1c056dac;    // Skip nBit validation of Block 259201 per PR #915

        // Zerocoin-related params
        consensus.ZC_Modulus                             = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
                                                           "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
                                                           "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
                                                           "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
                                                           "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
                                                           "31438167899885040445364023527381951378636564391212010397122822120720357";

        consensus.ZC_MaxPublicSpendsPerTx                = 637;         // Assume about 220 bytes each input
        consensus.ZC_MaxSpendsPerTx                      = 7;           // Assume about 20kb each input
        consensus.ZC_MinMintConfirmations                = 20;
        consensus.ZC_MinMintFee                          = 1 * CENT;
        consensus.ZC_MinStakeDepth                       = 200;
        consensus.ZC_TimeStart                           = 1508214600;  // October 17, 2017 4:30:00 AM
        consensus.ZC_WrappedSerialsSupply                = 0 * COIN;    // zerocoin supply at height_last_ZC_WrappedSerials

        // Network upgrades
        consensus.vUpgrades[Consensus::BASE_NETWORK].nActivationHeight          = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_TESTDUMMY].nActivationHeight     = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;
        consensus.vUpgrades[Consensus::UPGRADE_POS].nActivationHeight           = 5001;
        consensus.vUpgrades[Consensus::UPGRADE_POS_V2].nActivationHeight        = 5010;
        consensus.vUpgrades[Consensus::UPGRADE_ZC].nActivationHeight            = 5050;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].nActivationHeight         = 5100;
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].nActivationHeight         = Consensus::NetworkUpgrade::ALWAYS_ACTIVE;
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].nActivationHeight     = 5200;
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].nActivationHeight          = 5300;
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].nActivationHeight          = 5400;
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].nActivationHeight      = Consensus::NetworkUpgrade::NO_ACTIVATION_HEIGHT;

        consensus.vUpgrades[Consensus::UPGRADE_ZC].hashActivationBlock          = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_V2].hashActivationBlock       = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_BIP65].hashActivationBlock       = uint256S("0x000002c598d8323498d1e491f1d59ce4622e67d276bdd1398ff39cd7f67b63bf");
        consensus.vUpgrades[Consensus::UPGRADE_ZC_PUBLIC].hashActivationBlock   = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V3_4].hashActivationBlock        = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V4_0].hashActivationBlock        = uint256S("0x1");
        consensus.vUpgrades[Consensus::UPGRADE_V5_DUMMY].hashActivationBlock    = uint256S("0x1");

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa0;
        pchMessageStart[1] = 0xb0;
        pchMessageStart[2] = 0xc0;
        pchMessageStart[3] = 0xe0;
        nDefaultPort       = 31817;

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }

    void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
    {
        assert(idx > Consensus::BASE_NETWORK && idx < Consensus::MAX_NETWORK_UPGRADES);
        consensus.vUpgrades[idx].nActivationHeight = nActivationHeight;
    }
};
static CRegTestParams regTestParams;

static CChainParams* pCurrentParams = 0;

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
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

void UpdateNetworkUpgradeParameters(Consensus::UpgradeIndex idx, int nActivationHeight)
{
    regTestParams.UpdateNetworkUpgradeParameters(idx, nActivationHeight);
}
