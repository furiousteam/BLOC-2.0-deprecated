// Copyright (c) 2018, BLOC Developers
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

#include "SimpleWalletRPCMessages.h"
#include "SimpleWalletRPCServer.h"

#include <functional>


#include <System/EventLock.h>
#include "CryptoNoteConfig.h"
#include "Serialization/JsonInputValueSerializer.h"
#include "Serialization/JsonOutputStreamSerializer.h"
#include "IWalletLegacy.h"
#include "Rpc/JsonRpc.h"
#include "CryptoNoteCore/TransactionExtra.h"
#include "Common/StringTools.h"
#include "Wallet/WalletRpcServerErrorCodes.h"
#include "Wallet/WalletErrors.h"

namespace SimpleWalletRPC {

SimpleWalletRPCServer::SimpleWalletRPCServer(System::Dispatcher& sys, System::Event& stopEvent,  CryptoNote::WalletGreen& wallet, CryptoNote::INode &node, Config& cfg, Logging::ILogger& loggerGroup) 
  : JsonRpcServer(sys, stopEvent, loggerGroup)
  , wallet(wallet)
  , node(node)
  , cfg(cfg)
  , logger(loggerGroup, "SimpleWalletRPCServer")
  , readyEvent(sys)
{
  handlers.emplace("getbalance", jsonHandler<GetBalance::Request, GetBalance::Response>(std::bind(&SimpleWalletRPCServer::handleGetBalance, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("transfer", jsonHandler<Transfer::Request, Transfer::Response>(std::bind(&SimpleWalletRPCServer::handleTransfer, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("store", jsonHandler<Store::Request, Store::Response>(std::bind(&SimpleWalletRPCServer::handleStore, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("get_payments", jsonHandler<GetPayments::Request, GetPayments::Response>(std::bind(&SimpleWalletRPCServer::handleGetPayments, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("get_transfers", jsonHandler<GetTransfers::Request, GetTransfers::Response>(std::bind(&SimpleWalletRPCServer::handleGetTransfers, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("get_height", jsonHandler<GetHeight::Request, GetHeight::Response>(std::bind(&SimpleWalletRPCServer::handleGetHeight, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("reset", jsonHandler<Reset::Request, Reset::Response>(std::bind(&SimpleWalletRPCServer::handleReset, this, std::placeholders::_1, std::placeholders::_2)));
  handlers.emplace("optimize", jsonHandler<Optimize::Request, Optimize::Response>(std::bind(&SimpleWalletRPCServer::handleOptimize, this, std::placeholders::_1, std::placeholders::_2)));
  readyEvent.set();
}

void SimpleWalletRPCServer::processJsonRpcRequest(const Common::JsonValue& req, Common::JsonValue& resp) {
  try {
    prepareJsonResponse(req, resp);

  if (!cfg.legacySecurity) {
    std::string clientPassword;
    if (!req.contains("password")) {
      makeInvalidPasswordResponse(resp);
      return;
    }   
    if (!req("password").isString()) {
      makeInvalidPasswordResponse(resp);
      return;
    }
    clientPassword = req("password").getString();
    if (clientPassword != cfg.rpcPassword) {
      makeInvalidPasswordResponse(resp);
      return;
    }
  }

    if (!req.contains("method")) {
      logger(Logging::WARNING) << "Field \"method\" is not found in json request: " << req;
      makeGenericErrorReponse(resp, "Invalid Request", -3600);
      return;
    }

    if (!req("method").isString()) {
      logger(Logging::WARNING) << "Field \"method\" is not a string type: " << req;
      makeGenericErrorReponse(resp, "Invalid Request", -3600);
      return;
    }

    std::string method = req("method").getString();

    auto it = handlers.find(method);
    if (it == handlers.end()) {
      logger(Logging::WARNING) << "Requested method not found: " << method;
      makeMethodNotFoundResponse(resp);
      return;
    }

    logger(Logging::DEBUGGING) << method << " request came";

    Common::JsonValue params(Common::JsonValue::OBJECT);
    if (req.contains("params")) {
      params = req("params");
    }

    it->second(params, resp);
  } catch (std::exception& e) {
    logger(Logging::WARNING) << "Error occurred while processing JsonRpc request: " << e.what();
    makeGenericErrorReponse(resp, e.what());
  }
}

std::error_code SimpleWalletRPCServer::handleGetBalance(const GetBalance::Request& request, GetBalance::Response& response) {
  try {
    System::EventLock lk(readyEvent);
    logger(Logging::DEBUGGING) << "Getting wallet balance";

    response.availableBalance = wallet.getActualBalance();
    response.lockedAmount = wallet.getPendingBalance();
  } catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while getting balance: " << x.what();
    return x.code();
  }

  logger(Logging::DEBUGGING) << "Wallet actual balance: " << response.availableBalance << ", pending: " << response.lockedAmount;
  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleTransfer(const Transfer::Request& request, Transfer::Response& response) {
  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleStore(const Store::Request& request, Store::Response& response) {
  try {
	System::EventLock lk(readyEvent);
	wallet.save();
  }
  catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while doing reset: " << x.what();
    return x.code();
  }

  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleGetPayments(const GetPayments::Request& request, GetPayments::Response& response) {
  using namespace CryptoNote;

  Crypto::Hash expectedPaymentId;
  BinaryArray payment_id_blob;

  if (!Common::fromHex(request.payment_id, payment_id_blob)) {
    throw JsonRpc::JsonRpcError(WALLET_RPC_ERROR_CODE_WRONG_PAYMENT_ID, "Payment ID has invald format");
  }

  if (sizeof(expectedPaymentId) != payment_id_blob.size()) {
    throw JsonRpc::JsonRpcError(WALLET_RPC_ERROR_CODE_WRONG_PAYMENT_ID, "Payment ID has invalid size");
  }

  try {
	System::EventLock lk(readyEvent);
	expectedPaymentId = *reinterpret_cast<const Crypto::Hash*>(payment_id_blob.data());
	size_t transactionsCount = wallet.getTransactionCount();
	for (size_t trantransactionNumber = 0; trantransactionNumber < transactionsCount; ++trantransactionNumber) {
		WalletTransaction txInfo = wallet.getTransaction(trantransactionNumber);
		if (txInfo.state != WalletTransactionState::SUCCEEDED || txInfo.blockHeight == WALLET_UNCONFIRMED_TRANSACTION_HEIGHT) {
			continue;
		}

		if (txInfo.totalAmount < 0) continue;

		std::vector<uint8_t> extraVec;
		extraVec.reserve(txInfo.extra.size());
		std::for_each(txInfo.extra.begin(), txInfo.extra.end(), [&extraVec](const char el) { extraVec.push_back(el); });

		Crypto::Hash paymentId;
		if (getPaymentIdFromTxExtra(extraVec, paymentId) && paymentId == expectedPaymentId) {
			payment_details rpc_payment;
			rpc_payment.tx_hash = Common::podToHex(txInfo.hash);
			rpc_payment.amount = txInfo.totalAmount;
			rpc_payment.block_height = txInfo.blockHeight;
			rpc_payment.unlock_time = txInfo.unlockTime;
			response.payments.push_back(rpc_payment);
		}
	}
  }
  catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while doing get_payments: " << x.what();
    return x.code();
  }

  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleGetTransfers(const GetTransfers::Request& request, GetTransfers::Response& response) {
  using namespace CryptoNote;

  response.transfers.clear();
  try {
	System::EventLock lk(readyEvent);
	logger(Logging::DEBUGGING) << "get_transfers call";
	size_t transactionsCount = wallet.getTransactionCount();
	for (size_t trantransactionNumber = 0; trantransactionNumber < transactionsCount; ++trantransactionNumber) {
		WalletTransaction txInfo = wallet.getTransaction(trantransactionNumber);
		if (txInfo.state != WalletTransactionState::SUCCEEDED || txInfo.blockHeight == WALLET_UNCONFIRMED_TRANSACTION_HEIGHT) {
			continue;
		}

		WalletTransfer tr = wallet.getTransactionTransfer(trantransactionNumber, 0);

		transfer_details transfer;
		transfer.time = txInfo.timestamp;
		transfer.output = txInfo.totalAmount < 0;
		transfer.transactionHash = Common::podToHex(txInfo.hash);
		transfer.amount = std::abs(txInfo.totalAmount);
		transfer.fee = txInfo.fee;
		transfer.address = tr.address;
		transfer.blockIndex = txInfo.blockHeight;
		transfer.unlockTime = txInfo.unlockTime;
		transfer.paymentId = "";

		std::vector<uint8_t> extraVec;
		extraVec.reserve(txInfo.extra.size());
		std::for_each(txInfo.extra.begin(), txInfo.extra.end(), [&extraVec](const char el) { extraVec.push_back(el); });

		Crypto::Hash paymentId;
		transfer.paymentId = (getPaymentIdFromTxExtra(extraVec, paymentId) && paymentId != NULL_HASH ? Common::podToHex(paymentId) : "");

		response.transfers.push_back(transfer);
	}
  }
  catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while doing get_transfers: " << x.what();
    return x.code();
  }
  
  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleGetHeight(const GetHeight::Request& request, GetHeight::Response& response) {
  try {
	System::EventLock lk(readyEvent);
	response.height = node.getLastLocalBlockHeight();
  }
  catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while doing get_height: " << x.what();
    return x.code();
  }

  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleReset(const Reset::Request& request, Reset::Response& response) {
  try {
	System::EventLock lk(readyEvent);
	wallet.resetPendingTransactions();
  }
  catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while doing reset: " << x.what();
    return x.code();
  }

  return std::error_code();
}

std::error_code SimpleWalletRPCServer::handleOptimize(const Optimize::Request& request, Optimize::Response& response) {
  using namespace CryptoNote;
  
  try {
    System::EventLock lk(readyEvent);

    uint64_t threshold = wallet.getActualBalance();
    uint64_t bestThreshold = wallet.getActualBalance();
    size_t optimizable = 0;

    /* Find the best threshold by starting at threshold and decreasing by
       half till we get to the minimum amount, storing the threshold that
       gave us the most amount of optimizable amounts */
    while (threshold > CryptoNote::parameters::MINIMUM_FEE)
    {
        CryptoNote::IFusionManager::EstimateResult r = wallet.estimate(threshold);

        if (r.fusionReadyCount > optimizable)
        {
            optimizable = r.fusionReadyCount;
            bestThreshold = threshold;
        }

        threshold /= 2;
    }

    /* Can't optimize */
    if (optimizable == 0)
    {
        return make_error_code(CryptoNote::error::OBJECT_NOT_FOUND);
    }

    size_t transactionId = wallet.createFusionTransaction(bestThreshold, CryptoNote::parameters::DEFAULT_MIXIN, { }, request.address);
    response.transactionHash = Common::podToHex(wallet.getTransaction(transactionId).hash);

    logger(Logging::INFO) << "Fusion transaction " << response.transactionHash << " has been sent";
  } catch (std::system_error& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while optimizing wallet: " << x.what();
    return x.code();
  } catch (std::exception& x) {
    logger(Logging::WARNING, Logging::BRIGHT_YELLOW) << "Error while optimizing wallet: " << x.what();
    return make_error_code(CryptoNote::error::INTERNAL_WALLET_ERROR);
  }
  return std::error_code();
}
}
