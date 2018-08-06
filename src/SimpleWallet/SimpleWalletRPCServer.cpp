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

#include "SimpleWalletRPCServer.h"

#include <functional>

#include "PaymentServiceJsonRpcMessages.h"
#include "WalletService.h"

#include "Serialization/JsonInputValueSerializer.h"
#include "Serialization/JsonOutputStreamSerializer.h"

#include "Rpc/JsonRpc.h"

namespace SimpleWalletRPC {

SimpleWalletRPC::SimpleWalletRPCServer(System::Dispatcher& sys, System::Event& stopEvent, WalletService& service, Logging::ILogger& loggerGroup, PaymentService::Configuration& config) 
  : JsonRpcServer(sys, stopEvent, loggerGroup, config)
  , service(service)
  , logger(loggerGroup, "SimpleWalletRPCServer")
{
  handlers.emplace("save", jsonHandler<Save::Request, Save::Response>(std::bind(&SimpleWalletRPCServer::handleSave, this, std::placeholders::_1, std::placeholders::_2)));
}

void SimpleWalletRPC::processJsonRpcRequest(const Common::JsonValue& req, Common::JsonValue& resp) {
  try {
    prepareJsonResponse(req, resp);

	if (!config.legacySecurity) {
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

std::error_code SimpleWalletRPC::handleGetBalance(const GetBalance::Request& request, GetBalance::Response& response) {
  return service.getBalance(response.availableBalance, response.lockedAmount);
}

}
