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

template<typename... Ts> class TurnOnImmediateAction : public Action<Ts...> {
 public:
  explicit TurnOnImmediateAction(DelayedSwitch *a_delayed_switch) : delayed_switch_(a_delayed_switch) {}

  void play(Ts... x) override { this->delayed_switch_->turn_on_immediate(); }

 protected:
  DelayedSwitch *delayed_switch_;
};

template<typename... Ts> class TurnOnTemporaryAction : public Action<Ts...> {
 public:
  explicit TurnOnTemporaryAction(DelayedSwitch *a_delayed_switch) : delayed_switch_(a_delayed_switch) {}

  void play(Ts... x) override { this->delayed_switch_->turn_on_temporary(off_delay_.value()); }
  template<typename T> void set_off_delay(T delay) { this->off_delay_ = delay; }

 protected:
  DelayedSwitch *delayed_switch_;
  TemplatableValue<uint32_t> off_delay_{};
};

template<typename... Ts> class TurnOnAction : public Action<Ts...> {
 public:
  explicit TurnOnAction(DelayedSwitch *a_delayed_switch) : delayed_switch_(a_delayed_switch) {}

  void play(Ts... x) override {
    if (off_delay_.has_value())
      this->delayed_switch_->turn_on(off_delay_.value());
    else
      this->delayed_switch_->turn_on(0);
  }
  template<typename T> void set_off_delay(T delay) { this->off_delay_ = delay; }

 protected:
  DelayedSwitch *delayed_switch_;
  TemplatableValue<uint32_t> off_delay_{};
};

}  // namespace delayed
}  // namespace esphome
