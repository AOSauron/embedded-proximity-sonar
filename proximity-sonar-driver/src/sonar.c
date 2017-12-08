#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#
#include "../inc/pinout.h"
#include "../inc/sonar.h"

// ========================================================================= //
//                      INTERFACE FUNCTION WITH USER SPACE                   //
// ========================================================================= //
static int              sonar_device_major;     /*!< Major of the exposed device. */
static struct device*   sonar_device;           /*!< Handle of the exposed device. */
static struct class*    sonar_device_class;     /*!< Class of the exposed device. */

static dev_t            sonar_devt;             /*!< The exposed device intendifier. */

static ssize_t sonar_device_read(struct file *file, char __user *data, size_t size, loff_t *loff);

/**
 * @brief All required functions to interact with the driver from the user space.
 *
 * The only required action with this driver is to read the distance from the
 * sonar.
 */
static struct file_operations sonar_device_fops = {
    .read = &sonar_device_read,
    .write = NULL,
    .open = NULL,
    .release = NULL
};

/**
 * @brief Read data from the sonar device and return it to the user space.
 *
 * @param file
 * @param data The user space buffer that should be field.
 * @param size The size if the requested data.
 * @param loff
 *
 * @return The amount of data read from the device.
 */
static ssize_t sonar_device_read(struct file *file, char __user *data, size_t size, loff_t *loff) {
    char *buf;
    uint64_t distance;

    // Allocating memory in the kernel space.
    buf = kmalloc(size, GFP_KERNEL);

    // Getting the distance from the sonar...
    sonar_measure_distance(get_sonar_inst());
    distance = get_sonar_inst()->measured_distance;

    memset(buf, 0x0, sizeof(uint64_t));
    memcpy(buf, &distance, sizeof(uint64_t));

    // Handling errors...
    if (copy_to_user(data, buf, size)) {
        return -1;
    }

    // Freeing the kernel space allocated memory.
    kfree(buf);

    return size;
}

// ========================================================================= //
//                             KERNEL SPACE FUNCTIONS                        //
// ========================================================================= //
static struct sonar* sonar_inst = NULL; /*<! The only allowed instance of the sonar device. */

/**
 * @brief Get the unique instance of the sonar device.
 *
 * If the the instance doesn't exist yet, it is
 * created. You should not free the memory allocated
 * to the sonar device yourself, you have to use the
 * free_sonar_inst() instead.
 *
 * @return The unique instance of the sonar device.
 */
struct sonar* get_sonar_inst(void) {
    // If the instance has not been allocated yet.
    if (sonar_inst == NULL) {
        sonar_inst = kmalloc(sizeof(struct sonar), GFP_KERNEL);

        sonar_inst->trigger_pin = SONAR_TRIG;
        sonar_inst->echo_pin = SONAR_ECHO;
    }

    // Whatever happens above, the unique instance is returned.
    return sonar_inst;
}

/**
 * @brief Free the memory allocated to the unique sonar device instance by
 * the get_sonar_inst() function.
 *
 * This function must not be called if get_sonar_inst() has not been
 * called at least once.
 */
void free_sonar_inst(void) {
    if (sonar_inst != NULL) {
        kfree(sonar_inst);
    }
}

/**
 * @brief Measure the distance with the sonar device.
 *
 * The measured distance is stored as an 8-bytes integer.
 *
 * @param inst The unique instance of the sonar device that should be filled with
 * the measure distance.
 */
void sonar_measure_distance(struct sonar *inst) {
    // Send a trigger on the sonar trigger pin that lasts at least 10 us.
    gpio_set_value(inst->trigger_pin, VALUE_ON);

    udelay(15);

    gpio_set_value(inst->trigger_pin, VALUE_OFF);

    /*
     * Waiting for the associated interruption to fill the sonar instance...
     *
     * 50 ms is enough to detect an object 8 meters way, twice the limit of
     * the sonar... :D
     *
     * Wa could have done better to handle the interruption but we didn't have
     * time for this.
     */
    mdelay(50);

    return;
}

/**
 * @brief Initialize the sonar device, it is usable from the user space through
 * the /dev/sonar path.
 *
 * The function returns 0 on success and -1 otherwise.
 *
 * @return Status code.
 */
int sonar_device_init(void) {
    // Getting a major for the peripheral.
    if ((sonar_device_major = register_chrdev(0, "sonar", &sonar_device_fops)) <= 0) {
        printk(KERN_ERR"(1) Error while initializing the peripheral... :(\n");

        return -1;
    }

    // Creating a class for the device and so on...
    sonar_device_class = class_create(THIS_MODULE, "sonar");

    // Managing error if something wring happens.
    if (!sonar_device_class) {
        printk(KERN_ERR"(2) Error while initializing the peripheral... :(\n");

        unregister_chrdev(sonar_device_major, "sonar");

        return -1;
    }

    // Finally creating the device.
    sonar_devt = MKDEV(sonar_device_major, 0);
    sonar_device = device_create(sonar_device_class, NULL, sonar_devt, NULL, "sonar");

    return 0;
}

/**
 * @brief Free the sonar device initialized by the sonar_device_init function.
 */
void sonar_device_free(void) {
    device_destroy(sonar_device_class, sonar_devt);
    class_destroy(sonar_device_class);
    unregister_chrdev(sonar_device_major, "sonar");
}
