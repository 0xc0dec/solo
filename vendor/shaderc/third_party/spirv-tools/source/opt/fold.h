// Copyright (c) 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef LIBSPIRV_UTIL_FOLD_H_
#define LIBSPIRV_UTIL_FOLD_H_

#include "constants.h"
#include "def_use_manager.h"

#include <cstdint>
#include <vector>

namespace spvtools {
namespace opt {

uint32_t FoldScalars(SpvOp opcode,
                     const std::vector<analysis::Constant*>& operands);

std::vector<uint32_t> FoldVectors(
    SpvOp opcode, uint32_t num_dims,
    const std::vector<analysis::Constant*>& operands);

}  // namespace opt
}  // namespace spvtools

#endif  // LIBSPIRV_UTIL_FOLD_H_
