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
#include "Serialization/SerializationOverloads.h"

namespace SimpleWalletRPC {

void GetBalance::Request::serialize(CryptoNote::ISerializer& serializer) {
}

void GetBalance::Response::serialize(CryptoNote::ISerializer& serializer) {
  serializer(lockedAmount, "locked_amount");
  serializer(availableBalance, "available_balance");
}

} // namespace SimpleWalletRPC
