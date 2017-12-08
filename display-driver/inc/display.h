#ifndef __DISPLAY_H__
#define __DISPLAY_H__

struct display {
    unsigned int digit1_pin;        /*!< PIN to handle the first digit. */
    unsigned int digit2_pin;        /*!< PIN to handle the second digit. */
    unsigned int digit3_pin;        /*!< PIN to handle the third digit. */

    unsigned int a_segment_pin;     /*!< PIN to handle the A segment of all digits. */
    unsigned int b_segment_pin;     /*!< PIN to handle the B segment of all digits. */
    unsigned int c_segment_pin;     /*!< PIN to handle the C segment of all digits. */
    unsigned int d_segment_pin;     /*!< PIN to handle the D segment of all digits. */
    unsigned int e_segment_pin;     /*!< PIN to handle the E segment of all digits. */
    unsigned int f_segment_pin;     /*!< PIN to handle the F segment of all digits. */
    unsigned int g_segment_pin;     /*!< PIN to handle the G segment of all digits. */

    uint64_t displayed_number;      /*!< The digits that should be displayed. This digit should
                                        be between 0 and 999. */
};

struct display* get_display_inst(void);
void free_display_inst(void);

int display_device_init(void);
void display_device_free(void);

void display_0(const struct display *inst);
void display_1(const struct display *inst);
void display_2(const struct display *inst);
void display_3(const struct display *inst);
void display_4(const struct display *inst);
void display_5(const struct display *inst);
void display_6(const struct display *inst);
void display_7(const struct display *inst);
void display_8(const struct display *inst);
void display_9(const struct display *inst);

void display_E(const struct display *inst);
void display_r(const struct display *inst);

void display_blank(const struct display *inst);

void display_digit_1(const struct display *inst);
void display_digit_2(const struct display *inst);
void display_digit_3(const struct display *inst);

#endif // __DISPLAY_H__
