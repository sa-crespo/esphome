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
  explicit DelayedSwitch();
  void dump_config() override;
  /**
   * Add binary_sensors to the group when only one parameter is needed for the configured mapping type.
   *
   * @param *sensor The binary sensor.
   * @param value  The value this binary_sensor represents
   */
  void add_turn_on_channel(binary_sensor::BinarySensor *sensor);
  void add_blocking_channel(binary_sensor::BinarySensor *sensor);
  template<typename T> void set_time_off(T time_off) { this->time_off_ = time_off; }

  virtual void turn_off_immediate();
  virtual void turn_on_immediate();
  virtual void turn_on_temporary(uint32_t temporary_time_off);
  virtual void turn_on(uint32_t temporary_time_off);

 protected:
  void write_state(bool state) override;
  virtual void state_published(bool state);
  uint32_t get_time_off();
  bool is_deactivatable();
  bool is_activatable();

 private:
  std::vector<BinarySensors> turn_on_channels_{};
  std::vector<BinarySensors> blocking_channels_{};
  TemplatableValue<uint32_t> time_off_{};
  uint32_t temporary_time_off_{0};
};

}  // namespace delayed
}  // namespace esphome
