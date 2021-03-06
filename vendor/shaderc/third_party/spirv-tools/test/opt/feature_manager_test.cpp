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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <algorithm>

#include "opt/build_module.h"
#include "opt/ir_context.h"

using namespace spvtools;

using FeatureManagerTest = ::testing::Test;

TEST_F(FeatureManagerTest, MissingExtension) {
  const std::string text = R"(
OpCapability Shader
OpMemoryModel Logical GLSL450
  )";

  std::unique_ptr<ir::IRContext> context =
      BuildModule(SPV_ENV_UNIVERSAL_1_2, nullptr, text);
  ASSERT_NE(context, nullptr);

  ASSERT_FALSE(context->get_feature_mgr()->HasExtension(
      libspirv::Extension::kSPV_KHR_variable_pointers));
}

TEST_F(FeatureManagerTest, OneExtension) {
  const std::string text = R"(
OpCapability Shader
OpMemoryModel Logical GLSL450
OpExtension "SPV_KHR_variable_pointers"
  )";

  std::unique_ptr<ir::IRContext> context =
      BuildModule(SPV_ENV_UNIVERSAL_1_2, nullptr, text);
  ASSERT_NE(context, nullptr);

  ASSERT_TRUE(context->get_feature_mgr()->HasExtension(
      libspirv::Extension::kSPV_KHR_variable_pointers));
}

TEST_F(FeatureManagerTest, NotADifferentExtension) {
  const std::string text = R"(
OpCapability Shader
OpMemoryModel Logical GLSL450
OpExtension "SPV_KHR_variable_pointers"
  )";

  std::unique_ptr<ir::IRContext> context =
      BuildModule(SPV_ENV_UNIVERSAL_1_2, nullptr, text);
  ASSERT_NE(context, nullptr);

  ASSERT_FALSE(context->get_feature_mgr()->HasExtension(
      libspirv::Extension::kSPV_KHR_storage_buffer_storage_class));
}

TEST_F(FeatureManagerTest, TwoExtensions) {
  const std::string text = R"(
OpCapability Shader
OpMemoryModel Logical GLSL450
OpExtension "SPV_KHR_variable_pointers"
OpExtension "SPV_KHR_storage_buffer_storage_class"
  )";

  std::unique_ptr<ir::IRContext> context =
      BuildModule(SPV_ENV_UNIVERSAL_1_2, nullptr, text);
  ASSERT_NE(context, nullptr);

  ASSERT_TRUE(context->get_feature_mgr()->HasExtension(
      libspirv::Extension::kSPV_KHR_variable_pointers));
  ASSERT_TRUE(context->get_feature_mgr()->HasExtension(
      libspirv::Extension::kSPV_KHR_storage_buffer_storage_class));
}
