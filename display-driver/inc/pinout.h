#ifndef __PINOUT_H__
#define __PINOUT_H__

#define PIN_13      0x4       /*!< Alias on PIN 13. */
#define PIN_14      0x5       /*!< Alias on PIN 14. */
#define PIN_15      0x6       /*!< Alias on PIN 15. */
#define PIN_16      0x7       /*!< Alias on PIN 16. */
#define PIN_17      0x10      /*!< Alias on PIN 17. */
#define PIN_29      0x17      /*!< Alias on PIN 29. */
#define PIN_28      0x19      /*!< Alias on PIN 28. */
#define PIN_30      0x5b      /*!< Alias on PIN 30. */
#define PIN_24      0x34      /*!< Alias on PIN 24. */
#define PIN_26      0x35      /*!< Alias on PIN 26. */

#define SEGMENT_A       PIN_13  /*!< Segment A */
#define SEGMENT_B       PIN_14  /*!< Segment B */
#define SEGMENT_C       PIN_15  /*!< Segment C */
#define SEGMENT_D       PIN_16  /*!< Segment D */
#define SEGMENT_E       PIN_17  /*!< Segment E */
#define SEGMENT_F       PIN_29  /*!< Segment F */
#define SEGMENT_G       PIN_28  /*!< Segment G */
#define LEFT_DIGIT      PIN_26  /*!< Left digit controller */
#define MIDDLE_DIGIT    PIN_24  /*!< Middle digit controller */
#define RIGHT_DIGIT     PIN_30  /*!< Right digit controller */

#define VALUE_OFF   0x0     /*!< Alias on OFF state of a pin. */
#define VALUE_ON    0x1     /*!< Alias on ON state of a pin. */

int pinout_init(void);
void pinout_free(void);

void reset_gpios(void);

#endif // __PINOUT_H__
