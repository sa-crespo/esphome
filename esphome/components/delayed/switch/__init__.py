from esphome import automation
from esphome.automation import maybe_simple_id
import esphome.codegen as cg
from esphome.components import binary_sensor, switch
import esphome.config_validation as cv
from esphome.const import (
    CONF_BINARY_SENSOR,
    CONF_ID,
    ENTITY_CATEGORY_CONFIG,
    ICON_LIGHTBULB,
)

CONF_TIME_OFF = "time_off"
CONF_TIME_ADD = "time_add"

CONF_TURNON_CHANNELS = "turn_on_channels"
CONF_BLOCKING_CHANNELS = "blocking_channels"

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
            cv.Required(CONF_TIME_OFF): cv.templatable(
                cv.positive_time_period_milliseconds
            ),
            cv.Required(CONF_TURNON_CHANNELS): cv.All(
                cv.ensure_list(entry_one_binary_sensor), cv.Length(min=1, max=64)
            ),
            cv.Optional(CONF_BLOCKING_CHANNELS): cv.All(
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

    for ch in config[CONF_TURNON_CHANNELS]:
        input_var = await cg.get_variable(ch[CONF_BINARY_SENSOR])
        cg.add(var.add_turn_on_channel(input_var))

    for ch in config[CONF_BLOCKING_CHANNELS]:
        input_var = await cg.get_variable(ch[CONF_BINARY_SENSOR])
        cg.add(var.add_blocking_channel(input_var))


SWITCH_ACTION_SCHEMA = maybe_simple_id(
    {
        cv.Required(CONF_ID): cv.use_id(DelayedSwitch),
    }
)

TurnOffImmediateAction = delayed_ns.class_("TurnOffImmediateAction", automation.Action)


@automation.register_action(
    "switch.turn_off_immediate", TurnOffImmediateAction, SWITCH_ACTION_SCHEMA
)
async def turn_off_immediate_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


TurnOnImmediateAction = delayed_ns.class_("TurnOnImmediateAction", automation.Action)


@automation.register_action(
    "switch.turn_on_immediate", TurnOnImmediateAction, SWITCH_ACTION_SCHEMA
)
async def turn_on_immediate_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    return cg.new_Pvariable(action_id, template_arg, paren)


TurnOnTemporaryAction = delayed_ns.class_("TurnOnTemporaryAction", automation.Action)


@automation.register_action(
    "switch.turn_on_temporary",
    TurnOnTemporaryAction,
    maybe_simple_id(
        {
            cv.Required(CONF_ID): cv.use_id(DelayedSwitch),
            cv.Required(CONF_TIME_OFF): cv.templatable(
                cv.positive_time_period_milliseconds
            ),
        }
    ),
)
async def turn_on_temporary_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    template_ = await cg.templatable(config[CONF_TIME_OFF], [], cg.uint32)
    cg.add(var.set_off_delay(template_))

    return var


TurnOnAction = delayed_ns.class_("TurnOnAction", automation.Action)


@automation.register_action(
    "switch.turn_on",
    TurnOnAction,
    maybe_simple_id(
        {
            cv.Required(CONF_ID): cv.use_id(DelayedSwitch),
            cv.Optional(CONF_TIME_OFF): cv.templatable(
                cv.positive_time_period_milliseconds
            ),
        }
    ),
)
async def turn_on_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[CONF_ID])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    if CONF_TIME_OFF in config:
        template_ = await cg.templatable(config[CONF_TIME_OFF], [], cg.uint32)
        cg.add(var.set_off_delay(template_))
    return var
