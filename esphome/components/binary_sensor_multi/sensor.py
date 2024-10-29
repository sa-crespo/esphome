import esphome.codegen as cg
from esphome.components import binary_sensor, sensor
import esphome.config_validation as cv
from esphome.const import CONF_BINARY_SENSOR, CONF_CHANNELS, ICON_CHECK_CIRCLE_OUTLINE

DEPENDENCIES = ["binary_sensor"]

binary_sensor_multi_ns = cg.esphome_ns.namespace("binary_sensor_multi")
BinarySensorMulti = binary_sensor_multi_ns.class_(
    "BinarySensorMulti", cg.Component, sensor.Sensor
)

entry_one_parameter = {
    cv.Required(CONF_BINARY_SENSOR): cv.use_id(binary_sensor.BinarySensor),
}


CONFIG_SCHEMA = binary_sensor.binary_sensor_schema(
    BinarySensorMulti,
    icon=ICON_CHECK_CIRCLE_OUTLINE,
).extend(
    {
        cv.Required(CONF_CHANNELS): cv.All(
            cv.ensure_list(entry_one_parameter), cv.Length(min=1, max=64)
        ),
    }
)


async def to_code(config):
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    for ch in config[CONF_CHANNELS]:
        input_var = await cg.get_variable(ch[CONF_BINARY_SENSOR])
        cg.add(var.add_channel(input_var))
