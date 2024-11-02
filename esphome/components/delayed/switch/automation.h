#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "delayed_switch.h"

namespace esphome {
namespace delayed {

template<typename... Ts> class TurnOffImmediateAction : public Action<Ts...> {
 public:
  explicit TurnOffImmediateAction(DelayedSwitch *a_delayed_switch) : delayed_switch_(a_delayed_switch) {}

  void play(Ts... x) override { this->delayed_switch_->turn_off_immediate(); }

 protected:
  DelayedSwitch *delayed_switch_;
};

}  // namespace delayed
}  // namespace esphome
