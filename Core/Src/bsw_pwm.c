#include "bsw_pwm.h"
#include "tim.h"

void BSW_PWM_Start(void)
{
    /* Set duty to 0 before enabling outputs */
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 0U);

    /* CH3 : PWM2 mode, CCR3 = ARR → OC3REF rising edge exactly at PWM peak.
     * CubeMX configured CH3 as OC Timing (Frozen): OC3REF never changes → no
     * rising edge → ADC external trigger (TIM1_CC3) never fires.
     * PWM2 with CCR3=ARR: OC3REF=LOW for counter<ARR, HIGH at counter=ARR
     * → one rising edge per cycle at the peak → triggers ADC. */
    TIM_OC_InitTypeDef oc3 = {0};
    oc3.OCMode       = TIM_OCMODE_PWM2;
    oc3.Pulse        = 1800U;                /* CCR3 = 1800, test value */
    oc3.OCPolarity   = TIM_OCPOLARITY_HIGH;
    oc3.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    oc3.OCFastMode   = TIM_OCFAST_DISABLE;
    oc3.OCIdleState  = TIM_OCIDLESTATE_RESET;
    oc3.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&htim1, &oc3, TIM_CHANNEL_3);
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
