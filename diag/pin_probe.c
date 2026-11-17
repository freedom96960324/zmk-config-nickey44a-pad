/*
 * Temporary diagnostic for the right-half yuPad43 wiring.
 * Active only in the USB-logging central firmware (nickey44a_r_log).
 * 1) Reads each trackpad line with the MCU pull-down and with no pull.
 *    yuPad43 pulls SDA/SCL/RDY up with 4.7k and RST with 10k.
 * 2) Pulses RST (P0.16) low, then drives it HIGH so the IQS9151 is
 *    released from reset before the I2C driver initialises.
 */
#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include <zephyr/logging/log.h>
#include <hal/nrf_gpio.h>

LOG_MODULE_REGISTER(pin_probe, LOG_LEVEL_INF);

#if defined(CONFIG_ZMK_USB_LOGGING) && defined(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)

#define RST_PIN NRF_GPIO_PIN_MAP(0, 16)

struct probe_pin {
    uint32_t pin;
    const char *name;
};

static const struct probe_pin pins[] = {
    {NRF_GPIO_PIN_MAP(0, 9), "SDA green P0.09"},
    {NRF_GPIO_PIN_MAP(0, 10), "SCL blue P0.10"},
    {NRF_GPIO_PIN_MAP(1, 10), "RDY yellow P1.10"},
    {RST_PIN, "RST white P0.16"},
};

static void read_all(nrf_gpio_pin_pull_t pull, const char *label) {
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        nrf_gpio_cfg_input(pins[i].pin, pull);
    }
    k_busy_wait(2000);
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        uint32_t v = nrf_gpio_pin_read(pins[i].pin);
        LOG_WRN("PIN_PROBE %s %s = %s", label, pins[i].name, v ? "HIGH" : "LOW");
    }
}

static int pin_probe(void) {
    k_busy_wait(200000);
    read_all(NRF_GPIO_PIN_PULLDOWN, "pulldown");
    read_all(NRF_GPIO_PIN_NOPULL, "nopull");
    for (size_t i = 0; i < ARRAY_SIZE(pins); i++) {
        nrf_gpio_cfg_default(pins[i].pin);
    }

    /* Hardware reset of the IQS9151, then keep RST driven high. */
    nrf_gpio_pin_clear(RST_PIN);
    nrf_gpio_cfg_output(RST_PIN);
    k_busy_wait(10000);
    nrf_gpio_pin_set(RST_PIN);
    k_busy_wait(50000);
    LOG_WRN("PIN_PROBE RST pulsed and now driven HIGH, readback = %s",
            nrf_gpio_pin_out_read(RST_PIN) ? "HIGH" : "LOW");
    return 0;
}

SYS_INIT(pin_probe, PRE_KERNEL_1, 0);

#endif
