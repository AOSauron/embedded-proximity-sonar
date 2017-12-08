#ifndef __SONAR_H__
#define __SONAR_H__

/**
 * @defgroup Proximity_Radar_Driver_Sonar Sonar
 *
 * @{
 */

/**
 * @defgroup Proximity_Radar_Driver_Sonar_Structures Sonar's structures
 *
 * Structures to handle the sonar of the proximity radar.
 *
 * @{
 */

/**
 * @brief A structure to represent the sonar and its internal state.
 */
struct sonar {
    unsigned int trigger_pin;   /*!< The PIN used to trigger a measure on the sonar device.*/
    unsigned int echo_pin;      /*!< The PIN used by the sonar device to give the response back. */

    uint64_t measured_distance; /*!< The last measured distance by the sonar device. */
};

/**
 * @}
 */

/**
 * @defgroup Proximity_Radar_Driver_Sonar_Functions Sonar's functions
 *
 * Functions to handle the sonar of the proxmity radar.
 *
 * @{
 */

struct sonar* get_sonar_inst(void);
void free_sonar_inst(void);

void sonar_measure_distance(struct sonar *inst);

int sonar_device_init(void);
void sonar_device_free(void);

/**
 * @}
 */

/**
 * @}
 */

#endif // __SONAR_H__
