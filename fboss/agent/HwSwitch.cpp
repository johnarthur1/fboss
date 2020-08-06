/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#include "fboss/agent/HwSwitch.h"
#include "fboss/agent/hw/HwSwitchStats.h"
#include "fboss/agent/hw/switch_asics/HwAsic.h"

#include "fboss/agent/FbossError.h"

#include <folly/FileUtil.h>
#include <folly/experimental/TestUtil.h>

#include <fb303/ThreadCachedServiceData.h>

namespace facebook::fboss {

std::string HwSwitch::getDebugDump() const {
  folly::test::TemporaryDirectory tmpDir;
  auto fname = tmpDir.path().string() + "hw_debug_dump";
  dumpDebugState(fname);
  std::string out;
  if (!folly::readFile(fname.c_str(), out)) {
    throw FbossError("Unable get debug dump");
  }
  return out;
}

HwSwitchStats* HwSwitch::getSwitchStats() const {
  static HwSwitchStats hwSwitchStats(
      fb303::ThreadCachedServiceData::get()->getThreadStats(),
      getPlatform()->getAsic()->getVendor());
  return &hwSwitchStats;
}

} // namespace facebook::fboss
