/*
 * yuPad43 start-up helper for Nickey44A.
 *
 * All builds: wait 300 ms at boot so the IQS9151 has finished its own
 * power-up before the I2C driver initialises it.
 *
 * Right USB-logging build only (nickey44a_r_log): also log the level of each
 * trackpad line for wiring diagnosis.
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
};

static void probe_pins(void) {
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        nrf_gpio_cfg_input(pins[i].pin, NRF_GPIO_PIN_PULLDOWN);
    }
    k_busy_wait(2000);
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        uint32_t v = nrf_gpio_pin_read(pins[i].pin);
        LOG_WRN("PIN_PROBE %s = %s", pins[i].name, v ? "HIGH" : "LOW");
    }
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        nrf_gpio_cfg_default(pins[i].pin);
    }
}

#endif

static int yupad_startup_delay(void) {
    k_busy_wait(300000);
#if defined(CONFIG_ZMK_USB_LOGGING) && defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    probe_pins();
#endif
    return 0;
}

SYS_INIT(yupad_startup_delay, PRE_KERNEL_1, 0);
