/* -*- mode: c++ -*-
 * Copyright (c) 2021  Keyboard.io, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the names of its contributors
 *     may be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include "FlashStorage.h"

template <uint32_t _size>
class EEPROMClass {
 private:
  FlashStorage<_size> storage_;

 public:
  template<typename T>
  T& get(uint32_t offset, T& t) {
    storage_.read(offset, (uint8_t *) &t, sizeof(T));
    return t;
  }

  template<typename T>
  const T& put(uint32_t offset, const T& t) {
    storage_.writeBulk((const uint8_t *) &t, sizeof(T), offset);
  }
  const uint32_t put(uint32_t offset, const uint32_t &u) {
    storage_.writeSmall((const uint8_t *)&u, sizeof(uint32_t), offset);
  }
  const uint16_t put(uint32_t offset, const uint16_t &u) {
    storage_.writeSmall((const uint8_t *)&u, sizeof(uint16_t), offset);
  }
  const uint8_t put(uint32_t offset, const uint8_t &u) {
    storage_.writeSmall((const uint8_t *)&u, sizeof(uint8_t), offset);
  }

  uint8_t read(uint32_t offset) {
    uint8_t val;
    storage_.read(offset, &val, sizeof(val));
    return val;
  }

  void write(uint32_t offset, uint8_t val) {
    storage_.writeSmall((uint8_t *)&val, sizeof(val), offset);
  }
  void write(uint32_t offset, uint16_t val) {
    storage_.writeSmall((uint8_t *)&val, sizeof(val), offset);
  }
  void write(uint32_t offset, uint32_t val) {
    storage_.writeSmall((uint8_t *)&val, sizeof(val), offset);
  }
  void update(uint32_t offset, uint8_t val) {
    write(offset, val);
  }
  void update(uint32_t offset, uint16_t val) {
    write(offset, val);
  }
  void update(uint32_t offset, uint32_t val) {
    write(offset, val);
  }

  void begin() {
    storage_.begin();
  }

  uint32_t length() {
    return _size;
  }
};
