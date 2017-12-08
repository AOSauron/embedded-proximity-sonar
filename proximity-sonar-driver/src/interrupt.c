#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#
#include <linux/math64.h>
#
#include "../inc/pinout.h"
#include "../inc/interrupt.h"
#include "../inc/sonar.h"

static int sonar_echo_interrupt_id = 0x0;   /*!< The identifier of the sonar echo output signal. */
static uint64_t rising_time = 0x0;          /*!< Time of the last rising edge of the sonar echo output signal. */
static uint64_t elapsed_time = 0x0;         /*!< Elapsed time between rising and falling edge. */

/**
 * @brief The interruption binded to the SONAR_EHO PIN.
 *
 * @param irq The interruption identifier.
 * @param data Some data... :|
 *
 * @return Status code.
 */
static irqreturn_t sonar_echo_interrupt_handler(int irq, void *data) {
    int value = gpio_get_value(SONAR_ECHO);

    switch (value) {
        // When the interrupt is triggered on a rising edge.
        case VALUE_OFF:
            elapsed_time = ktime_to_us(ktime_get()) - rising_time;
            get_sonar_inst()->measured_distance = div64_ul(17 * elapsed_time, 100);
            break;

            // When  the interrupt is triggered on a falling edge.
        case VALUE_ON:
            rising_time = ktime_to_us(ktime_get());
            break;

        default:
            break;
    }

    // The interrupt is always handled.
    return IRQ_HANDLED;
}

/**
 * @brief Initialize all required interruptions.
 */
int interrupt_init(void) {
    unsigned long sonar_echo_interrupt_flags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;

    if ((sonar_echo_interrupt_id = gpio_to_irq(SONAR_ECHO)) == 0) {
        printk(KERN_ERR"Error while initializing an interruption... :(");

        return -1;
    }

    if (request_irq(sonar_echo_interrupt_id, &sonar_echo_interrupt_handler, sonar_echo_interrupt_flags, "LOL", NULL) != 0x0) {
        printk(KERN_ERR"Error while requesting an interruption... :(");

        return -1;
    }

    return 0;
}

/**
 * @brief Free all interruptions previously initialized.
 */
int interrupt_free(void) {
    // Freeing all reserved interruptions...
    free_irq(sonar_echo_interrupt_id, NULL);

    return 0;
}
