#include "asw.h"
#include "abi.h"
#include "bsw_adc.h"

static float s_duty_pct = 0.0f;


void ASW_Init(void)
{
    s_duty_pct = 50.0f;
    ABI_MonitorRegister("duty", &s_duty_pct);
    BSW_ADC_RegisterCallback(ASW_Run);
}

void ASW_HandleCommand(void)
{
    (void)ABI_GetCommand();
}

/* Called from ADC ConvCplt ISR - fixed 50% to verify interrupt + PWM chain */
void ASW_Run(void)
{
    ABI_SetDuty(s_duty_pct);
}
