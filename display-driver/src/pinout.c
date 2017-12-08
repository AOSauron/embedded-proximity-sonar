#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#
#include "../inc/pinout.h"

 /**
  * @brief List of GPIO PIN initialization with parameters.
  */
 static struct gpio init_gpios[] = {
    {LEFT_DIGIT, GPIOF_OUT_INIT_LOW, "Left digit"},        /* Left digit, default to OFF. */
    {MIDDLE_DIGIT, GPIOF_OUT_INIT_LOW, "Middle digit"},    /* Middle digit, default to OFF. */
    {RIGHT_DIGIT, GPIOF_OUT_INIT_LOW, "Right digit"},      /* Right digit, default to OFF. */
    {SEGMENT_A, GPIOF_OUT_INIT_LOW, "Segment A"},          /* Segment A, default to OFF. */
    {SEGMENT_B, GPIOF_OUT_INIT_LOW, "Segment B"},          /* Segment B, default to OFF. */
    {SEGMENT_C, GPIOF_OUT_INIT_LOW, "Segment C"},          /* Segment C, default to OFF. */
    {SEGMENT_D, GPIOF_OUT_INIT_LOW, "Segment D"},          /* Segment D, default to OFF. */
    {SEGMENT_E, GPIOF_OUT_INIT_LOW, "Segment E"},          /* Segment E, default to OFF. */
    {SEGMENT_F, GPIOF_OUT_INIT_LOW, "Segment F"},          /* Segment F, default to OFF. */
    {SEGMENT_G, GPIOF_OUT_INIT_LOW, "Segment G"},          /* Segment G, default to OFF. */
};

/**
 * @brief Initialize all required GPIO PINs and set the them
 * all.
 *
 * @return Status code.
 */
int pinout_init(void) {
    int err_code;

    // Initializing the GPIOs...
    err_code = gpio_request_array(init_gpios, ARRAY_SIZE(init_gpios));

    if (err_code != 0x0) {
        printk(KERN_ERR"Error while requesting GPIOs... :(");

        gpio_free_array(init_gpios, ARRAY_SIZE(init_gpios));

        return -1;
    }

    // Ensure that all pins are at 0
    reset_gpios();

    return 0;
}

/**
 * @brief Releases all reserved GPIO PINs and reset the state
 * of all output PINs.
 */
void pinout_free(void) {
    // Initial state reset of all output GPIO PINs.
    reset_gpios();

    // Releasing reserved GPIOs...
    gpio_free_array(init_gpios, ARRAY_SIZE(init_gpios));
}


/*
 * @brief Reset the GPIOs to initial state (VALUE_OFF)
 */
void reset_gpios()
{
    int index;
    for (index = 0; index < ARRAY_SIZE(init_gpios); index++) {
        gpio_set_value(init_gpios[index].gpio, VALUE_OFF);
    }
}
