#include "binary_sensor_multi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace delayed {

static const char *const TAG = "binary_sensor_multi";

void BinarySensorMulti::dump_config() { LOG_BINARY_SENSOR("", "Multi Binary Sensor", this); }

void BinarySensorMulti::loop() { this->process_sensors_(); }

bool BinarySensorMulti::check_sensors_() {
  uint8_t num_active_sensors = 0;

  // - return false when sensor is ignoring channels
  if (this->ignoring_channels_) {
    return false;
  }
  // - check all binary_sensors for its state
  for (size_t i = 0; i < this->channels_.size(); i++) {
    auto bs = this->channels_[i];
    if (bs.binary_sensor->state) {
      num_active_sensors++;
    }
  }
  return (num_active_sensors > 0);
}

void BinarySensorMulti::process_sensors_() { this->publish_state(check_sensors_()); }

void BinarySensorMulti::add_channel(binary_sensor::BinarySensor *sensor) {
  BinarySensorMultiChannel sensor_channel{
      .binary_sensor = sensor,
  };
  this->channels_.push_back(sensor_channel);
}

bool BinarySensorMulti::is_ignoring_channels() { return this->ignoring_channels_; }

void BinarySensorMulti::set_ignoring_channels(bool ignoring_channels) { this->ignoring_channels_ = ignoring_channels; }

}  // namespace delayed
}  // namespace esphome
