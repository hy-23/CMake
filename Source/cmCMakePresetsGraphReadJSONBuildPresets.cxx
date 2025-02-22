/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file Copyright.txt or https://cmake.org/licensing for details.  */
#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include <cm/optional>
#include <cmext/string_view>

#include <cm3p/json/value.h>

#include "cmCMakePresetsGraph.h"
#include "cmCMakePresetsGraphInternal.h"
#include "cmJSONHelpers.h"

namespace {
using ReadFileResult = cmCMakePresetsGraph::ReadFileResult;
using BuildPreset = cmCMakePresetsGraph::BuildPreset;

auto const BuildPresetHelper =
  cmJSONObjectHelper<BuildPreset, ReadFileResult>(
    ReadFileResult::READ_OK, ReadFileResult::INVALID_PRESET, false)
    .Bind("name"_s, &BuildPreset::Name,
          cmCMakePresetsGraphInternal::PresetStringHelper)
    .Bind("inherits"_s, &BuildPreset::Inherits,
          cmCMakePresetsGraphInternal::PresetVectorOneOrMoreStringHelper,
          false)
    .Bind("hidden"_s, &BuildPreset::Hidden,
          cmCMakePresetsGraphInternal::PresetBoolHelper, false)
    .Bind<std::nullptr_t>("vendor"_s, nullptr,
                          cmCMakePresetsGraphInternal::VendorHelper(
                            ReadFileResult::INVALID_PRESET),
                          false)
    .Bind("displayName"_s, &BuildPreset::DisplayName,
          cmCMakePresetsGraphInternal::PresetStringHelper, false)
    .Bind("description"_s, &BuildPreset::Description,
          cmCMakePresetsGraphInternal::PresetStringHelper, false)
    .Bind("environment"_s, &BuildPreset::Environment,
          cmCMakePresetsGraphInternal::EnvironmentMapHelper, false)
    .Bind("configurePreset"_s, &BuildPreset::ConfigurePreset,
          cmCMakePresetsGraphInternal::PresetStringHelper, false)
    .Bind("inheritConfigureEnvironment"_s,
          &BuildPreset::InheritConfigureEnvironment,
          cmCMakePresetsGraphInternal::PresetOptionalBoolHelper, false)
    .Bind("jobs"_s, &BuildPreset::Jobs,
          cmCMakePresetsGraphInternal::PresetOptionalIntHelper, false)
    .Bind("targets"_s, &BuildPreset::Targets,
          cmCMakePresetsGraphInternal::PresetVectorOneOrMoreStringHelper,
          false)
    .Bind("configuration"_s, &BuildPreset::Configuration,
          cmCMakePresetsGraphInternal::PresetStringHelper, false)
    .Bind("cleanFirst"_s, &BuildPreset::CleanFirst,
          cmCMakePresetsGraphInternal::PresetOptionalBoolHelper, false)
    .Bind("verbose"_s, &BuildPreset::Verbose,
          cmCMakePresetsGraphInternal::PresetOptionalBoolHelper, false)
    .Bind("nativeToolOptions"_s, &BuildPreset::NativeToolOptions,
          cmCMakePresetsGraphInternal::PresetVectorStringHelper, false)
    .Bind("condition"_s, &BuildPreset::ConditionEvaluator,
          cmCMakePresetsGraphInternal::PresetConditionHelper, false);
}

namespace cmCMakePresetsGraphInternal {
ReadFileResult BuildPresetsHelper(std::vector<BuildPreset>& out,
                                  const Json::Value* value)
{
  static auto const helper = cmJSONVectorHelper<BuildPreset, ReadFileResult>(
    ReadFileResult::READ_OK, ReadFileResult::INVALID_PRESETS,
    BuildPresetHelper);

  return helper(out, value);
}
}
