#pragma once

#include <cinttypes>
#include <utility>
#include <vector>

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/core/hal.h"
#include "binary_sensor_multi.h"

namespace esphome {
namespace delayed {

template<typename... Ts> class BinarySensorMultiIgnoringChannelsCondition : public Condition<Ts...> {
 public:
  BinarySensorMultiIgnoringChannelsCondition(BinarySensorMulti *parent, bool ignoring_channels)
      : parent_(parent), ignoring_channels_(ignoring_channels) {}
  bool check(Ts... x) override { return this->parent_->is_ignoring_channels() == this->ignoring_channels_; }

 protected:
  BinarySensorMulti *parent_;
  bool ignoring_channels_;
};

template<typename... Ts> class BinarySensorMultiIgnoreChannelsAction : public Action<Ts...> {
 public:
  explicit BinarySensorMultiIgnoreChannelsAction(BinarySensorMulti *parent, bool ignore_channels)
      : parent_(parent), ignore_channels_(ignore_channels) {}
  void play(Ts... x) override { this->parent_->set_ignoring_channels(this->ignore_channels_); }

 protected:
  BinarySensorMulti *parent_;
  bool ignore_channels_;
};

}  // namespace delayed
}  // namespace esphome
