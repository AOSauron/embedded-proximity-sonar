#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/ktime.h>
#include <linux/timer.h>
#
#include "../inc/pinout.h"
#include "../inc/display.h"

// ========================================================================= //
//                      INTERFACE FUNCTION WITH USER SPACE                   //
// ========================================================================= //
static int              display_device_major;
static struct device*   display_device;
static struct class*    display_device_class;
static dev_t            display_devt;

static ssize_t display_device_write(struct file *file, const char __user *data, size_t size, loff_t *loff);

static struct file_operations display_device_fops = {
    .read = NULL,
    .write = &display_device_write,
    .open = NULL,
    .release = NULL
};

static ssize_t display_device_write(struct file *file, const char __user *data, size_t size, loff_t *loff) {
    char *buf = kmalloc(size, GFP_KERNEL);

    if (copy_from_user(buf, data, 8) > 0) {
        return -1;
    }

    get_display_inst()->displayed_number = ((uint64_t) *buf);

    kfree(buf);

    return size;
}
// ========================================================================= //
//                             KERNEL SPACE FUNCTIONS                        //
// ========================================================================= //
#define INTERVAL 1e6

static struct display* display_inst = NULL;
static struct hrtimer display_timer;
static char current_digit = 0;

/**
 * @brief Array of function pointers to the different functions used to
 * display a number on a 7-segment display.
 */
static void (*display_helper []) (const struct display *) = {
    &display_0,
    &display_1,
    &display_2,
    &display_3,
    &display_4,
    &display_5,
    &display_6,
    &display_7,
    &display_8,
    &display_9
};

/**
 * @brief This function is a callback used to update the displayer with
 * the appropriate number.
 */
static enum hrtimer_restart display_timer_callback(struct hrtimer *arg) {
    ktime_t curr_time = ktime_get(), interval = ktime_set(0, INTERVAL);
    uint8_t unit, decade, hundred;
    uint16_t cpy = (uint16_t) (display_inst->displayed_number & 0xFFFFFFFF);

    // Compute the different digit to display.
    hundred = cpy / 100;
    decade = (cpy - hundred * 100) / 10;
    unit = (cpy - hundred * 100 - decade * 10);

    // Reset the display.
    display_blank(display_inst);

    // Display the appropriate digit at the appropriate place.
    switch (current_digit) {
        case 0:
            display_digit_1(display_inst);
            display_helper[hundred](display_inst);
            break;
        case 1:
            display_digit_2(display_inst);
            display_helper[decade](display_inst);
            break;
        case 2:
            display_digit_3(display_inst);
            display_helper[unit](display_inst);
            break;
        default:
            break;
    }

    // Moving to the next digit and back to the first... :D
    current_digit = (current_digit + 1) % 3;

    hrtimer_forward(arg, curr_time, interval);

    return HRTIMER_RESTART;
}

/**
 * @brief Get the unique instance of the display handle.
 */
struct display* get_display_inst(void) {
    if (display_inst == NULL) {
        ktime_t interval;

        display_inst = kmalloc(sizeof(struct display), GFP_KERNEL);

        // PIN numbers affectation
        display_inst->digit1_pin = LEFT_DIGIT;
        display_inst->digit2_pin = MIDDLE_DIGIT;
        display_inst->digit3_pin = RIGHT_DIGIT;

        display_inst->a_segment_pin = SEGMENT_A;
        display_inst->b_segment_pin = SEGMENT_B;
        display_inst->c_segment_pin = SEGMENT_C;
        display_inst->d_segment_pin = SEGMENT_D;
        display_inst->e_segment_pin = SEGMENT_E;
        display_inst->f_segment_pin = SEGMENT_F;
        display_inst->g_segment_pin = SEGMENT_G;

        // Initialize the displayed number.
        display_inst->displayed_number = 111;

        /*
         * Initializing the display timer.
         *
         * The timer must refresh the entire display 20 times per
         * second. The display has 3 digit so the timer must
         * hit 3 * 20 = 60 times per seconds. Then the interval must
         * be around 1/60 s = 16.666 ms. Let's take 15 ms. :D
         */
        interval = ktime_set(0, INTERVAL);

        hrtimer_init(&display_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
        display_timer.function = &display_timer_callback;

        hrtimer_start(&display_timer, interval, HRTIMER_MODE_REL);
    }

    return display_inst;
}

/**
 * @brief Free the unique instance of the display handle.
 */
void free_display_inst(void) {
    if (display_inst != NULL) {
        hrtimer_cancel(&display_timer);

        kfree(display_inst);
    }
}

/**
 * Initialize the virtual file associated to the sonar device to create an
 * interface in the user space.
 */
int display_device_init(void) {
    // Getting a major for the peripheral.
    if ((display_device_major = register_chrdev(0, "display", &display_device_fops)) <= 0) {
        printk(KERN_ERR"(1) Error while initializing the peripheral... :(\n");

        return -1;
    }

    // Creating a class for the device and so on...
    display_device_class = class_create(THIS_MODULE, "display");
    if (!display_device_class) {
        printk(KERN_ERR"(2) Error while initializing the peripheral... :(\n");

        unregister_chrdev(display_device_major, "display");

        return -1;
    }

    display_devt = MKDEV(display_device_major, 0);
    display_device = device_create(display_device_class, NULL, display_devt, NULL, "display");

    return 0;
}

/**
 * @brief Free the virtual file associated to the sonar device.
 */
void display_device_free(void) {
    device_destroy(display_device_class, display_devt);
    class_destroy(display_device_class);
    unregister_chrdev(display_device_major, "display");
}

///////////////////////////////////
//  Displaying functions         //
//                               //
//  Pattern of 7-segments:       //
//          a                    //
//           _                   //
//      | f  _ g  | b            //
//      | e  _    | c            //
//          d                    //
///////////////////////////////////

// Display a '0' on a 7-segment
void display_0(const struct display *inst)
{
    /*  _
     * | |
     * |_|
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->e_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->g_segment_pin, VALUE_OFF);
}

// Display a '1' on a 7-segment
void display_1(const struct display *inst)
{
     /*
      *  |
      *  |
      *
      */

    // LED on
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);

    //LED off
    gpio_set_value(inst->a_segment_pin, VALUE_OFF);
    gpio_set_value(inst->d_segment_pin, VALUE_OFF);
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);
    gpio_set_value(inst->f_segment_pin, VALUE_OFF);
    gpio_set_value(inst->g_segment_pin, VALUE_OFF);
}

// Display a '2' on a 7-segment
void display_2(const struct display *inst)
{
    /*  _
     *  _|
     * |_
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->e_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->c_segment_pin, VALUE_OFF);
    gpio_set_value(inst->f_segment_pin, VALUE_OFF);
}

// Display a '3' on a 7-segment
void display_3(const struct display *inst)
{
    /*  _
     *  _|
     *  _|
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);
    gpio_set_value(inst->f_segment_pin, VALUE_OFF);
}

// Display a '4' on a 7-segment
void display_4(const struct display *inst)
{
    /*
     * |_|
     *   |
     *
     */

    // LED on
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->a_segment_pin, VALUE_OFF);
    gpio_set_value(inst->d_segment_pin, VALUE_OFF);
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);

}

// Display a '5' on a 7-segment
void display_5(const struct display *inst)
{
    /*  _
     * |_
     *  _|
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->b_segment_pin, VALUE_OFF);
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);

}

// Display a '6' on a 7-segment
void display_6(const struct display *inst)
{
    /*  _
     * |_
     * |_|
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->e_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->b_segment_pin, VALUE_OFF);
}

// Display a '7' on a 7-segment
void display_7(const struct display *inst)
{
    /*  _
     *   |
     *   |
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->d_segment_pin, VALUE_OFF);
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);
    gpio_set_value(inst->f_segment_pin, VALUE_OFF);
    gpio_set_value(inst->g_segment_pin, VALUE_OFF);
}

// Display a '8' on a 7-segment
void display_8(const struct display *inst)
{
    /*  _
     * |_|
     * |_|
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->e_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    // None !
}

// Display a '9' on a 7-segment
void display_9(const struct display *inst)
{
    /*  _
     * |_|
     *  _|
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->b_segment_pin, VALUE_ON);
    gpio_set_value(inst->c_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    //LED off
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);
}


// Display a 'E' on a 7-segment
void display_E(const struct display *inst)
{
    /*  _
     * |_
     * |_
     *
     */

    // LED on
    gpio_set_value(inst->a_segment_pin, VALUE_ON);
    gpio_set_value(inst->d_segment_pin, VALUE_ON);
    gpio_set_value(inst->e_segment_pin, VALUE_ON);
    gpio_set_value(inst->f_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->b_segment_pin, VALUE_OFF);
    gpio_set_value(inst->c_segment_pin, VALUE_OFF);
}

// Display a 'r' on a 7-segment
void display_r(const struct display *inst)
{
    /*
     *  _
     * |
     *
     */

    // LED on
    gpio_set_value(inst->e_segment_pin, VALUE_ON);
    gpio_set_value(inst->g_segment_pin, VALUE_ON);

    // LED off
    gpio_set_value(inst->a_segment_pin, VALUE_OFF);
    gpio_set_value(inst->b_segment_pin, VALUE_OFF);
    gpio_set_value(inst->c_segment_pin, VALUE_OFF);
    gpio_set_value(inst->d_segment_pin, VALUE_OFF);
    gpio_set_value(inst->f_segment_pin, VALUE_OFF);

}


// Display a blank (nothing)
void display_blank(const struct display *inst)
{
    /*
     *
     *
     *
     */

    // LED on
    // None !

    // LED off
    gpio_set_value(inst->a_segment_pin, VALUE_OFF);
    gpio_set_value(inst->b_segment_pin, VALUE_OFF);
    gpio_set_value(inst->c_segment_pin, VALUE_OFF);
    gpio_set_value(inst->d_segment_pin, VALUE_OFF);
    gpio_set_value(inst->e_segment_pin, VALUE_OFF);
    gpio_set_value(inst->f_segment_pin, VALUE_OFF);
    gpio_set_value(inst->g_segment_pin, VALUE_OFF);
}


// Init the digit 1 (Left) and disable the others
void display_digit_1(const struct display *inst)
{
    gpio_set_value(inst->digit1_pin, VALUE_ON);
    gpio_set_value(inst->digit2_pin, VALUE_OFF);
    gpio_set_value(inst->digit3_pin, VALUE_OFF);
}

// Init the digit 2 (Middle) and disable the others
void display_digit_2(const struct display *inst)
{
    gpio_set_value(inst->digit1_pin, VALUE_OFF);
    gpio_set_value(inst->digit2_pin, VALUE_ON);
    gpio_set_value(inst->digit3_pin, VALUE_OFF);
}

// Init the digit 3 (Right) and disable the others
void display_digit_3(const struct display *inst)
{
    gpio_set_value(inst->digit1_pin, VALUE_OFF);
    gpio_set_value(inst->digit2_pin, VALUE_OFF);
    gpio_set_value(inst->digit3_pin, VALUE_ON);
}
