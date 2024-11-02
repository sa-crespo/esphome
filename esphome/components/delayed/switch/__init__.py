import esphome.codegen as cg
from esphome.components import binary_sensor, switch
import esphome.config_validation as cv
from esphome.const import (
    CONF_BINARY_SENSOR,
    CONF_BINARY_SENSORS,
    ENTITY_CATEGORY_CONFIG,
    ICON_LIGHTBULB,
)

DEPENDENCIES = ["binary_sensor"]

delayed_ns = cg.esphome_ns.namespace("delayed")
DelayedSwitch = delayed_ns.class_("DelayedSwitch", switch.Switch, cg.Component)


entry_one_binary_sensor = {
    cv.Required(CONF_BINARY_SENSOR): cv.use_id(binary_sensor.BinarySensor),
}

CONFIG_SCHEMA = (
    switch.switch_schema(
        DelayedSwitch,
        icon=ICON_LIGHTBULB,
        entity_category=ENTITY_CATEGORY_CONFIG,
        block_inverted=True,
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(
        {
            cv.Required(CONF_BINARY_SENSORS): cv.All(
                cv.ensure_list(entry_one_binary_sensor), cv.Length(min=1, max=64)
            ),
        }
    )
)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    for ch in config[CONF_BINARY_SENSORS]:
        input_var = await cg.get_variable(ch[CONF_BINARY_SENSOR])
        cg.add(var.add_binary_sensor(input_var))
