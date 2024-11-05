from esphome import automation
from esphome.automation import Condition, maybe_simple_id
import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_BINARY_SENSOR,
    CONF_CHANNELS,
    CONF_ID,
    ICON_CHECK_CIRCLE_OUTLINE,
)

DEPENDENCIES = ["binary_sensor"]

delayed_ns = cg.esphome_ns.namespace("delayed")
BinarySensorMulti = delayed_ns.class_(
    "BinarySensorMulti", binary_sensor.BinarySensor, cg.Component
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


# Condition
BinarySensorCondition = delayed_ns.class_(
    "BinarySensorMultiIgnoringChannelsCondition", Condition
)

BINARY_SENSOR_CONDITION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(BinarySensorMulti),
    }
)


@automation.register_condition(
    "binary_sensor.is_ignoring_channels",
    BinarySensorCondition,
    BINARY_SENSOR_CONDITION_SCHEMA,
)
async def binary_sensor_is_on_to_code(config, condition_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(condition_id, template_arg, paren, True)


@automation.register_condition(
    "binary_sensor.is_following_channels",
    BinarySensorCondition,
    BINARY_SENSOR_CONDITION_SCHEMA,
)
async def binary_sensor_is_off_to_code(config, condition_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(condition_id, template_arg, paren, False)


BinarySensorMultiIgnoreChannelsAction = delayed_ns.class_(
    "BinarySensorMultiIgnoreChannelsAction", automation.Action
)

BINARY_SENSOR_ACTION_SCHEMA = maybe_simple_id(
    {cv.Required(CONF_ID): cv.use_id(BinarySensorMulti)}
)


@automation.register_action(
    "binary_sensor.ignore_channels",
    BinarySensorMultiIgnoreChannelsAction,
    BINARY_SENSOR_ACTION_SCHEMA,
)
async def ignore_channels_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren, True)
    return var


@automation.register_action(
    "binary_sensor.follow_channels",
    BinarySensorMultiIgnoreChannelsAction,
    BINARY_SENSOR_ACTION_SCHEMA,
)
async def follow_channels_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren, False)
    return var
