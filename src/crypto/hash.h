// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Bytecoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Bytecoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <stddef.h>

#include <CryptoTypes.h>
#include "generic-ops.h"
#include "cn_slow_hash.hpp"

namespace Crypto {

  extern "C" {
#include "hash-ops.h"
  }

  /*
    Cryptonight hash functions
  */

  inline void cn_fast_hash(const void *data, size_t length, Hash &hash) {
    cn_fast_hash(data, length, reinterpret_cast<char *>(&hash));
  }

  inline Hash cn_fast_hash(const void *data, size_t length) {
    Hash h;
    cn_fast_hash(data, length, reinterpret_cast<char *>(&h));
    return h;
  }

  inline void tree_hash(const Hash *hashes, size_t count, Hash &root_hash) { 
    tree_hash(reinterpret_cast<const char (*)[HASH_SIZE]>(hashes), count, reinterpret_cast<char *>(&root_hash)); 
  } 
 
  inline void tree_branch(const Hash *hashes, size_t count, Hash *branch) { 
    tree_branch(reinterpret_cast<const char (*)[HASH_SIZE]>(hashes), count, reinterpret_cast<char (*)[HASH_SIZE]>(branch)); 
  } 
 
  inline void tree_hash_from_branch(const Hash *branch, size_t depth, const Hash &leaf, const void *path, Hash &root_hash) { 
    tree_hash_from_branch(reinterpret_cast<const char (*)[HASH_SIZE]>(branch), depth, reinterpret_cast<const char *>(&leaf), path, reinterpret_cast<char *>(&root_hash)); 
  } 
 
}

CRYPTO_MAKE_HASHABLE(Hash)
