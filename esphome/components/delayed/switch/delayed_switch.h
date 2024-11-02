#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/switch/switch.h"

#include <vector>

namespace esphome {
namespace delayed {

struct BinarySensors {
  binary_sensor::BinarySensor *binary_sensor;
};

class DelayedSwitch : public switch_::Switch, public Component {
 public:
  void dump_config() override;
  /**
   * Add binary_sensors to the group when only one parameter is needed for the configured mapping type.
   *
   * @param *sensor The binary sensor.
   * @param value  The value this binary_sensor represents
   */
  void add_binary_sensor(binary_sensor::BinarySensor *sensor);

 protected:
  void write_state(bool state) override;

 private:
  std::vector<BinarySensors> binary_sensors_{};
  bool are_binary_sensors_off();
};

}  // namespace delayed
}  // namespace esphome
