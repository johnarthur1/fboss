/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "fboss/agent/hw/HwPortFb303Stats.h"
#include "fboss/agent/hw/StatsConstants.h"

#include <fb303/ServiceData.h>

#include <gtest/gtest.h>
using namespace facebook::fboss;
using namespace facebook::fb303;
namespace {
std::array<folly::StringPiece, 19> kStatNames = {
    kInBytes(),         kInUnicastPkts(),     kInMulticastPkts(),
    kInBroadcastPkts(), kInDiscardsRaw(),     kInDiscards(),
    kInErrors(),        kInPause(),           kInIpv4HdrErrors(),
    kInIpv6HdrErrors(), kInDstNullDiscards(), kOutBytes(),
    kOutUnicastPkts(),  kOutMulticastPkts(),  kOutBroadcastPkts(),
    kOutDiscards(),     kOutErrors(),         kOutPause(),
    kOutEcnCounter(),
};
}

TEST(HwPortFb303StatsTest, StatsInit) {
  HwPortFb303Stats stats("eth1/1/1");
  for (const auto& statName : stats.statNames()) {
    EXPECT_TRUE(fbData->getStatMap()->contains(statName));
  }
}

TEST(HwPortFb303StatsTest, StatsDeInit) {
  std::vector<std::string> statNames;
  {
    HwPortFb303Stats stats("eth1/1/1");
    statNames = stats.statNames();
  }
  for (const auto& statName : statNames) {
    EXPECT_FALSE(fbData->getStatMap()->contains(statName));
  }
}

TEST(HwPortFb303StatsTest, ReInit) {
  constexpr auto kOrigPortName = "eth1/1/1";
  constexpr auto kNewPortName = "eth1/2/1";

  HwPortFb303Stats stats(kOrigPortName);
  stats.reinitPortStats(kNewPortName);
  for (const auto& sName : kStatNames) {
    EXPECT_TRUE(fbData->getStatMap()->contains(
        HwPortFb303Stats::statName(sName, kNewPortName)));
    EXPECT_FALSE(fbData->getStatMap()->contains(
        HwPortFb303Stats::statName(sName, kOrigPortName)));
  }
}