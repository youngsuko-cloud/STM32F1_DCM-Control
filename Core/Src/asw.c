#include "asw.h"
#include "abi.h"
#include "bsw_adc.h"

static float s_duty_pct = 0.0f;

#define RAMP_PERIOD  20000U   /* 1 s ramp at ~20 kHz ISR rate */

void ASW_Init(void)
{
    s_duty_pct = 0.0f;
    ABI_MonitorRegister("duty", &s_duty_pct);
    BSW_ADC_RegisterCallback(ASW_Run);
}

void ASW_HandleCommand(void)
{
    (void)ABI_GetCommand();
}

/* Called from ADC ConvCplt ISR ~20 kHz - triangle-ramps duty 0→100%→0% every 2 s */
void ASW_Run(void)
{
    static uint32_t s_count = 0U;
    static uint8_t  s_up    = 1U;

    s_duty_pct = (float)s_count * 100.0f / (float)RAMP_PERIOD;
    ABI_SetDuty(s_duty_pct);

    if (s_up) {
        if (++s_count >= RAMP_PERIOD) {
            s_count = RAMP_PERIOD;
            s_up    = 0U;
        }
    } else {
        if (s_count == 0U) {
            s_up = 1U;
        } else {
            --s_count;
        }
    }
}
