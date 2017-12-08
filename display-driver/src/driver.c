#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#
#include "../inc/pinout.h"
#include "../inc/display.h"

/**
 * @brief Initialize the driver that handles the display.
 */
static int __init display_driver_init(void) {
    printk("Starting DisplayDriver! So long! <3\n");

    if (pinout_init() != 0x0) {
        return -1;
    }

    if (display_device_init() != 0x0) {
        pinout_free();

        return -1;
    }

    get_display_inst();

    return 0;
}

/**
 * @brief Release the driver that handles the display and
 * free all used resources.
 */
static void __exit display_driver_exit(void) {
    printk("Exiting DisplayDriver ! <3\n");
    pinout_free();
    display_device_free();

    free_display_inst();
}

module_init(display_driver_init);
module_exit(display_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guillaume Garcia & Alexandre V. Jamet");
MODULE_DESCRIPTION("Driver de commande d'un sonar utilise en tant que capteur de proximite");
