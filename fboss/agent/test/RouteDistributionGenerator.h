/*
 *  Copyright (c) 2004-present, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#pragma once

#include "fboss/agent/types.h"

#include <folly/IPAddress.h>
#include <map>
#include <memory>
#include <vector>

namespace facebook {
namespace fboss {
class SwitchState;
}
} // namespace facebook

namespace facebook {
namespace fboss {
namespace utility {

using Masklen2NumPrefixes = std::map<uint8_t, uint32_t>;

/*
 * RouteDistributionGenerator takes a input state, distribution spec and
 * chunk size to generate a vector of vector<Routes> (chunk) satisfying that
 * distribution spec and chunk size.
 */
class RouteDistributionGenerator {
 public:
  struct Route {
    folly::CIDRNetwork prefix;
    std::vector<folly::IPAddress> nhops;
  };
  using RouteChunk = std::vector<Route>;
  using RouteChunks = std::vector<RouteChunk>;
  RouteDistributionGenerator(
      const std::shared_ptr<SwitchState>& startingState,
      const Masklen2NumPrefixes& v6DistributionSpec,
      const Masklen2NumPrefixes& v4DistributionSpec,
      unsigned int chunkSize,
      unsigned int ecmpWidth,
      RouterID routerId = RouterID(0));
  /*
   * Compute, cache and return route distribution
   */
  const RouteChunks& get();

  std::shared_ptr<SwitchState> startingState() const {
    return startingState_;
  }
  unsigned int ecmpWidth() const {
    return ecmpWidth_;
  }
  unsigned int chunkSize() const {
    return chunkSize_;
  }

 private:
  template <typename AddrT>
  const std::vector<folly::IPAddress>& getNhops() const;
  template <typename AddT>
  void genRouteDistribution(const Masklen2NumPrefixes& routeDistribution);

  std::shared_ptr<SwitchState> startingState_;
  Masklen2NumPrefixes v6DistributionSpec_;
  Masklen2NumPrefixes v4DistributionSpec_;
  unsigned int chunkSize_;
  unsigned int ecmpWidth_;
  RouterID routerId_{0};
  folly::Optional<RouteChunks> generatedRouteChunks_;
};

class RouteDistributionSwitchStatesGenerator {
 public:
  using SwitchStates = std::vector<std::shared_ptr<SwitchState>>;
  RouteDistributionSwitchStatesGenerator(
      const std::shared_ptr<SwitchState>& startingState,
      const Masklen2NumPrefixes& v6DistributionSpec,
      const Masklen2NumPrefixes& v4DistributionSpec,
      unsigned int chunkSize,
      unsigned int ecmpWidth,
      RouterID routerId = RouterID(0));
  /*
   * Compute, cache and return route distribution
   */
  SwitchStates get();

 private:
  RouteDistributionGenerator routeDistributionGen_;
  folly::Optional<SwitchStates> generatedStates_;
};

} // namespace utility
} // namespace fboss
} // namespace facebook
