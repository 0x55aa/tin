// Copyright (c) 2016 Tin Project. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <cstdlib>

#include "base/basictypes.h"
#include "base/strings/string_piece.h"
#include "tin/io/io.h"

namespace tin {
namespace bufio {

const int kDefaultReaderBufSize = 4096;

/*
+--------------+--------------------------------+
|              |    buffered       |    free    |
+--------------+--------------------+-----------+
                |                   |
                v                   v
             read_idx           write_idx
*/

class Reader : public tin::io::Reader {
 public:
  explicit Reader(tin::io::Reader* rd, size_t size = kDefaultReaderBufSize);
  virtual ~Reader();

  // reset a new underlying reader.
  void Reset(tin::io::Reader* rd);

  virtual int Read(void* buf, int nbytes);

  // return error code.
  int ReadSlice(uint8 delim, base::StringPiece* line);

  // return error code.
  int ReadLine(base::StringPiece* line, bool* is_prefix);

  int ReadByte(uint8* c);
  int UnreadByte();
  int Peek(int n, base::StringPiece* piece);

  // inline functions
  int buffered() const { return write_idx_ - read_idx_; }
  int free() const { return (storage_size_ - write_idx_); }

  bool empty() const { return read_idx_ == write_idx_; }
  bool full() const {
    return ((write_idx_ == storage_size_) && (read_idx_ != write_idx_));
  }

  typedef uint8* iterator;
  typedef const uint8* const_iterator;

  iterator begin() { return storage_ + read_idx_; }
  const_iterator begin() const { return storage_ + read_idx_; }

  iterator end() { return storage_ + write_idx_; }
  const_iterator end() const { return storage_ + write_idx_; }

 private:
  int ReadErr();
  void Fill();

 private:
  uint8* storage_;
  int storage_size_;
  int read_idx_;
  int write_idx_;
  int err_;
  tin::io::Reader* rd_;
  int last_byte_;
};

class Writer : public tin::io::Writer {
 public:
  virtual ~Writer() {}
  virtual int Write(const void* buf, int nbytes) = 0;
};



}  // namespace bufio
}  // namespace tin




