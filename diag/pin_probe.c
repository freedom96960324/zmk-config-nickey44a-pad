/*
 * Temporary diagnostic for the right-half yuPad43 wiring.
 * Active only in the USB-logging central firmware (nickey44a_r_log).
 * Before the I2C driver claims the pins, each trackpad line is read with
 * the MCU's internal pull-down enabled. yuPad43 pulls SDA/SCL/RDY up with
 * 4.7k and RST with 10k, so a line that reads HIGH is connected to a
 * powered pad; a line that reads LOW is open (or the pad is unpowered).
 */
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <hal/nrf_gpio.h>

LOG_MODULE_REGISTER(pin_probe, LOG_LEVEL_INF);

#if defined(CONFIG_ZMK_USB_LOGGING) && defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

struct probe_pin {
    uint32_t pin;
    const char *name;
};

static const struct probe_pin pins[] = {
    {NRF_GPIO_PIN_MAP(0, 9), "SDA green P0.09"},
    {NRF_GPIO_PIN_MAP(0, 10), "SCL blue P0.10"},
    {NRF_GPIO_PIN_MAP(1, 10), "RDY yellow P1.10"},
    {NRF_GPIO_PIN_MAP(0, 16), "RST white P0.16"},
};

static int pin_probe(void) {
    k_busy_wait(200000);
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        nrf_gpio_cfg_input(pins[i].pin, NRF_GPIO_PIN_PULLDOWN);
    }
    k_busy_wait(2000);
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        uint32_t v = nrf_gpio_pin_read(pins[i].pin);
        LOG_WRN("PIN_PROBE %s = %s", pins[i].name, v ? "HIGH connected" : "LOW NOT-connected");
    }
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        nrf_gpio_cfg_default(pins[i].pin);
    }
    return 0;
}

SYS_INIT(pin_probe, PRE_KERNEL_1, 0);

#endif
