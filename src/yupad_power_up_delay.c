#include <zephyr/kernel.h>
#include <zephyr/init.h>

/*
 * Right half only: on a cold power-on the yuPad43 is not ready yet when its
 * driver (POST_KERNEL, priority 80) starts, so the driver gives up and the
 * pad never works until the reset button is pressed. Wait 1 s first.
 */
static int yupad_power_up_delay(void) {
#if IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    k_msleep(1000);
#endif
    return 0;
}

SYS_INIT(yupad_power_up_delay, POST_KERNEL, 79);
