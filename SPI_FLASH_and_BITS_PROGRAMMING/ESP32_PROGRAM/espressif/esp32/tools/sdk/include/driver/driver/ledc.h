// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _DRIVER_LEDC_H_
#define _DRIVER_LEDC_H_
#include "esp_err.h"
#include "soc/soc.h"
#include "soc/ledc_reg.h"
#include "soc/ledc_reg.h"
#include "soc/ledc_struct.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LEDC_APB_CLK_HZ (APB_CLK_FREQ)
#define LEDC_REF_CLK_HZ (1*1000000)

typedef enum {
    LEDC_HIGH_SPEED_MODE = 0, /*LEDC high speed speed_mode */
    //in this version, we only support high speed speed_mode. We will access low speed speed_mode later
    //LEDC_LOW_SPEED_MODE,    /*LEDC low speed speed_mode */
    LEDC_SPEED_MODE_MAX,
} ledc_mode_t;

typedef enum {
    LEDC_INTR_DISABLE = 0,    /*Disable LEDC interrupt */
    LEDC_INTR_FADE_END,       /*Enable LEDC interrupt */
} ledc_intr_type_t;

typedef enum {
    LEDC_DUTY_DIR_DECREASE = 0,    /*LEDC duty decrease direction */
    LEDC_DUTY_DIR_INCREASE = 1,    /*LEDC duty increase direction */
} ledc_duty_direction_t;

typedef enum  {
    LEDC_REF_TICK = 0, /*LEDC timer clock divided from reference tick(1Mhz) */
    LEDC_APB_CLK,      /*LEDC timer clock divided from APB clock(80Mhz)*/
} ledc_clk_src_t;

typedef enum {
    LEDC_TIMER_0 = 0, /*LEDC source timer TIMER0 */
    LEDC_TIMER_1,     /*LEDC source timer TIMER1 */
    LEDC_TIMER_2,     /*LEDC source timer TIMER2 */
    LEDC_TIMER_3,     /*LEDC source timer TIMER3 */
} ledc_timer_t;

typedef enum {
    LEDC_CHANNEL_0 = 0, /*LEDC channel 0 */
    LEDC_CHANNEL_1,     /*LEDC channel 1 */
    LEDC_CHANNEL_2,     /*LEDC channel 2 */
    LEDC_CHANNEL_3,     /*LEDC channel 3 */
    LEDC_CHANNEL_4,     /*LEDC channel 4 */
    LEDC_CHANNEL_5,     /*LEDC channel 5 */
    LEDC_CHANNEL_6,     /*LEDC channel 6 */
    LEDC_CHANNEL_7,     /*LEDC channel 7 */
} ledc_channel_t;

typedef enum {
    LEDC_TIMER_10_BIT = 10, /*LEDC PWM depth 10Bit */
    LEDC_TIMER_11_BIT = 11, /*LEDC PWM depth 11Bit */
    LEDC_TIMER_12_BIT = 12, /*LEDC PWM depth 12Bit */
    LEDC_TIMER_13_BIT = 13, /*LEDC PWM depth 13Bit */
    LEDC_TIMER_14_BIT = 14, /*LEDC PWM depth 14Bit */
    LEDC_TIMER_15_BIT = 15, /*LEDC PWM depth 15Bit */
} ledc_timer_bit_t;

typedef struct {
    int gpio_num;                   /*the LEDC output gpio_num, if you want to use gpio16, gpio_num = 16*/
    ledc_mode_t speed_mode;         /*LEDC speed speed_mode, high-speed mode or low-speed mode*/
    ledc_channel_t channel;         /*LEDC channel(0 - 7)*/
    ledc_intr_type_t intr_type;     /*configure interrupt, Fade interrupt enable  or Fade interrupt disable*/
    ledc_timer_t timer_sel;         /*Select the timer source of channel (0 - 3)*/
    uint32_t duty;                  /*LEDC channel duty, the duty range is [0, (2**bit_num) - 1], */
} ledc_channel_config_t;

typedef struct {
    ledc_mode_t speed_mode;         /*LEDC speed speed_mode, high-speed mode or low-speed mode*/
    ledc_timer_bit_t bit_num;       /*LEDC channel duty depth*/
    ledc_timer_t  timer_num;        /*The timer source of channel (0 - 3)*/
    uint32_t freq_hz;               /*LEDC timer frequency(Hz)*/
} ledc_timer_config_t;


/**
 * @brief      LEDC channel configuration
 *
 * User this Function, configure LEDC channel with the given channel/output gpio_num/interrupt/source timer/frequency(Hz)/LEDC depth
 *
 * @param[in]  ledc_channel_config_t
 *             ledc_channel_config_t.speed_mode     : LEDC speed speed_mode
 *             ledc_channel_config_t.gpio_num       : LEDC output gpio_num, if you want to use gpio16, ledc_channel_config_t.gpio_num = 16
 *             ledc_channel_config_t.channel        : LEDC channel(0 - 7)
 *             ledc_channel_config_t.intr_type      : configure interrupt, Fade interrupt enable  or Fade interrupt disable
 *             ledc_channel_config_t.timer_sel      : Select the timer source of channel (0 - 3), high speed channel must bind with high speed timer.
 *             ledc_channel_config_t.duty           : LEDC channel duty, the duty range is [0, (2**timer_bit_num) - 1],
 * @return     ESP_OK:  success
 *             ESP_ERR_INVALID_ARG: parameter error
 *
 */
esp_err_t ledc_channel_config(ledc_channel_config_t* ledc_conf);

/**
 * @brief      LEDC timer configuration
 *
 * User this Function, configure LEDC timer with the given source timer/frequency(Hz)/bit_num
 *
 * @param[in]  ledc_timer_config_t
 *             ledc_timer_config_t.speed_mode     : LEDC speed speed_mode
 *             ledc_timer_config_t.timer_num      : Select the timer source of channel (0 - 3)
 *             ledc_timer_config_t.freq_hz        : LEDC channel frequency(Hz),
 *             ledc_timer_config_t.bit_num        : LEDC channel duty depth
 * @return     ESP_OK:  success
 *             ESP_ERR_INVALID_ARG: parameter error
 *             ESP_FAIL: Can not find a proper pre-divider number base on the given frequency and the current bit_num.
 *
 */
esp_err_t ledc_timer_config(ledc_timer_config_t* timer_conf);

/**
 * @brief      LEDC update channel parameters
 *
 * Call this function to activate the LEDC updated parameters.
 * After ledc_set_duty, ledc_set_fade, we need to call this function to update the settings.
 *
 * @param[in]  speed_mode : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  channel    : LEDC channel(0-7), select from ledc_channel_t
 *
 * @return     ESP_OK:  success
 *             ESP_ERR_INVALID_ARG: parameter error
 *
 */
esp_err_t ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel);

/**
 * @brief   LEDC stop
 *
 * Disable LEDC output, and set idle level
 *
 * @param[in]  speed_mode   : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  channel      : LEDC channel(0-7), select from ledc_channel_t
 *
 * @return     ESP_OK:  success
 *             ESP_ERR_INVALID_ARG: parameter error
 */
esp_err_t ledc_stop(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t idle_level);

/**
 * @brief      LEDC set channel frequency(Hz)
 *
 * Set LEDC frequency(Hz)
 *
 * @param[in]  speed_mode  : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  timer_num   : LEDC timer index(0-3), select from ledc_timer_t
 *
 * @param[in]  freq_hz     : set the LEDC frequency
 *
 * @return     ESP_OK:  success
 *             ESP_ERR_INVALID_ARG: parameter error
 *             ESP_FAIL: Can not find a proper pre-divider number base on the given frequency and the current bit_num.
 */
esp_err_t ledc_set_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num, uint32_t freq_hz);

/**
 * @brief      LEDC get channel frequency(Hz)
 *
 * @param[in]  speed_mode  : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  timer_num   : LEDC timer index(0-3), select from ledc_timer_t
 *
 * @return    0      :  error
 *            others :  current LEDC frequency
 *
 */
uint32_t ledc_get_freq(ledc_mode_t speed_mode, ledc_timer_t timer_num);

/**
 * @brief      LEDC set duty
 *
 * Set LEDC duty, After the function calls the ledc_update_duty function, the function can take effect.
 *
 * @param[in]  speed_mode  : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  channel     : LEDC channel(0-7), select from ledc_channel_t
 *
 * @param[in]  duty        : set the LEDC duty, the duty range is [0, (2**bit_num) - 1]
 *
 * @return     ESP_OK:  success
 *             ESP_ERR_INVALID_ARG: parameter error
 */
esp_err_t ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty);

/**
 * @brief      LEDC get duty
 *
 * @param[in]  speed_mode    : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  channel : LEDC channel(0-7), select from ledc_channel_t
 *
 *
 * @return     -1: parameter error
 *             other value: current LEDC duty
 *
 */
int ledc_get_duty(ledc_mode_t speed_mode, ledc_channel_t channel);

/**
 * @brief      LEDC set gradient
 *
 * Set LEDC gradient, After the function calls the ledc_update_duty function, the function can take effect.
 *
 * @param[in]  speed_mode           : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  channel              : LEDC channel(0-7), select from ledc_channel_t
 *
 * @param[in]  duty                 : set the start of the gradient duty, the duty range is [0, (2**bit_num) - 1]
 *
 * @param[in]  gradule_direction    : set the direction of the gradient
 *
 * @param[in]  step_num             : set the number of the gradient
 *
 * @param[in]  duty_cyle_num        : set how many LEDC tick each time the gradient lasts
 *
 * @param[in]  duty_scale           : set gradient change amplitude
 *
 * @return     ESP_OK               : success
 *             ESP_ERR_INVALID_ARG  : parameter error
 */
esp_err_t ledc_set_fade(ledc_mode_t speed_mode, uint32_t channel, uint32_t duty, ledc_duty_direction_t gradule_direction,
                        uint32_t step_num, uint32_t duty_cyle_num, uint32_t duty_scale);

/**
 * @brief   register LEDC interrupt handler, the handler is an ISR.
 *          The handler will be attached to the same CPU core that this function is running on.
 *          Users should know that which CPU is running and then pick a INUM that is not used by system.
 *          We can find the information of INUM and interrupt level in soc.h.
 *          TODO: to move INUM options to menu_config
 * @param[in]   uint32_t ledc_intr_num    : LEDC interrupt number, check the info in soc.h, and please see the core-isa.h for more details
 * @param[in]   void (* fn)(void* )       : interrupt handler function.
 *                                          Note that the handler function MUST be defined with attribution of "IRAM_ATTR".
 * @param[in]   void * arg                : parameter for handler function
 *
 * @return      ESP_OK                    : success ;
 *              ESP_ERR_INVALID_ARG       : function ptr error.
 */
esp_err_t ledc_isr_register(uint32_t ledc_intr_num, void (*fn)(void*), void * arg);

/**
 * @brief      configure LEDC settings
 *
 * @param[in]  speed_mode    : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  timer_sel     : timer index(0-3), there are 4 timers in LEDC module
 *
 * @param[in]  div_num       : timer clock divide number, the timer clock is divided from the selected clock source
 *
 * @param[in]  bit_num       : the count number of one period, counter range is 0 ~ ((2 ** bit_num) - 1)
 *
 * @param[in]  clk_src       : select LEDC source clock.
 *
 * @return     -1: parameter error
 *             other value: current LEDC duty
 *
 */
esp_err_t ledc_timer_set(ledc_mode_t speed_mode, ledc_timer_t timer_sel, uint32_t div_num, uint32_t bit_num, ledc_clk_src_t clk_src);

/**
 * @brief      reset LEDC timer
 *
 * @param[in]  speed_mode    : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  timer_sel     : LEDC timer index(0-3), select from ledc_timer_t
 *
 *
 * @return     ESP_ERR_INVALID_ARG: parameter error
 *             ESP_OK: success
 *
 */
esp_err_t ledc_timer_rst(ledc_mode_t speed_mode, uint32_t timer_sel);

/**
 * @brief      pause LEDC timer counter
 *
 * @param[in]  speed_mode    : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  timer_sel     : LEDC timer index(0-3), select from ledc_timer_t
 *
 *
 * @return     ESP_ERR_INVALID_ARG: parameter error
 *             ESP_OK: success
 *
 */
esp_err_t ledc_timer_pause(ledc_mode_t speed_mode, uint32_t timer_sel);

/**
 * @brief      pause LEDC timer resume
 *
 * @param[in]  speed_mode    : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  timer_sel     : LEDC timer index(0-3), select from ledc_timer_t
 *
 *
 * @return     ESP_ERR_INVALID_ARG: parameter error
 *             ESP_OK: success
 *
 */
esp_err_t ledc_timer_resume(ledc_mode_t speed_mode, uint32_t timer_sel);

/**
 * @brief      bind LEDC channel with the selected timer
 *
 * @param[in]  speed_mode    : select the LEDC speed_mode, high-speed mode and low-speed mode, now we only support high-speed mode. We will access low-speed mode in next version
 *
 * @param[in]  channel       : LEDC channel index(0-7), select from ledc_channel_t
 *
 * @param[in]  timer_idx     : LEDC timer index(0-3), select from ledc_timer_t
 *
 *
 * @return     ESP_ERR_INVALID_ARG: parameter error
 *             ESP_OK: success
 *
 */
esp_err_t ledc_bind_channel_timer(ledc_mode_t speed_mode, uint32_t channel, uint32_t timer_idx);

/***************************EXAMPLE**********************************
 *
 *
 * ----------------EXAMPLE OF LEDC SETTING ---------------------
 *     //1. enable LEDC
 *     periph_module_enable(PERIPH_LEDC_MODULE);                //enable LEDC module, or you can not set any register of it.
 *
 *     //2. set LEDC timer
 *     ledc_timer_config_t timer_conf = {
 *         .bit_num = LEDC_TIMER_12_BIT,                        //set timer counter bit number
 *         .freq_hz = 1000,                                     //set frequency of pwm, here, 1000Hz
 *         .speed_mode = LEDC_HIGH_SPEED_MODE                   //timer mode,
 *         .timer_num = LEDC_TIMER_0,                           //timer number
 *     };
 *     ledc_timer_config(&timer_conf);                          //setup timer.
 *
 *     //3. set LEDC channel
 *     ledc_channel_config_t ledc_conf = {
 *         .channel = LEDC_CHANNEL_0;                           //set LEDC channel 0
 *         .duty = 1000;                                        //set the duty for initialization.(duty range is 0 ~ ((2**bit_num)-1)
 *         .gpio_num = 16;                                      //GPIO number
 *         .intr_type = LEDC_INTR_FADE_END;                     //GPIO INTR TYPE, as an example, we enable fade_end interrupt here.
 *         .speed_mode = LEDC_HIGH_SPEED_MODE;                  //set LEDC mode, from ledc_mode_t
 *         .timer_sel = LEDC_TIMER_0;                            //set LEDC timer source, if different channel use one timer, the frequency and bit_num of these channels should be the same
 *     }
 *     ledc_channel_config(&ledc_conf);                                 //setup the configuration
 *
 * ----------------EXAMPLE OF SETTING DUTY --- -----------------
 *     uint32_t ledc_channel = LEDC_CHANNEL_0;                  //LEDC channel(0-73)
 *     uint32_t duty = 2000;                                    //duty range is 0 ~ ((2**bit_num)-1)
 *     LEDC_set_duty(LEDC_HIGH_SPEED_MODE, ledc_channel, duty); //set speed mode, channel, and duty.
 *     ledc_update_duty(LEDC_HIGH_SPEED_MODE, ledc_channel);    //after set duty, we need to call ledc_update_duty to update the settings.
 *
 *
 * ----------------EXAMPLE OF LEDC INTERRUPT ------------------
 *     //we have fade_end interrupt and counter overflow interrupt. we just give an example of fade_end interrupt here.
 *     ledc_isr_register(18, ledc_isr_handler, NULL);           //hook the isr handler for LEDC interrupt
 *                                                              //the first parameter is INUM, you can pick one form interrupt level 1/2 which is not used by the system.
 *                                                              //NOTE1:user should arrange the INUMs that used, better not to use a same INUM for different interrupt source.
 *                                                              //NOTE2:do not pick the INUM that already occupied by the system.
 *                                                              //NOTE3:refer to soc.h to check which INUMs that can be used.
 * ----------------EXAMPLE OF INTERRUPT HANDLER ---------------
 * #include "esp_attr.h"
 * void IRAM_ATTR ledc_isr_handler(void* arg)                   //we should add 'IRAM_ATTR' attribution when we declare the isr function
 * {
 *    uint32_t intr_st = LEDC.int_st.val;                       //read LEDC interrupt status.
 *
 *    //you will find which channels have triggered fade_end interrupt here,
 *    //then, you can post some event to RTOS queue to process the event.
 *    //later we will add a queue in the driver code.
 *
 *    LEDC.int_clr.val = intr_st;                               //clear LEDC interrupt status.
 * }
 *
 *
 *--------------------------END OF EXAMPLE --------------------------
 */




#ifdef __cplusplus
}
#endif

#endif /* _DRIVER_LEDC_H_ */
