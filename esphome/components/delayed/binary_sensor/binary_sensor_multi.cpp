#include "binary_sensor_multi.h"
#include "esphome/core/log.h"

namespace esphome {
namespace delayed {

static const char *const TAG = "binary_sensor_multi";

void BinarySensorMulti::dump_config() { LOG_BINARY_SENSOR("", "Multi Binary Sensor", this); }

void BinarySensorMulti::loop() { this->process_sensors_(); }

bool BinarySensorMulti::check_sensors_() {
  // - check all binary_sensors for its state
  for (size_t i = 0; i < this->channels_.size(); i++) {
    auto bs = this->channels_[i];
    if (bs.binary_sensor->state) {
      return true;
    }
  }
  return false;
}

void BinarySensorMulti::process_sensors_() {
  bool value;
  if (this->ignoring_channels_) {
    value = ignoring_channels_value_;
  } else {
    value = check_sensors_();
  }
  this->publish_state(value);
}

void BinarySensorMulti::add_channel(binary_sensor::BinarySensor *sensor) {
  BinarySensorMultiChannel sensor_channel{
      .binary_sensor = sensor,
  };
  this->channels_.push_back(sensor_channel);
}

bool BinarySensorMulti::is_ignoring_channels() { return this->ignoring_channels_; }

void BinarySensorMulti::set_ignoring_channels(bool ignoring_channels) {
  if (!this->ignore_channels_dedup_.next(ignoring_channels))
    return;
  this->ignoring_channels_ = ignoring_channels;
  if (this->ignoring_channels_) {
    this->publish_state(this->ignoring_channels_value_);
    ESP_LOGI(TAG, "'%s': is ignoring channels, value is %d", this->get_name().c_str(), this->ignoring_channels_value_);
  } else {
    ESP_LOGI(TAG, "'%s': is following channels", this->get_name().c_str());
  }
}

}  // namespace delayed
}  // namespace esphome
