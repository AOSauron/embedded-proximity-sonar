#ifndef __PINOUT_H__
#define __PINOUT_H__

/**
 * @defgroup Proximity_Radar_Driver_Pinout Pinout
 *
 * This module contains all the tools required to handle the periphal pinout.
 *
 * @{
 */

/**
 * @defgroup Proximity_Radar_Driver_Pinout_Defines Defines of the PINs
 *
 * Defines to handle the input/output of the proximity radar's driver.
 *
 * @{
 */
#define PIN_10      0x1     /*!< Alias on PIN 10. */
#define PIN_11      0x2     /*!< Alias on PIN 11. */
//#define PIN_13      0x4     /*!< Alias on PIN 13. */ WARNING : USED BY DISPLAYDRIVER

//#define LED         PIN_13  /*!< Alias on the status LED. Not used anymore. */
#define SONAR_TRIG  PIN_10  /*!< Alias on the sonar trigger (input). */
#define SONAR_ECHO  PIN_11  /*!< Alias on the sonar echo (output). */

#define VALUE_OFF   0x0     /*!< Alias on OFF state of a pin. */
#define VALUE_ON    0x1     /*!< Alias on ON state of a pin. */-
/**
 * @{
 */

/**
 * @defgroup Proximity_Radar_Driver_Pinout_Functions Functions of the PINs
 *
 * Functions to handle the input/output of the proximity radar's driver.
 * @{
 */
int pinout_init(void);
void pinout_free(void);
/**
 * @}
 */

/**
 * @}
 */

#endif // __PINOUT_H__
