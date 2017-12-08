#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#
#include "../inc/pinout.h"

/**
 * @brief List of GPIO PIN initialization with parameters.
 */
static struct gpio init_gpios[] = {
    {SONAR_TRIG, GPIOF_OUT_INIT_LOW, "Sonar Input"},    /* Sonar input, default to OFF. */
    {SONAR_ECHO, GPIOF_IN, "Sonar Output"},             /* Sonar output. */
};

/**
 * @brief Initialize all required GPIO PINs and set the them
 * all.
 *
 * Returns 0 on success and -1 otherwise.
 *
 * @return Status code.
 */
int pinout_init(void) {
    int err_code;

    // Initializing the GPIOs...
    err_code = gpio_request_array(init_gpios, ARRAY_SIZE(init_gpios));
    
    /*
     * Managing errors GPIOs request...
     *
     * If an error occurs while requesting the GPIOs, the driver
     * get the kernel back to its initial state by releasing the
     * GPIOs.
     */
    if (err_code != 0x0) {
        printk(KERN_ERR"Error while requesting GPIOs... :(");
        
        gpio_free_array(init_gpios, ARRAY_SIZE(init_gpios));

        return -1;
    }

    // Setting debounce on SONAR_ECHO
    gpio_set_debounce(SONAR_ECHO, 100);

    // Setting output pin values...
    gpio_set_value(LED, VALUE_OFF);
    gpio_set_value(SONAR_TRIG, VALUE_OFF);

    return 0;
}

/**
 * @brief Releases all reserved GPIO PINs and reset the state
 * of all output PINs.
 */
void pinout_free(void) {
    // Initial state reset of all output GPIO PINs.
    gpio_set_value(LED, VALUE_OFF);
    gpio_set_value(SONAR_TRIG, VALUE_OFF);

    // Releasing reserved GPIOs...
    gpio_free_array(init_gpios, ARRAY_SIZE(init_gpios));
}
