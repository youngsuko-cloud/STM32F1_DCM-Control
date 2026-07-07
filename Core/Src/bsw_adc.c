#include "bsw_adc.h"
#include "adc.h"

static volatile uint16_t   s_latest_ad = 0U;
static BSW_ADC_Callback_t  s_callback  = 0;

void BSW_ADC_Start(void)
{
    /* DEBUG: PB0 pulse to verify ADC sampling timing on scope */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef dbg = {0};
    dbg.Pin   = GPIO_PIN_0;
    dbg.Mode  = GPIO_MODE_OUTPUT_PP;
    dbg.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &dbg);
    GPIOB->BSRR = GPIO_PIN_0 << 16U;  /* LOW */

    HAL_ADCEx_Calibration_Start(&hadc1);

    /* NVIC enabled by CubeMX in HAL_ADC_MspInit (adc.c) */

    /* Start ADC in interrupt mode; TIM1_CC3 external trigger fires conversions */
    HAL_ADC_Start_IT(&hadc1);
}

void BSW_ADC_RegisterCallback(BSW_ADC_Callback_t cb)
{
    s_callback = cb;
}

uint16_t BSW_ADC_GetLatestAD(void)
{
    return s_latest_ad;
}

/* ------------------------------------------------------------------ */
/*  HAL callback : called after each TIM1_CC3-triggered conversion     */
/*  Re-arms ADC so the next TIM1_CC3 event triggers another sample.   */
/* ------------------------------------------------------------------ */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1) {
        GPIOB->BSRR = GPIO_PIN_0;           /* DEBUG: HIGH - ISR start */

        s_latest_ad = (uint16_t)HAL_ADC_GetValue(hadc);
        HAL_ADC_Start_IT(hadc);     /* re-arm for next external trigger */

        if (s_callback != 0) {
            s_callback();           /* notify upper layer every PWM cycle */
        }

        GPIOB->BSRR = GPIO_PIN_0 << 16U;   /* DEBUG: LOW - ISR end */
    }
}
