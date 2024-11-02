#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace delayed {

class DelayedSwitch : public switch_::Switch, public Component {
 public:
  void dump_config() override;

 protected:
  void write_state(bool state) override;
};

}  // namespace delayed
}  // namespace esphome
