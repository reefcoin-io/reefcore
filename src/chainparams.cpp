// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Copyright (c) 2014-2018 The Reef Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"
#include "arith_uint256.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "chainparamsseeds.h"

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
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Reef Coin MainNet Test launches on May 6, 2018";
    const CScript genesisOutputScript = CScript() << ParseHex("040a3ada5ba6280b99f49a92ba47221e6a72af844ec49d0c8bbdae1ec09a4c79b22e42eefe670ae04490556f91780eb57de76493d020c91d0c421c2fa052b28a2b") << OP_CHECKSIG;
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
class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 60000; // every130k blocks
        consensus.nMasternodePaymentsStartBlock = 2; // block after premine
        consensus.nMasternodePaymentsIncreaseBlock = 158000000; // not used
        consensus.nMasternodePaymentsIncreasePeriod = 576*30; // not used
        consensus.nInstantSendKeepLock = 24;
        consensus.nBudgetPaymentsStartBlock = 2100000000; // year 10000+
        consensus.nBudgetPaymentsCycleBlocks = 16616;
        consensus.nBudgetPaymentsWindowBlocks = 100;
        consensus.nBudgetProposalEstablishingTime = 60*60*24;
        consensus.nSuperblockStartBlock = 2100000000; // year 10000+
        consensus.nSuperblockCycle = 16616;
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = 227931; // FIX
        consensus.BIP34Hash = uint256S("0x000000000000024b89b42a942fe0d9fea3bb44ab7bd1b19115dd6a759c0808b8"); // FIX
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        //consensus.nPowTargetTimespan = 2 * 60; // Reef: every blocks
        //consensus.nPowTargetSpacing = 2 * 60; // Reef: 2 minutes
        consensus.nPowTargetTimespan = 1 * 60; // Reef: every blocks
        consensus.nPowTargetSpacing = 1 * 60; // Reef: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1916; // 95% of 2016
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1523675804; // Aug 17th, 2019
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1555459200; // April 17, 2019

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000000000010d96c32fd677"); //1938
        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000009cc1f28c974798e6222442be48a61a8f23a1497d4cdada1c38a76c4"); //1938

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0x6a;
        pchMessageStart[1] = 0xb1;
        pchMessageStart[2] = 0x9c;
        pchMessageStart[3] = 0xd5;
        vAlertPubKey = ParseHex("044513449073a8efe161dc42e7c07c61c4a8f59297dc8ebacbc2f77345084d058399022bc6a0db0719739f183d14b04893fb78c3b9bd9a3f88ecf8ea06adae99fe");
        nDefaultPort = 13058;
        nMaxTipAge = 1.5 * 60 * 60; // ~36 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nPruneAfterHeight = 100000;

        //genesis = CreateGenesisBlock(1523764584, 360505, 0x1e0ffff0, 1, 50 * COIN);

        uint32_t nTime = 1526191846;
        uint32_t nNonce = 4585107;
        uint32_t nBits = 504365040;
        int32_t nVersion = 1;
        //const CAmount& genesisReward = 50 * COIN;
        const CAmount& genesisReward = 5000 * COIN;
        genesis = CreateGenesisBlock(nTime, nNonce, nBits, nVersion, genesisReward);
        consensus.hashGenesisBlock = genesis.GetHash();
/*
        if(genesis.GetHash() != uint256S("0x"))
            {
                    bool fNegative;
                    bool fOverflow;
                    arith_uint256 bigNum;
                    printf("Searching for genesis block...\n");
                    bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
                    while(UintToArith256(genesis.GetHash()) > bigNum)
                    {
                            ++genesis.nNonce;
                            if (genesis.nNonce == 0)
                            {
                                    printf("NONCE WRAPPED, incrementing time");
                                    std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                                    ++genesis.nTime;
                            }
                            if (genesis.nNonce % 10000 == 0)
                            {
                                  //  printf("Mainnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                            }
                    }
                    printf("Mainnet block.nBits = %u \n", genesis.nBits);
                    printf("Mainnet block.nTime = %u \n", genesis.nTime);
                    printf("Mainnet block.nNonce = %u \n", genesis.nNonce);
                    printf("Mainnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                    printf("Mainnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
                    printf("Mainnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
            }
        **/
            //printf("block.hashGenesisBlock = %s\n",consensus.hashGenesisBlock.ToString().c_str());
            //printf("block.hashMerkleRoot = %s\n",genesis.hashMerkleRoot.ToString().c_str());
	    assert(consensus.hashGenesisBlock == uint256S("0x0000010e08cdaff5220e3db48752673b1b05b08129a2c8121e88350d6d5ef0ef"));
        assert(genesis.hashMerkleRoot == uint256S("0x5441361edd31ed9073d5efc41f2ec32154fbee009d19c7ba32e6f25ff1165a52"));


        vSeeds.push_back(CDNSSeedData("seed1", "seed1.reefcoin.io"));
        vSeeds.push_back(CDNSSeedData("seed2", "seed2.reefcoin.io"));
        vSeeds.push_back(CDNSSeedData("seed3", "159.89.90.181"));
        vSeeds.push_back(CDNSSeedData("seed4", "138.68.91.38"));
	
	//vSeeds.clear();
	//vFixedSeeds.clear();
        // Reef addresses start with 'R'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,60);
        // Reef script addresses start with '7'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,16);
        // Reef private keys start with 'E'
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,33);
        // Reef BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        // Reef BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // Reef BIP44 coin type is '5'
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x05).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour
        strSporkPubKey = "04d9491a6cf40a2afaf51de3939eadca259a95843b637f82c772a5719bc64051409031803a1c33f1f9b14c24a2d6937fe5b76ffa99a9730aa27726f9934cabf7f4";
        strMasternodePaymentsPubKey = "041fda8a1eff0a55d4d5c2d10f426e9c204d8faa228e3bbbaccd716a0db59bbfbe15dc17975f41e554ad551316b97586ddf5bec909a9fc3fc36c17a9611294fcf8";

        /*checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            (    0, uint256S("0x00000e1728b630fd83aecbc51546c7915fffb7d3c897b5fd8c4b14043070b7f0"))
            ( 1000, uint256S("0x00000040520462647ef6997ab360d95af29e5288d8bc393643357f8ff433c366"))
            ( 1900, uint256S("0x000000000071b6d0803cd72115d51445bc1703ed5b4cfcd18e6c7bc3fa72c167"))
            ( 1938, uint256S("0x0000000009cc1f28c974798e6222442be48a61a8f23a1497d4cdada1c38a76c4"))
            ( 2261, uint256S("0x00000000016d36f27c5a082407e5fbbf27760f769202165fc9ca2303db8f8da3"))
            ( 2500, uint256S("0x000000001d9c136b34055b16c604e81cc516cedcf0d6e02358178c1096f71271"))
            ( 8800, uint256S("0x0000000002241f4c3a10353ec625b642490dfff01734fcebbdeb733778e58635")),
            1521392251, // * UNIX timestamp of last checkpoint block
            10020,      // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            2800        // * estimated number of transactions per day after checkpoint
        };*/
       checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x0000010e08cdaff5220e3db48752673b1b05b08129a2c8121e88350d6d5ef0ef")),
            1526191846, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500	        // * estimated number of transactions per day after checkpoint
        };
            /**  */
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 130000;
        consensus.nMasternodePaymentsStartBlock = 2; // not true, but it's ok as long as it's less then nMasternodePaymentsIncreaseBlock
        consensus.nMasternodePaymentsIncreaseBlock = 46000;
        consensus.nMasternodePaymentsIncreasePeriod = 576;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 2100000000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 2100000000; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP34Height = 21111; // FIX
        consensus.BIP34Hash = uint256S("0x0000000023b3a96d3484e5abb3755c413e7d41500f8e2a5c3f0dd01299cd8ef8"); // FIX
        consensus.powLimit = uint256S("00000fffff000000000000000000000000000000000000000000000000000000");
        consensus.nPowTargetTimespan = 1 * 60; // Reef: 1 hour
        //consensus.nPowTargetSpacing = 2 * 60; // Reef: 2 minutes
        consensus.nPowTargetSpacing = 1 * 60; // Reef: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1523923200; // Apr 17, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1555459200; // Apr 17, 2018

        pchMessageStart[0] = 0x70;
        pchMessageStart[1] = 0xa7;
        pchMessageStart[2] = 0x7d;
        pchMessageStart[3] = 0xd6;
        vAlertPubKey = ParseHex("04f9e05c65b8cf20e31464d7f35504b62999f845c9242bc6b1bcd1993c643e3ca40527a13de58afa831dccdeacae82b39c01602daf3a7f4151032f5dacefa36932");
        nDefaultPort = 13717;
        nMaxTipAge = 0x7fffffff; // allow mining on top of old blocks for testnet
        nPruneAfterHeight = 1000;
        uint32_t nTime = 1526191746;
        uint32_t nNonce = 6892045;
        uint32_t nBits = 504365040;
        int32_t nVersion = 1;
        //const CAmount& genesisReward = 50 * COIN;
        const CAmount& genesisReward = 5000 * COIN;
        genesis = CreateGenesisBlock(nTime, nNonce, nBits, nVersion, genesisReward);
        //genesis = CreateGenesisBlock(1513728000, 21635, 0x1e0ffff0, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        /** if(genesis.GetHash() != uint256S("0x"))
             {
                     bool fNegative;
                     bool fOverflow;
                     arith_uint256 bigNum;
                     printf("Searching for genesis block...\n");
                     bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
                     while(UintToArith256(genesis.GetHash()) > bigNum)
                     {
                             ++genesis.nNonce;
                             if (genesis.nNonce == 0)
                             {
                                     printf("NONCE WRAPPED, incrementing time");
                                     std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                                     ++genesis.nTime;
                             }
                             if (genesis.nNonce % 10000 == 0)
                             {
                                     printf("Testnet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                             }
                     }
                     printf("Testnet block.nBits = %u \n", genesis.nBits);
                     printf("Testnet block.nTime = %u \n", genesis.nTime);
                     printf("Testnet block.nNonce = %u \n", genesis.nNonce);
                     printf("Testnet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                     printf("Testnet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
                     printf("Testnet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
             }
         */

       assert(consensus.hashGenesisBlock == uint256S("0x000000bcc46441623bf58ecd450460e8ad9465c018c0bcbb6f4d1ceef00f8d5c"));
       assert(genesis.hashMerkleRoot == uint256S("0x5441361edd31ed9073d5efc41f2ec32154fbee009d19c7ba32e6f25ff1165a52"));


        vFixedSeeds.clear();
        vSeeds.clear();

        // Testnet Reef addresses start with 'R'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,61);
        // Testnet Reef script addresses start with '5'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        // Testnet private keys start with '5' or 'n' (Bitcoin defaults) (?)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,33);
        // Testnet Reef BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Testnet Reef BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Testnet Reef BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes
        strSporkPubKey = "04bd429fcefdd9775510ebb36e824e8184659c72fdc09e4083f129b5de2971032941a43e1114c0da68ece23d5e60dc830fa40b525fb64c2447ed411b4830531a92";
        strMasternodePaymentsPubKey = "04640195cd018340f4782744b2937099ad224624471076f40a9b1b1b14b908b3690577f4ecb585ce004e378184715d1a3d6bdba0a65a9802535a2c014cc49f7ea6";

        checkpointData = (CCheckpointData) {
            boost::assign::map_list_of
            ( 0, uint256S("0x000000bcc46441623bf58ecd450460e8ad9465c018c0bcbb6f4d1ceef00f8d5c")),
            1526191746, // * UNIX timestamp of last checkpoint block
            0,          // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            500	        // * estimated number of transactions per day after checkpoint
        };
        /** */

    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nInstantSendKeepLock = 6;
        consensus.nBudgetPaymentsStartBlock = 1000;
        consensus.nBudgetPaymentsCycleBlocks = 50;
        consensus.nBudgetPaymentsWindowBlocks = 10;
        consensus.nBudgetProposalEstablishingTime = 60*20;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 60 * 60; // Reef: 1 hour
        //consensus.nPowTargetSpacing = 2 * 60; // Reef: 2 minutes
        consensus.nPowTargetSpacing = 1 * 60; // Reef: 2 minutes
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;

        pchMessageStart[0] = 0x76;
        pchMessageStart[1] = 0x9d;
        pchMessageStart[2] = 0x5e;
        pchMessageStart[3] = 0xd7;
        nMaxTipAge = 6 * 60 * 60; // ~144 blocks behind -> 2 x fork detection time, was 24 * 60 * 60 in bitcoin
        nDefaultPort = 13617;
        nPruneAfterHeight = 1000;
        uint32_t nTime = 1526191646;
        uint32_t nNonce = 824823;
        uint32_t nBits = 504655040;
        int32_t nVersion = 1;
        //const CAmount& genesisReward = 50 * COIN;
        const CAmount& genesisReward = 5000 * COIN;
        genesis = CreateGenesisBlock(nTime, nNonce, nBits, nVersion, genesisReward);
        //genesis = CreateGenesisBlock(1513814400, 3, 0x207fffff, 1, 50 * COIN);

        consensus.hashGenesisBlock = genesis.GetHash();
        /**if(genesis.GetHash() != uint256S("0x"))
             {
                     bool fNegative;
                     bool fOverflow;
                     arith_uint256 bigNum;
                     printf("Searching for genesis block...\n");
                     bigNum.SetCompact(genesis.nBits, &fNegative, &fOverflow);
                     while(UintToArith256(genesis.GetHash()) > bigNum)
                     {
                             ++genesis.nNonce;
                             if (genesis.nNonce == 0)
                             {
                                     printf("NONCE WRAPPED, incrementing time");
                                     std::cout << std::string("NONCE WRAPPED, incrementing time:\n");
                                     ++genesis.nTime;
                             }
                             if (genesis.nNonce % 10000 == 0)
                             {
                                 //    printf("RegTestNet: nonce %08u: hash = %s \n", genesis.nNonce, genesis.GetHash().ToString().c_str());
                             }
                     }
                     printf("RegTestNet block.nBits = %u \n", genesis.nBits);
                     printf("RegTestNet block.nTime = %u \n", genesis.nTime);
                     printf("RegTestNet block.nNonce = %u \n", genesis.nNonce);
                     printf("RegTestNet block.GetHash = %s\n", genesis.GetHash().ToString().c_str());
                     printf("RegTestNet block.GetHash = %s\n", genesis.GetHash().GetHex().c_str());
                     printf("RegTestNet block.Merkleroot = %s\n", genesis.hashMerkleRoot.ToString().c_str());
             }

         */
        assert(consensus.hashGenesisBlock == uint256S("0x00000f06f0373d435738e2be9b7e042e004718ad9fce41b4f3ec8bf99284925f"));
        assert(genesis.hashMerkleRoot == uint256S("0x5441361edd31ed9073d5efc41f2ec32154fbee009d19c7ba32e6f25ff1165a52"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;

        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = (CCheckpointData){
            boost::assign::map_list_of
            ( 0, uint256S("0x00000f06f0373d435738e2be9b7e042e004718ad9fce41b4f3ec8bf99284925f")),
            0,
            0,
            0
        };
        // Regtest Reef addresses start with 'n'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,112);
        // Regtest Reef script addresses start with '5'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,10);
        // Regtest private keys start with '5' or 'c' (Bitcoin defaults) (?)
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,240);
        // Regtest Reef BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        // Regtest Reef BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        // Regtest Reef BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE]  = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();
   }
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
            return mainParams;
    else if (chain == CBaseChainParams::TESTNET)
            return testNetParams;
    else if (chain == CBaseChainParams::REGTEST)
            return regTestParams;
    else
        throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}
