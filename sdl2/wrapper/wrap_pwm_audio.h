#ifndef _wrap_pwm_audio_h
#define _wrap_pwm_audio_h

#include <stdint.h>
#include <stdlib.h>

typedef uint32_t esp_err_t;

typedef enum {
    TIMER_GROUP_0 = 0, /*!<Hw timer group 0*/
#if SOC_TIMER_GROUPS > 1
    TIMER_GROUP_1 = 1, /*!<Hw timer group 1*/
#endif
    TIMER_GROUP_MAX,
} timer_group_t;

/**
 * @brief Select a hardware timer from timer groups
 */
typedef enum {
    TIMER_0 = 0, /*!<Select timer0 of GROUPx*/
#if SOC_TIMER_GROUP_TIMERS_PER_GROUP > 1
    TIMER_1 = 1, /*!<Select timer1 of GROUPx*/
#endif
    TIMER_MAX,
} timer_idx_t;

typedef enum {
    LEDC_TIMER_0 = 0, /*!< LEDC timer 0 */
    LEDC_TIMER_1,     /*!< LEDC timer 1 */
    LEDC_TIMER_2,     /*!< LEDC timer 2 */
    LEDC_TIMER_3,     /*!< LEDC timer 3 */
    LEDC_TIMER_MAX,
} ledc_timer_t;

typedef enum {
    LEDC_CHANNEL_0 = 0, /*!< LEDC channel 0 */
    LEDC_CHANNEL_1,     /*!< LEDC channel 1 */
    LEDC_CHANNEL_2,     /*!< LEDC channel 2 */
    LEDC_CHANNEL_3,     /*!< LEDC channel 3 */
    LEDC_CHANNEL_4,     /*!< LEDC channel 4 */
    LEDC_CHANNEL_5,     /*!< LEDC channel 5 */
#if SOC_LEDC_CHANNEL_NUM > 6
    LEDC_CHANNEL_6,     /*!< LEDC channel 6 */
    LEDC_CHANNEL_7,     /*!< LEDC channel 7 */
#endif
    LEDC_CHANNEL_MAX,
} ledc_channel_t;

typedef enum {
    LEDC_TIMER_1_BIT = 1,   /*!< LEDC PWM duty resolution of  1 bits */
    LEDC_TIMER_2_BIT,       /*!< LEDC PWM duty resolution of  2 bits */
    LEDC_TIMER_3_BIT,       /*!< LEDC PWM duty resolution of  3 bits */
    LEDC_TIMER_4_BIT,       /*!< LEDC PWM duty resolution of  4 bits */
    LEDC_TIMER_5_BIT,       /*!< LEDC PWM duty resolution of  5 bits */
    LEDC_TIMER_6_BIT,       /*!< LEDC PWM duty resolution of  6 bits */
    LEDC_TIMER_7_BIT,       /*!< LEDC PWM duty resolution of  7 bits */
    LEDC_TIMER_8_BIT,       /*!< LEDC PWM duty resolution of  8 bits */
    LEDC_TIMER_9_BIT,       /*!< LEDC PWM duty resolution of  9 bits */
    LEDC_TIMER_10_BIT,      /*!< LEDC PWM duty resolution of 10 bits */
    LEDC_TIMER_11_BIT,      /*!< LEDC PWM duty resolution of 11 bits */
    LEDC_TIMER_12_BIT,      /*!< LEDC PWM duty resolution of 12 bits */
    LEDC_TIMER_13_BIT,      /*!< LEDC PWM duty resolution of 13 bits */
    LEDC_TIMER_14_BIT,      /*!< LEDC PWM duty resolution of 14 bits */
#if SOC_LEDC_TIMER_BIT_WIDE_NUM > 14
    LEDC_TIMER_15_BIT,      /*!< LEDC PWM duty resolution of 15 bits */
    LEDC_TIMER_16_BIT,      /*!< LEDC PWM duty resolution of 16 bits */
    LEDC_TIMER_17_BIT,      /*!< LEDC PWM duty resolution of 17 bits */
    LEDC_TIMER_18_BIT,      /*!< LEDC PWM duty resolution of 18 bits */
    LEDC_TIMER_19_BIT,      /*!< LEDC PWM duty resolution of 19 bits */
    LEDC_TIMER_20_BIT,      /*!< LEDC PWM duty resolution of 20 bits */
#endif
    LEDC_TIMER_BIT_MAX,
} ledc_timer_bit_t;



typedef struct
{
    timer_group_t tg_num;                /*!< timer group number (0 - 1) */
    timer_idx_t timer_num;               /*!< timer number  (0 - 1) */
    int gpio_num_left;                   /*!< the LEDC output gpio_num, Left channel */
    int gpio_num_right;                  /*!< the LEDC output gpio_num, Right channel */
    ledc_channel_t ledc_channel_left;    /*!< LEDC channel (0 - 7), Corresponding to left channel*/
    ledc_channel_t ledc_channel_right;   /*!< LEDC channel (0 - 7), Corresponding to right channel*/
    ledc_timer_t ledc_timer_sel;         /*!< Select the timer source of channel (0 - 3) */
    ledc_timer_bit_t duty_resolution;    /*!< ledc pwm bits */
    uint32_t ringbuf_len;                /*!< ringbuffer size */
} pwm_audio_config_t;

typedef enum {
    PWM_AUDIO_STATUS_UN_INIT = 0,        /*!< pwm audio uninitialized */
    PWM_AUDIO_STATUS_IDLE = 1,           /*!< pwm audio idle */
    PWM_AUDIO_STATUS_BUSY = 2,           /*!< pwm audio busy */
} pwm_audio_status_t;

typedef enum {
    PWM_AUDIO_CH_MONO        = 0,        /*!< 1 channel (mono)*/
    PWM_AUDIO_CH_STEREO      = 1,        /*!< 2 channel (stereo)*/
    PWM_AUDIO_CH_MAX,
} pwm_audio_channel_t;

inline esp_err_t pwm_audio_init(const pwm_audio_config_t *cfg)
{
    return 0;
}

inline esp_err_t pwm_audio_deinit(void)
{
    return 0;
}

inline esp_err_t pwm_audio_start(void)
{
    return 0;
}

inline esp_err_t pwm_audio_stop(void)
{
    return 0;
}

esp_err_t pwm_audio_write(uint8_t* inbuf, size_t len, size_t* bytes_written, TickType_t ticks_to_wait);
esp_err_t pwm_audio_set_param(int rate, ledc_timer_bit_t bits, int ch);

//inline esp_err_t pwm_audio_write(uint8_t *inbuf, size_t len, size_t *bytes_written, TickType_t ticks_to_wait)
//{
//    return 0;
//}

//inline esp_err_t pwm_audio_set_param(int rate, ledc_timer_bit_t bits, int ch)
//{
//    return 0;
//}

inline esp_err_t pwm_audio_set_sample_rate(int rate)
{
    return 0;
}

inline esp_err_t pwm_audio_set_volume(int8_t volume)
{
    return 0;
}

inline esp_err_t pwm_audio_get_volume(int8_t *volume)
{
    return 0;
}

inline esp_err_t pwm_audio_get_param(int *rate, int *bits, int *ch)
{
    return 0;
}

inline esp_err_t pwm_audio_get_status(pwm_audio_status_t *status)
{
    return 0;
}




#endif