#include "bsw_pwm.h"
#include "tim.h"

void BSW_PWM_Start(void)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0U);

    /* CubeMX sets CENTERALIGNED1 (CC events on DOWN count only).
     * Override to CENTERALIGNED3 (CC events on both UP and DOWN) so that
     * CCR3 = ARR fires exactly at the PWM peak (UP count reaching ARR).
     * TODO: update CubeMX project to CENTERALIGNED3 to avoid this override. */
    htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED3;
    HAL_TIM_Base_Init(&htim1);

    /* CH3 : PWM2 mode, CCR3 = ARR → one OC3REF rising edge per cycle at peak.
     * DOWN count starts at ARR-1 (3598) → never reaches CCR3=ARR → no 2nd edge. */
    TIM_OC_InitTypeDef oc3 = {0};
    oc3.OCMode       = TIM_OCMODE_PWM2;
    oc3.Pulse        = BSW_PWM_PERIOD - 1U;  /* CCR3 = 3598 = ARR-1 (ARR masked by update event) */
    oc3.OCPolarity   = TIM_OCPOLARITY_HIGH;
    oc3.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    oc3.OCFastMode   = TIM_OCFAST_DISABLE;
    oc3.OCIdleState  = TIM_OCIDLESTATE_RESET;
    oc3.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&htim1, &oc3, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_3);

    /* CH2 (PA9): logical inversion of CH1 (PA8), not the hardware CH1N
     * complementary output (that's PB13). Achieved by reusing CH1's compare
     * value with OCPolarity flipped to LOW, so switching instants stay
     * identical and only the output level inverts.
     * TODO: set CH2 OCPolarity=LOW in the CubeMX .ioc to avoid this override. */
    TIM_OC_InitTypeDef oc2 = {0};
    oc2.OCMode       = TIM_OCMODE_PWM1;
    oc2.Pulse        = 0U;
    oc2.OCPolarity   = TIM_OCPOLARITY_LOW;
    oc2.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    oc2.OCFastMode   = TIM_OCFAST_DISABLE;
    oc2.OCIdleState  = TIM_OCIDLESTATE_RESET;
    oc2.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    HAL_TIM_PWM_ConfigChannel(&htim1, &oc2, TIM_CHANNEL_2);

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
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, count);  /* mirrors CH1; OCPolarity_LOW inverts the output */
}
