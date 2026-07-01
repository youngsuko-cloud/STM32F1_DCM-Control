#include "bsw_pwm.h"
#include "tim.h"

void BSW_PWM_Start(void)
{
    /* Set duty to 0 before enabling outputs */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0U);

    /* CH3 : OC Timing mode -> generates TIM1_CC3 event for ADC trigger */
    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_3);

    /* CH1, CH2 : PWM outputs to H-bridge */
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void BSW_PWM_Stop(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0U);
}

void BSW_PWM_SetDutyCount(uint32_t count)
{
    if (count > BSW_PWM_PERIOD) { count = BSW_PWM_PERIOD; }
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, count);

    /* CH2 is kept at 0 (unidirectional).
     * [BSW REQ] If bidirectional control is required, CH2 handling must be added.
     * See bsw_pwm.h comments. */
}
