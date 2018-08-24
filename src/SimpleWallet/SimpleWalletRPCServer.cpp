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
#include "Serialization/JsonInputValueSerializer.h"
#include "Serialization/JsonOutputStreamSerializer.h"

#include "Rpc/JsonRpc.h"

namespace SimpleWalletRPC {

SimpleWalletRPCServer::SimpleWalletRPCServer(System::Dispatcher& sys, System::Event& stopEvent,  CryptoNote::WalletGreen& wallet, Logging::ILogger& loggerGroup) 
  : JsonRpcServer(sys, stopEvent, loggerGroup)
  , wallet(wallet)
  , logger(loggerGroup, "SimpleWalletRPCServer")
  , readyEvent(sys)
{
  handlers.emplace("getbalance", jsonHandler<GetBalance::Request, GetBalance::Response>(std::bind(&SimpleWalletRPCServer::handleGetBalance, this, std::placeholders::_1, std::placeholders::_2)));
}

void SimpleWalletRPCServer::processJsonRpcRequest(const Common::JsonValue& req, Common::JsonValue& resp) {
  try {
    prepareJsonResponse(req, resp);

	/*if (!config.legacySecurity) {
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
	       if (clientPassword != config.rpcPassword) {
	         makeInvalidPasswordResponse(resp);
	         return;
	       }
	     }*/

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
    //System::EventLock lk(readyEvent); \todo re-add this lock
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

}
