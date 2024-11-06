#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

#include <vector>

namespace esphome {
namespace delayed {

struct BinarySensorMultiChannel {
  binary_sensor::BinarySensor *binary_sensor;
};

/** Class to map one or more binary_sensors to one Sensor.
 *
 * Each binary sensor has configured parameters that each mapping type uses to compute the single numerical result
 */
class BinarySensorMulti : public binary_sensor::BinarySensorInitiallyOff, public Component {
 public:
  void dump_config() override;

  /**
   * The loop calls the configured type processing method
   *
   * The processing method loops through all sensors and calculates the numerical result
   * The result is only published if a binary sensor state has changed or, for some types, on initial boot
   */
  void loop() override;

  /**
   * Add binary_sensors to the group when only one parameter is needed for the configured mapping type.
   *
   * @param *sensor The binary sensor.
   * @param value  The value this binary_sensor represents
   */
  void add_channel(binary_sensor::BinarySensor *sensor);
  /**
   * Returns true when sensor is ignoring the status of the channels.
   *
   */
  bool is_ignoring_channels();
  void set_ignoring_channels(bool ignoring_channels);

  void set_ignoring_channels_value(bool ignoring_channels_value) {
    this->ignoring_channels_value_ = ignoring_channels_value;
  };

 protected:
  std::vector<BinarySensorMultiChannel> channels_{};
  void process_sensors_();
  bool check_sensors_();

  Deduplicator<bool> delay_;

 private:
  bool ignoring_channels_{false};
  bool ignoring_channels_value_{false};
  Deduplicator<bool> ignore_channels_dedup_;
};

}  // namespace delayed
}  // namespace esphome
