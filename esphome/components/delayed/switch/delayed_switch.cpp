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
    if (!this->is_deactivatable()) {
      ESP_LOGI(TAG, "'%s': turn off is not allowed when turn on channles are on. Time off will be reseted",
               this->get_name().c_str());
      this->set_timeout("OFF", this->time_off_.value(), [this]() { this->write_state(false); });
      return;
    }
  }
  // Verify is the sensor can be on
  if (state && !this->state) {
    if (!this->is_activatable()) {
      ESP_LOGI(TAG, "'%s': turn on is not allowed when blocking channles are on.", this->get_name().c_str());
      return;
    }
  }
  this->publish_state(state);
}

void DelayedSwitch::dump_config() { LOG_SWITCH("", "Delayed Switch", this); }

void DelayedSwitch::add_turn_on_channel(binary_sensor::BinarySensor *sensor) {
  BinarySensors sensor_channel{
      .binary_sensor = sensor,
  };
  ESP_LOGD(TAG, "'%s': was added to turn on channels", sensor->get_name().c_str());
  this->turn_on_channels_.push_back(sensor_channel);
}

void DelayedSwitch::add_blocking_channel(binary_sensor::BinarySensor *sensor) {
  BinarySensors sensor_channel{
      .binary_sensor = sensor,
  };
  ESP_LOGD(TAG, "'%s': was added to blocking channels", sensor->get_name().c_str());
  this->blocking_channels_.push_back(sensor_channel);
}

bool DelayedSwitch::is_activatable() {
  for (size_t i = 0; i < this->blocking_channels_.size(); i++) {
    auto bs = this->blocking_channels_[i];
    if (bs.binary_sensor->state) {
      ESP_LOGI(TAG, "'%s': is active", bs.binary_sensor->get_name().c_str());
      return false;
    }
  }
  return true;
}

bool DelayedSwitch::is_deactivatable() {
  for (size_t i = 0; i < this->turn_on_channels_.size(); i++) {
    auto bs = this->turn_on_channels_[i];
    if (bs.binary_sensor->state) {
      ESP_LOGI(TAG, "'%s': is active", bs.binary_sensor->get_name().c_str());
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

void DelayedSwitch::turn_off_immediate() {
  ESP_LOGD(TAG, "'%s' Turning OFF immediate.", this->get_name().c_str());
  this->publish_state(this->is_inverted());
}

void DelayedSwitch::turn_on_immediate() {
  ESP_LOGD(TAG, "'%s' Turning ON immediate.", this->get_name().c_str());
  this->publish_state(!this->is_inverted());
}

}  // namespace delayed
}  // namespace esphome
