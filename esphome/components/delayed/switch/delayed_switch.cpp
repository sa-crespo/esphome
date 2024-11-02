#include "delayed_switch.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace delayed {

static const char *const TAG = "delayed switch";

DelayedSwitch::DelayedSwitch() {
  add_on_state_callback([this](bool state) { this->state_published(state); });
}

void DelayedSwitch::write_state(bool state) {
  // Verify is the sensor can be off
  if (!state && this->state) {
    if (!this->are_binary_sensors_off()) {
      ESP_LOGD(TAG, "Turn off is not allowed when binary sensors are still on. Time off will be reseted");
      this->set_timeout("OFF", this->time_off_.value(), [this]() { this->write_state(false); });
      return;
    }
  }
  this->publish_state(state);
}
void DelayedSwitch::dump_config() { LOG_SWITCH("", "Delayed Switch", this); }

void DelayedSwitch::add_binary_sensor(binary_sensor::BinarySensor *sensor) {
  BinarySensors sensor_channel{
      .binary_sensor = sensor,
  };
  this->binary_sensors_.push_back(sensor_channel);
}

bool DelayedSwitch::are_binary_sensors_off() {
  for (size_t i = 0; i < this->binary_sensors_.size(); i++) {
    auto bs = this->binary_sensors_[i];
    if (bs.binary_sensor->state) {
      return false;
    }
  }
  return true;
}

void DelayedSwitch::state_published(bool state) {
  ESP_LOGD(TAG, "State Changed");
  if (state) {
    ESP_LOGD(TAG, "Switch is on time_off will be set");
    this->set_timeout("OFF", this->time_off_.value(), [this]() { this->write_state(false); });
  } else {
    ESP_LOGD(TAG, "Switch is off time_off will be cancel");
    this->cancel_timeout("OFF");
  }
}

}  // namespace delayed
}  // namespace esphome
