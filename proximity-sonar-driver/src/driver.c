#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/gpio.h>
#
#include "../inc/pinout.h"
#include "../inc/interrupt.h"
#include "../inc/sonar.h"

/**
 * @mainpage
 *
 * This Linux driver was designed to handle the sonar used in the proximity radar
 * system developed by Guillaume Garcia and Alexandre V. Jamet during the embedded
 * week at [TELECOM Nancy](http://telecomnancy.univ-lorraine.fr).
 *
 * The sonar used by this driver to determine the distance to the surrounding
 * object is ... For more informations about this sonar, you can visit
 * [this web site](http://d.d).
 *
 * This driver gives the user a virtual file to easily read the measured distance
 * to the surrounding objects. This virtual file is simply called `/dev/sonar`.
 * You can read the first 8 bytes of this file to get the measured distance to
 * the surrounding objects in millimeters. Here's a simple example of how to use
 * this virtual file to read the informations straight out of the sonar:
 *
 * ```sh
 * dd if=/dev/sonar out=result.bin bs=8 count=1
 * ```
 */

/**
 * @defgroup Proximity_Radar_Driver Proximity radar's driver
 *
 * Functions to initialize and free the proximity radar's driver.
 *
 * @{
 */

/**
 * @brief Initialize the driver that handles the sonar
 * and uses it as a proximity radar.
 */
static int __init sonar_driver_init(void) {
    printk("Starting ProximitySonarDriver! So long! <3\n");

    if (pinout_init() != 0x0) {
        return -1;
    }

    if (interrupt_init() != 0x0) {
        pinout_free();

        return -1;
    }

    if (sonar_device_init() != 0x0) {
        interrupt_free();
        pinout_free();

        return -1;
    }

    return 0;
}

/**
 * @brief Release the driver that handles the sonar and
 * free all used resources.
 */
static void __exit sonar_driver_exit(void) {
    printk("Exiting ProximitySonarDriver ! <3\n");
    pinout_free();
    interrupt_free();
    sonar_device_free();

    free_sonar_inst();
}

/**
 * @}
 */

// Exporting all required symbols of the driver.
module_init(sonar_driver_init);
module_exit(sonar_driver_exit);

// Exporting the driver's meta data.
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Guillaume Garcia & Alexandre V. Jamet");
MODULE_DESCRIPTION("Driver to command a sonar used as a proximity radar.");
