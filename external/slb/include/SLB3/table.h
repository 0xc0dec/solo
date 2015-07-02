// Copyright (c) 2013, Jose L. Hidalgo Valiño <pplux@pplux.com>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#ifndef __SLB3_TABLE__
#define __SLB3_TABLE__ 1

#include "value.h"

namespace SLB3 {

  class Subtable;

  class Table {
  public:
    enum Action {
      kNew_table,
      kTable_at_top,
    };

    // Creates a table (not in the stack)
    Table(lua_State *L, const Action action = kNew_table );
    ~Table();

    // pushes the table to the stack so it can be used by regular API.
    // It will be pushed to the same stack where the table was created
    // the argument is only used to check you push the table to the same state
    // where the table was created.
    void push(lua_State *L) const;

    // Generic method to set values of the table
    template<class T>
    Table& set(const char *name, T value);

    // Creates a subtable
    Table subtable(const char *name);

    // closes the subtable, and returns the parent table
    Table& close();

  protected:
    Table();
    explicit Table(Table *parent);
    void init(lua_State *L, const Action action);
    lua_State* L_;

  private:
    int registry_ref_;
    Table* parent_;

    void rawSet(const char *name);

    Table(const Table&);
    Table& operator=(const Table&);
  };

  template<class T>
  Table& Table::set(const char *name, T value) {
    Value<T>::Push(L_, value);
    rawSet(name);
    return *this;
  }

}  // SLB3 namespace

#endif

