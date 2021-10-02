/* -*- mode: c++ -*-
 * Copyright (c) 2020  GigaDevice Semiconductor Inc.
 *               2021  Keyboard.io, Inc.
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

#include <Arduino.h>

template <uint32_t _storage_size,
          uint32_t _fmc_end = ARDUINO_UPLOAD_MAXIMUM_SIZE>
class FlashStorage {
 private:
  uint8_t buffer_[_storage_size];
  static constexpr uint32_t fmc_base_address = 0x08000000;
  static constexpr uint32_t bank0_end = fmc_base_address + 512 * 1024 - 1;
  static constexpr uint32_t fmc_end_address = fmc_base_address + _fmc_end;
  static constexpr uint32_t data_area_start = fmc_end_address - _storage_size;

  uint16_t pageSizeForAddress(uint32_t addr) {
    if (addr > bank0_end)
      return 4096;
    else
      return 2048;
  }

 public:
  void begin() {
    fmc_unlock();

    uint8_t *src = (uint8_t *)data_area_start;
    for (auto i = 0; i < _storage_size; i++) {
      buffer_[i] = src[i];
    }
  }

  uint32_t length() {
    return _storage_size;
  }

  void read(uint32_t offset, uint8_t *data, uint32_t data_size) {
    // We always read from the buffer, as that's the most up-to-date. Flash is
    // lagging behind until we commit(), but we may want to read before doing
    // so.

    // If we're out of bounds, or try to read too much, bail out.
    if (offset > _storage_size || (data_size > (_storage_size - offset)))
      return;

    for (uint32_t i = 0; i < data_size; i++) {
      data[i] = buffer_[i + offset];
    }
  }

  void write(uint32_t offset, const uint8_t *data, uint32_t data_size) {
    // If we're out of bounds, or try to write too much, bail out.
    if (offset > _storage_size || data_size > _storage_size)
      return;

    for (uint32_t i = 0; i < data_size; i++) {
      buffer_[offset + i] = data[i];
    }
  }

  void commit() {
    uint32_t address = data_area_start;
    uint32_t *ptrs = (uint32_t *)buffer_;

    do {
      uint16_t page_size = pageSizeForAddress(address);
      fmc_page_erase(address);

      uint32_t word_count = page_size / 4;
      uint32_t i = 0;

      do {
        fmc_word_program(address, *ptrs++);
        address += 4U;
      } while (++i < word_count);
    } while (address < fmc_end_address);
  }
};
