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

template <uint32_t _storage_size>
class FlashStorage {
 private:
  static constexpr uint32_t fmc_base_address = 0x08000000;
  static constexpr uint32_t fmc_end_address =
      fmc_base_address + ARDUINO_UPLOAD_MAXIMUM_SIZE;
  static constexpr uint32_t data_area_start = fmc_end_address - _storage_size;
  static constexpr uint32_t bank0_end = fmc_base_address + 512 * 1024 - 1;

  uint16_t pageSizeForAddress(uint32_t addr) {
    if (addr > bank0_end)
      return 4096;
    else
      return 2048;
  }

  uint32_t addr(uint32_t offset) {
    return data_area_start + offset;
  }

  uint32_t alignAddress(uint32_t addr) {
    const uint16_t page_size = pageSizeForAddress(addr);
    return (addr / page_size) * page_size;
  }

  uint32_t updatePage(uint8_t alignedAddress, uint32_t offset,
                      const uint8_t *data, uint32_t data_size, uint32_t *data_pos) {
    uint8_t page_data[4096];
    uint16_t page_size = pageSizeForAddress(alignedAddress);
    uint32_t pos = *data_pos;

    // Copy the page data to a temporary, updateable place
    read(alignedAddress, (uint8_t *)page_data, page_size);

    // Update the page data from our updated data set
    do {
      page_data[offset++] = data[pos++];
    } while (offset < page_size && pos < data_size);

    // Write it all back to the same address
    fmc_page_erase(alignedAddress);

    uint32_t word_count = page_size / 4;
    uint32_t i = 0;
    uint32_t *ptrs = (uint32_t *)page_data;

    do {
      fmc_word_program(alignedAddress, *ptrs++);
      alignedAddress += 4U;
    } while (++i < word_count);

    *data_pos = pos;
    return alignedAddress;
  }

 public:
  void begin() {
    fmc_unlock();
  }

  const uint32_t length() {
    return _storage_size;
  }

  void read(uint32_t offset, uint8_t *data, uint32_t data_size) {
    uint32_t address = addr(offset);

    // If we're out of bounds, bail out.
    if (address < data_area_start || address > fmc_end_address)
      return;

    uint8_t *src = (uint8_t *)(address);
    for (auto i = 0; i < data_size; i++) {
      data[i] = src[i];
    }
  }

  void writeSmall(const uint8_t *data, uint8_t size, uint32_t offset) {
    uint32_t address = addr(offset);

    // If we're out of bounds, bail out.
    if (address < data_area_start || address > fmc_end_address)
      return;

    // Do we span bank boundaries?
    if (address < bank0_end && address + 4 >= bank0_end) {
      // Instead of figuring out where to split the data and do two small
      // writes, fall back to writeBulk(), to simplify the logic. A bit of a
      // waste, as we do two big writes, but this case should be rare enough to
      // not matter.
      return writeBulk(data, size, offset);
    }

    // Within a single bank.
    uint8_t cell_data[4];
    read(offset, (uint8_t *)cell_data, sizeof(cell_data));
    for (uint8_t i = 0; i < size; i++) {
      cell_data[i] = data[i];
    }
    fmc_word_reprogram(address, *((uint32_t *)cell_data));
  }

  void writeBulk(const uint8_t *data, uint32_t size, uint32_t offset = 0) {
    uint32_t address = addr(offset);

    // If we're out of bounds, bail out.
    if (address < data_area_start || address > fmc_end_address)
      return;

    uint32_t alignedAddr = alignAddress(address);
    uint32_t page_offset = address - alignedAddr;
    uint32_t pos = 0;

    do {
      alignedAddr = updatePage(alignedAddr, page_offset, data, size, &pos);
      page_offset = 0;
    } while (pos < size);
  }
};
