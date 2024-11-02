from esphome import automation
from esphome.automation import maybe_simple_id
import esphome.codegen as cg
from esphome.components import binary_sensor, switch
import esphome.config_validation as cv
from esphome.const import (
    CONF_BINARY_SENSOR,
    CONF_BINARY_SENSORS,
    CONF_ID,
    ENTITY_CATEGORY_CONFIG,
    ICON_LIGHTBULB,
)

CONF_TIME_OFF = "time_off"
CONF_TIME_ADD = "time_add"

DEPENDENCIES = ["binary_sensor"]

delayed_ns = cg.esphome_ns.namespace("delayed")
DelayedSwitch = delayed_ns.class_("DelayedSwitch", switch.Switch, cg.Component)


TurnOffImmediateAction = delayed_ns.class_("TurnOffImmediateAction", automation.Action)


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
            cv.Required(CONF_TIME_OFF): cv.templatable(
                cv.positive_time_period_milliseconds
            ),
            cv.Required(CONF_BINARY_SENSORS): cv.All(
                cv.ensure_list(entry_one_binary_sensor), cv.Length(min=1, max=64)
            ),
        }
    )
)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)
    template_ = await cg.templatable(config[CONF_TIME_OFF], [], cg.uint32)
    cg.add(var.set_time_off(template_))

    for ch in config[CONF_BINARY_SENSORS]:
        input_var = await cg.get_variable(ch[CONF_BINARY_SENSOR])
        cg.add(var.add_binary_sensor(input_var))


SWITCH_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(DelayedSwitch),
    }
)


@automation.register_action(
    "switch.turn_off_immediate", TurnOffImmediateAction, SWITCH_ACTION_SCHEMA
)
async def switch_toggle_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)
