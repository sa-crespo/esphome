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
    if (this->delay_.next(true)) {
      this->cancel_timeout("OFF");
    }
    if (this->state == false) {
      ESP_LOGD(TAG, "Turning on light");
    }
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
  if (this->state) {
    if (this->delay_.next(false)) {
      ESP_LOGD(TAG, "Ligths will be out in 30 secs");
      this->set_timeout("OFF", delay, [this]() {
        ESP_LOGD(TAG, "Delay is off.");
        this->publish_state(false);
      });
    }
  }
}

void BinarySensorMulti::turn_off_immediate() {
  if (this->state) {
    if (!check_sensors_()) {
      if (this->delay_.next(true))
        this->cancel_timeout("OFF");
      this->publish_state(false);
    } else {
      ESP_LOGW(TAG, "Channels are still on");
    }
  } else {
    ESP_LOGW(TAG, "Light is already off");
  }
}

}  // namespace binary_sensor_multi
}  // namespace esphome
