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

#pragma once

#include <unordered_map>

#include "Common/JsonValue.h"
#include "JsonRpcServer/JsonRpcServer.h"
#include "SimpleWalletRPCMessages.h"
#include "Serialization/JsonInputValueSerializer.h"
#include "Serialization/JsonOutputStreamSerializer.h"
#include "Wallet/WalletGreen.h"
#include "SimpleWallet/Types.h"

namespace SimpleWalletRPC {

class SimpleWalletRPCServer : public CryptoNote::JsonRpcServer {
public:
  SimpleWalletRPCServer(System::Dispatcher& sys, System::Event& stopEvent, CryptoNote::WalletGreen& wallet, Config& cfg, Logging::ILogger& loggerGroup);
  SimpleWalletRPCServer(const SimpleWalletRPCServer&) = delete;

  inline void start() { CryptoNote::JsonRpcServer::start(cfg.rpcHost, cfg.rpcPort); }

protected:
  virtual void processJsonRpcRequest(const Common::JsonValue& req, Common::JsonValue& resp) override;

private:
  CryptoNote::WalletGreen& wallet;
  Config& cfg;
  Logging::LoggerRef logger;
  System::Event readyEvent;


  typedef std::function<void (const Common::JsonValue& jsonRpcParams, Common::JsonValue& jsonResponse)> HandlerFunction;

  template <typename RequestType, typename ResponseType, typename RequestHandler>
  HandlerFunction jsonHandler(RequestHandler handler) {
    return [handler] (const Common::JsonValue& jsonRpcParams, Common::JsonValue& jsonResponse) mutable {
      RequestType request;
      ResponseType response;

      try {
        CryptoNote::JsonInputValueSerializer inputSerializer(const_cast<Common::JsonValue&>(jsonRpcParams));
        serialize(request, inputSerializer);
      } catch (std::exception&) {
        makeGenericErrorReponse(jsonResponse, "Invalid Request", -32600);
        return;
      }

      std::error_code ec = handler(request, response);
      if (ec) {
        makeErrorResponse(ec, jsonResponse);
        return;
      }

      CryptoNote::JsonOutputStreamSerializer outputSerializer;
      serialize(response, outputSerializer);
      fillJsonResponse(outputSerializer.getValue(), jsonResponse);
    };
  }

  std::unordered_map<std::string, HandlerFunction> handlers;

  /*bool on_getbalance(const wallet_rpc::COMMAND_RPC_GET_BALANCE::request& req, wallet_rpc::COMMAND_RPC_GET_BALANCE::response& res);
  bool on_transfer(const wallet_rpc::COMMAND_RPC_TRANSFER::request& req, wallet_rpc::COMMAND_RPC_TRANSFER::response& res);
  bool on_store(const wallet_rpc::COMMAND_RPC_STORE::request& req, wallet_rpc::COMMAND_RPC_STORE::response& res);
  bool on_get_payments(const wallet_rpc::COMMAND_RPC_GET_PAYMENTS::request& req, wallet_rpc::COMMAND_RPC_GET_PAYMENTS::response& res);
  bool on_get_transfers(const wallet_rpc::COMMAND_RPC_GET_TRANSFERS::request& req, wallet_rpc::COMMAND_RPC_GET_TRANSFERS::response& res);
  bool on_get_height(const wallet_rpc::COMMAND_RPC_GET_HEIGHT::request& req, wallet_rpc::COMMAND_RPC_GET_HEIGHT::response& res);
  bool on_reset(const wallet_rpc::COMMAND_RPC_RESET::request& req, wallet_rpc::COMMAND_RPC_RESET::response& res);*/

  std::error_code handleGetBalance(const GetBalance::Request& request, GetBalance::Response& response);
  std::error_code handleTransfer(const Transfer::Request& request, Transfer::Response& response);
  std::error_code handleStore(const Store::Request& request, Store::Response& response);
  std::error_code handleGetPayments(const GetPayments::Request& request, GetPayments::Response& response);
  std::error_code handleGetTransfers(const GetTransfers::Request& request, GetTransfers::Response& response);
  std::error_code handleGetHeight(const GetHeight::Request& request, GetHeight::Response& response);
  std::error_code handleReset(const Reset::Request& request, Reset::Response& response);
  std::error_code handleOptimize(const Optimize::Request& request, Optimize::Response& response);
};

}//namespace SimpleWalletRPC
