#include "binary_sensor_multi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace binary_sensor_multi {

static const char *const TAG = "binary_sensor_multi";

void BinarySensorMulti::dump_config() { LOG_BINARY_SENSOR("", "Multi Binary Sensor", this); }

void BinarySensorMulti::loop() { this->process_sensors_(); }

void BinarySensorMulti::process_sensors_() {
  uint8_t num_active_sensors = 0;

  // - check all binary_sensors for its state
  //  - if active, add its value to total_current_value.
  // - creates a bitmask for the binary_sensor states on all channels
  for (size_t i = 0; i < this->channels_.size(); i++) {
    auto bs = this->channels_[i];
    if (bs.binary_sensor->state) {
      num_active_sensors++;
    }
  }

  this->publish_state((num_active_sensors > 0));
}

void BinarySensorMulti::add_channel(binary_sensor::BinarySensor *sensor) {
  BinarySensorMultiChannel sensor_channel{
      .binary_sensor = sensor,
  };
  this->channels_.push_back(sensor_channel);
}

void BinarySensorMulti::turn_off_immedite() {
  if (!this->publish_dedup_.next(false))
    return;
  this->send_state_internal(false, false);
}

}  // namespace binary_sensor_multi
}  // namespace esphome
