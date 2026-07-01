#include "asw.h"
#include "abi.h"

static float s_duty_pct = 0.0f;

void ASW_Init(void)
{
    s_duty_pct = 0.0f;
    ABI_MonitorRegister("duty", &s_duty_pct);
}

/* Main loop context - UART TX (ACK/NACK) is safe here */
void ASW_HandleCommand(void)
{
    ABI_Cmd_t cmd = ABI_GetCommand();

    if (cmd.type == ABI_CMD_VOLTAGE) {
        s_duty_pct = cmd.value;
    }
    /* ABI_CMD_CURRENT : not active (no sensing HW) */
}

/* ISR context - only register write, safe */
void ASW_Run(void)
{
    ABI_SetDuty(s_duty_pct);
}
