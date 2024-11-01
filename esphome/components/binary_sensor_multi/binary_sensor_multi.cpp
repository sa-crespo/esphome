#include "binary_sensor_multi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace binary_sensor_multi {

static const char *const TAG = "binary_sensor_multi";

void BinarySensorMulti::dump_config() { LOG_BINARY_SENSOR("", "Multi Binary Sensor", this); }

void BinarySensorMulti::loop() { this->process_sensors_(); }

bool BinarySensorMulti::check_sensors_() {
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
  return (num_active_sensors > 0);
}

void BinarySensorMulti::process_sensors_() {
  if (check_sensors_()) {
    this->cancel_timeout("OFF");
    this->publish_state(true);
  } else {
    turn_off_delayed(30000);
  }
}

void BinarySensorMulti::add_channel(binary_sensor::BinarySensor *sensor) {
  BinarySensorMultiChannel sensor_channel{
      .binary_sensor = sensor,
  };
  this->channels_.push_back(sensor_channel);
}

void BinarySensorMulti::turn_off_delayed(uint32_t delay) {
  if (!this->publish_dedup_.next(false))
    return;
  ESP_LOGD(TAG, "Ligths will be out in 30 secs");
  this->set_timeout("OFF", delay, [this]() { this->publish_state(false); });
}

void BinarySensorMulti::turn_off_immediate() {
  if (!this->publish_dedup_.next(false))
    return;
  if (!check_sensors_()) {
    ESP_LOGD(TAG, "Ligths will be out now");
    this->cancel_timeout("OFF");
    this->publish_state(false);
  }
}

}  // namespace binary_sensor_multi
}  // namespace esphome
