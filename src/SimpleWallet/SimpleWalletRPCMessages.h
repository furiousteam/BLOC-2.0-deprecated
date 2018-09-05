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

#include <exception>
#include <limits>
#include <vector>
#include <list>

#include "Serialization/ISerializer.h"

namespace SimpleWalletRPC {

class RequestSerializationError: public std::exception {
public:
  virtual const char* what() const throw() override { return "Request error"; }
};

struct GetBalance {
  struct Request {
    void serialize(CryptoNote::ISerializer& serializer);
  };

  struct Response {
    uint64_t lockedAmount;
    uint64_t availableBalance;

    void serialize(CryptoNote::ISerializer& serializer);
  };
};

struct transfer_destination
{
    uint64_t amount;
    std::string address;

    void serialize(CryptoNote::ISerializer& s) {
      KV_MEMBER(amount)
      KV_MEMBER(address)
    }
};

struct Transfer
{
    struct Request
    {
        std::list<transfer_destination> destinations;
        uint64_t fee;
        uint64_t mixin;
        uint64_t unlock_time;
        std::string payment_id;

        void serialize(CryptoNote::ISerializer& s) {
          KV_MEMBER(destinations)
          KV_MEMBER(fee)
          KV_MEMBER(mixin)
          KV_MEMBER(unlock_time)
          KV_MEMBER(payment_id)
        }
    };

    struct Response
    {
      std::string tx_hash;

      void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(tx_hash)
      }
    };
};

struct Store
{
    struct Request {
      void serialize(CryptoNote::ISerializer& serializer) {}
    };

    struct Response {
      void serialize(CryptoNote::ISerializer& serializer) {}
    };
};

struct payment_details
{
    std::string tx_hash;
    uint64_t amount;
    uint64_t block_height;
    uint64_t unlock_time;

    void serialize(CryptoNote::ISerializer& s) {
      KV_MEMBER(tx_hash)
      KV_MEMBER(amount)
      KV_MEMBER(block_height)
      KV_MEMBER(unlock_time)
    }
};

struct GetPayments
{
    struct Request
    {
      std::string payment_id;

      void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(payment_id)
      }
    };

    struct Response
    {
      std::list<payment_details> payments;

      void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(payments)
      }
    };
};

struct transfer_details {
    uint64_t time;
    bool output;
    std::string transactionHash;
    uint64_t amount;
    uint64_t fee;
    std::string paymentId;
    std::string address;
    uint64_t blockIndex;
    uint64_t unlockTime;

    void serialize(CryptoNote::ISerializer& s) {
      KV_MEMBER(time)
      KV_MEMBER(output)
      KV_MEMBER(transactionHash)
      KV_MEMBER(amount)
      KV_MEMBER(fee)
      KV_MEMBER(paymentId)
      KV_MEMBER(address)
      KV_MEMBER(blockIndex)
      KV_MEMBER(unlockTime)
    }
};

struct GetTransfers
{
    struct Request {
      void serialize(CryptoNote::ISerializer& serializer) {}
    };

    struct Response {
      std::list<transfer_details> transfers;

      void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(transfers)
      }
    };
};

struct GetHeight
{
    struct Request {
      void serialize(CryptoNote::ISerializer& serializer) {}
    };

    struct Response {
      uint64_t height;

      void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(height)
      }
    };
};

struct Reset 
{
    struct Request {
      void serialize(CryptoNote::ISerializer& serializer) {}
    };

    struct Response {
      void serialize(CryptoNote::ISerializer& serializer) {}
    };
};

struct Optimize 
{
  struct Request {
    std::string address;

    void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(address)
    }
  };

  struct Response {
    std::string transactionHash;

    void serialize(CryptoNote::ISerializer& s) {
        KV_MEMBER(transactionHash)
    }
  };
};
} //namespace SimpleWalletRPC
