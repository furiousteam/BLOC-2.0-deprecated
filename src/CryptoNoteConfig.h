// Copyright (c) 2018, BLOC developers
// Copyright (c) 2017-2018, Intense Coin developers
// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
//
// This file is part of BLOC.
//
// BLOC is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// BLOC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with BLOC.  If not, see <http://www.gnu.org/licenses/>.

/*
Coin name: BLOC
Coin Ticker: BLOC
Adresse prefix: Start with letter 'abloc'
Total Supply: 50 000 000
Premine: 10%
Emission speed factor: 21
Difficulty target: 120

Fees of UINT64_C(1) = 0.0001
*/


#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

namespace CryptoNote {
namespace parameters {

const uint64_t DIFFICULTY_TARGET                             = 120; // seconds

const uint32_t CRYPTONOTE_MAX_BLOCK_NUMBER                   = 500000000;
const size_t   CRYPTONOTE_MAX_BLOCK_BLOB_SIZE                = 500000000;
const size_t   CRYPTONOTE_MAX_TX_SIZE                        = 1000000000;
const uint64_t CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX       = 0xee06ac8; // addresses start with "abLoc"
const uint32_t CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW          = 50;
const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT            = 60 * 60 * 2;
const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V3         = 3 * DIFFICULTY_TARGET;

const size_t   BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW             = 60;
const size_t   BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V3          = 11;

const size_t  DIFFICULTY_BLOCKS_V2        = 17;
const size_t  DIFFICULTY_CUT_V2          = 6;
const size_t  DIFFICULTY_WINDOW_V2        = DIFFICULTY_BLOCKS_V2 + DIFFICULTY_CUT_V2 * 2;

// MONEY_SUPPLY - total number coins to be generated
const uint64_t MONEY_SUPPLY				                           = UINT64_C(500000000000);
const uint32_t ZAWY_DIFFICULTY_BLOCK_INDEX                   = 40500;
const size_t ZAWY_DIFFICULTY_V2                              = 0;
const uint8_t ZAWY_DIFFICULTY_DIFFICULTY_BLOCK_VERSION       = 4;

const uint64_t LWMA_2_DIFFICULTY_BLOCK_INDEX                 = 40500;
const uint64_t DIFFICULTY_WINDOW_V3                          = 60;
const uint64_t DIFFICULTY_BLOCKS_COUNT_V3                    = DIFFICULTY_WINDOW_V3 + 1;

const uint64_t FINAL_SUBSIDY_PER_MINUTE			                 = UINT64_C(0);
const unsigned EMISSION_SPEED_FACTOR                         = 21;
static_assert(EMISSION_SPEED_FACTOR <= 8 * sizeof(uint64_t), "Bad EMISSION_SPEED_FACTOR");

const size_t   CRYPTONOTE_REWARD_BLOCKS_WINDOW               = 100;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE     = 1000000; //size of block (bytes) after which reward for block calculated using block size
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2  = 1000000;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1  = 1000000;
const size_t   CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE;
const size_t   CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE        = 600;
const size_t   CRYPTONOTE_DISPLAY_DECIMAL_POINT              = 4;

// With DISPLAY_DECIMAL_POINT 4 i.e. 00000
// Fees of UINT64_C(1) = 0.0001
const uint64_t MINIMUM_FEE                                   = UINT64_C(1);    // pow(10, 5)
const uint64_t DEFAULT_DUST_THRESHOLD                        = UINT64_C(1);    // pow(10, 5)
const uint16_t DEFAULT_MIXIN                                 = 0;
/* minimum_mixin = enforced for deamon
   minimum_mixin_no_dust = enforced for simplewallet, when dust is not present
   if dust is present, 0 mixin allowed. Possibly later disabled, or relegated to sweep_unmixable */
const uint16_t MINIMUM_MIXIN_NO_DUST                         = 0;
const uint16_t MINIMUM_MIXIN_V1                              = 0;
const uint16_t MAXIMUM_MIXIN_V1                              = 10;

const uint64_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY             = 24 * 60 * 60 / DIFFICULTY_TARGET;
const size_t   DIFFICULTY_WINDOW                             = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; // blocks
const size_t   DIFFICULTY_CUT                                = 60;  // timestamps to cut after sorting
const size_t   DIFFICULTY_LAG                                = 15;
static_assert(2 * DIFFICULTY_CUT <= DIFFICULTY_WINDOW - 2, "Bad DIFFICULTY_WINDOW or DIFFICULTY_CUT");

const size_t   MAX_BLOCK_SIZE_INITIAL                        	= 20 * 1024;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR         	= 100 * 1024;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR_V2		= 35 * 100 * 1024;
const size_t   MAX_BLOCK_SIZE_ALLOWED_EVERY_N_BLOCK		= 10; // every 10 blocks, 1 big block is allowed
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR       	= 365 * 24 * 60 * 60 / DIFFICULTY_TARGET;

const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS    	= 1;
const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS    	= DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;

const uint64_t CRYPTONOTE_MEMPOOL_TX_LIVETIME                	= 60 * 60 * 24;     //seconds, one day
const uint64_t CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME 	= 60 * 60 * 24 * 7; //seconds, one week
const uint64_t CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL = 7;  // CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL * CRYPTONOTE_MEMPOOL_TX_LIVETIME = time to forget tx

const size_t   FUSION_TX_MAX_SIZE                            	= CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT * 30 / 100;
const size_t   FUSION_TX_MIN_INPUT_COUNT                     	= 12;
const size_t   FUSION_TX_MIN_IN_OUT_COUNT_RATIO              	= 4;

const uint32_t KEY_IMAGE_CHECKING_BLOCK_INDEX                	= 0;
const uint32_t UPGRADE_HEIGHT_V2                             	= 50;
const uint32_t UPGRADE_HEIGHT_MAX_BLOCK_SIZE			            = 65000;
const uint32_t UPGRADE_HEIGHT_V3                             	= 100;
const uint32_t UPGRADE_HEIGHT_V4                             =  40500;
const unsigned UPGRADE_VOTING_THRESHOLD                      	= 90;               // percent
const uint32_t UPGRADE_VOTING_WINDOW                         	= EXPECTED_NUMBER_OF_BLOCKS_PER_DAY;  // blocks
const uint32_t UPGRADE_WINDOW                                	= EXPECTED_NUMBER_OF_BLOCKS_PER_DAY;  // blocks
static_assert(0 < UPGRADE_VOTING_THRESHOLD && UPGRADE_VOTING_THRESHOLD <= 100, "Bad UPGRADE_VOTING_THRESHOLD");
static_assert(UPGRADE_VOTING_WINDOW > 1, "Bad UPGRADE_VOTING_WINDOW");

const char     CRYPTONOTE_BLOCKS_FILENAME[]                  = "blocks.bin";
const char     CRYPTONOTE_BLOCKINDEXES_FILENAME[]            = "blockindexes.bin";
const char     CRYPTONOTE_POOLDATA_FILENAME[]                = "poolstate.bin";
const char     P2P_NET_DATA_FILENAME[]                       = "p2pstate.bin";
const char     MINER_CONFIG_FILE_NAME[]                      = "miner_conf.json";
} // parameters

const char     CRYPTONOTE_NAME[]                             = "BLOC";

const uint8_t  TRANSACTION_VERSION_1                         =  1;
const uint8_t  TRANSACTION_VERSION_2                         =  2;
const uint8_t  CURRENT_TRANSACTION_VERSION                   =  TRANSACTION_VERSION_1;
const uint8_t  BLOCK_MAJOR_VERSION_1                         =  1;
const uint8_t  BLOCK_MAJOR_VERSION_2                         =  2;
const uint8_t  BLOCK_MAJOR_VERSION_3                         =  3;
const uint8_t  BLOCK_MAJOR_VERSION_4                         =  4;
const uint8_t  BLOCK_MINOR_VERSION_0                         =  0;
const uint8_t  BLOCK_MINOR_VERSION_1                         =  0;

const size_t   BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT        =  10000;  //by default, blocks ids count in synchronizing
const size_t   BLOCKS_SYNCHRONIZING_DEFAULT_COUNT            =  100;    //by default, blocks count in blocks downloading
const size_t   COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT         =  1000;

const int      P2P_DEFAULT_PORT                              =  2082;
const int      RPC_DEFAULT_PORT                              =  2086;

const size_t   P2P_LOCAL_WHITE_PEERLIST_LIMIT                =  1000;
const size_t   P2P_LOCAL_GRAY_PEERLIST_LIMIT                 =  5000;

const size_t   P2P_CONNECTION_MAX_WRITE_BUFFER_SIZE          = 64 * 1024 * 1024; // 32 MB
const uint32_t P2P_DEFAULT_CONNECTIONS_COUNT                 = 8;
const size_t   P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT     = 70;
const uint32_t P2P_DEFAULT_HANDSHAKE_INTERVAL                = 60;            // seconds
const uint32_t P2P_DEFAULT_PACKET_MAX_SIZE                   = 50000000;      // 50000000 bytes maximum packet size
const uint32_t P2P_DEFAULT_PEERS_IN_HANDSHAKE                = 250;
const uint32_t P2P_DEFAULT_CONNECTION_TIMEOUT                = 5000;          // 5 seconds
const uint32_t P2P_DEFAULT_PING_CONNECTION_TIMEOUT           = 2000;          // 2 seconds
const uint64_t P2P_DEFAULT_INVOKE_TIMEOUT                    = 60 * 2 * 1000; // 2 minutes
const size_t   P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT          = 5000;          // 5 seconds
const char     P2P_STAT_TRUSTED_PUB_KEY[] = "";

const char* const SEED_NODES[] = {
	"174.138.59.135:2082",//atlantic
	"138.197.208.94:2082",//pacific
	"95.216.3.162:2082",//baltic
	"88.198.57.162:2082",//europe
	"206.189.42.115:2082"//asia
		
};

struct CheckpointData {
  uint32_t index;
  const char* blockId;
};

const CheckpointData CHECKPOINTS[] = {
	{300, "a20d4c204aa5c01055f80504e8f802c59e8d3e7f8d058884bf8eee18d9b136e8"},
	{800, "2140edb727df761f6e2a0f1a07a86e6f9c9997c2a2a6ee59d94bdc785c4d76ea"},
	{1300, "6e2b2a587fb16b0fe6c7c03de69f9060137ad218a941204ad19b832fce8c008f"},
	{2000, "4d7241ab61013ca26d1e29b34340e743ca6ca2670346704da12f4edb3343aa25"},
	{3000, "c6e5e6730714bb03b9ade5cc1ed9c4c221611b395c7140a7b1e2886165f464c2"},
	{5000, "9474c4e9cceaa0590db12ab3aec16aa75608a9626fded74c5f77bee6e29eb20e"},
	{8000, "5daf3f64c60eba8fd907d2e9c9bb33d88a3fd986eb31bdec9c474a1c8839a687"},
	{30000, "6fd1c2960dc5c6c83f4864d690d124614f9d538ad7ac798276a30035e2f144da"},
	{40000, "f3470ae01eedc1456e7c41c57367cfd2e9fa29439e05aee1571532e18f75bfe4"},
	{40600, "0fd41c01f0a76aa45a3ccb62c2fc5ab325fa57d4c25ddfbe168cd11e7424aa78"},
	{45000, "014d3a57cfe8c55b198abc96464598261ce9a658a471c0bf2c864543056eed2d"},
	{50000, "49ed4e36995ff484963e32fe360cea1ba504d802bbe00f9fe2ff5266f70a9c2b"},
	{60000, "7f398b91508a52f821b23a8c614b39b29c0c499469843c7c43b793dcc10615ca"},
	{74243, "a27e3ab59adfc42eeb8ee22e16736b092896fb4a25144ce439d35b5d5cbfd70a"}};

}

#define ALLOW_DEBUG_COMMANDS
