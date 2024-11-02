#include "delayed_switch.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace delayed {

static const char *const TAG = "delayed switch";

void DelayedSwitch::write_state(bool state) {
  // Acknowledge
  this->publish_state(false);

  if (state) {
    ESP_LOGI(TAG, "Restarting device...");
    // Let MQTT settle a bit
    delay(100);  // NOLINT
    App.safe_reboot();
  }
}
void DelayedSwitch::dump_config() { LOG_SWITCH("", "Restart Switch", this); }

void DelayedSwitch::add_binary_sensor(binary_sensor::BinarySensor *sensor) {
  BinarySensors sensor_channel{
      .binary_sensor = sensor,
  };
  this->binary_sensors_.push_back(sensor_channel);
}

bool DelayedSwitch::are_binary_sensors_off() {
  for (size_t i = 0; i < this->binary_sensors_.size(); i++) {
    auto bs = this->channels_[i];
    if (bs.binary_sensor->state) {
      return false;
    }
  }
  return true;

}  // namespace delayed
}  // namespace esphome
