#include "abi.h"
#include "bsw_adc.h"
#include "bsw_pwm.h"
#include "bsw_uart.h"

#include <stdio.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Internal : Monitoring                                              */
/* ------------------------------------------------------------------ */
typedef struct {
    char   name[ABI_MONITOR_NAME_LEN + 1U];
    float *ptr;
} ABI_MonVar_t;

static ABI_MonVar_t s_mon_vars[ABI_MONITOR_MAX_VARS];
static uint8_t      s_mon_count = 0U;

/* ------------------------------------------------------------------ */
/*  Upward Interface                                                   */
/* ------------------------------------------------------------------ */

uint16_t ABI_GetSensedCurrentAD(void)
{
    return BSW_ADC_GetLatestAD();
}

ABI_Cmd_t ABI_GetCommand(void)
{
    ABI_Cmd_t cmd = { ABI_CMD_NONE, 0.0f };
    char      line[32];
    char      resp[24];
    int       resp_len = 0;
    float     val;

    if (!BSW_UART_GetLine(line, sizeof(line))) {
        return cmd;
    }

    if (sscanf(line, "C %f", &val) == 1) {
        cmd.type  = ABI_CMD_CURRENT;
        cmd.value = val;
        resp_len  = snprintf(resp, sizeof(resp), "ACK C %.1f\r\n", val);

    } else if (sscanf(line, "V %f", &val) == 1) {
        cmd.type  = ABI_CMD_VOLTAGE;
        cmd.value = val;
        resp_len  = snprintf(resp, sizeof(resp), "ACK V %.1f\r\n", val);

    } else {
        resp_len = snprintf(resp, sizeof(resp), "NACK\r\n");
    }

    if (resp_len > 0) {
        BSW_UART_Transmit(resp, (uint16_t)resp_len);
    }

    return cmd;
}

/* ------------------------------------------------------------------ */
/*  Downward Interface                                                 */
/* ------------------------------------------------------------------ */

void ABI_SetDuty(float duty_pct)
{
    if (duty_pct < 0.0f)   { duty_pct = 0.0f;   }
    if (duty_pct > 100.0f) { duty_pct = 100.0f; }

    uint32_t count = (uint32_t)(duty_pct / 100.0f * (float)BSW_PWM_PERIOD);
    BSW_PWM_SetDutyCount(count);
}

/* ------------------------------------------------------------------ */
/*  Monitoring                                                         */
/* ------------------------------------------------------------------ */

void ABI_MonitorRegister(const char *name, float *var_ptr)
{
    if (s_mon_count >= ABI_MONITOR_MAX_VARS) { return; }

    strncpy(s_mon_vars[s_mon_count].name, name, ABI_MONITOR_NAME_LEN);
    s_mon_vars[s_mon_count].name[ABI_MONITOR_NAME_LEN] = '\0';
    s_mon_vars[s_mon_count].ptr = var_ptr;
    s_mon_count++;
}

void ABI_MonitorUpdate(void)
{
    char    buf[96];
    int     pos = 0;
    uint8_t i;

    for (i = 0U; i < s_mon_count; i++) {
        pos += snprintf(buf + pos, sizeof(buf) - (size_t)pos,
                        "%s:%.1f ", s_mon_vars[i].name, *s_mon_vars[i].ptr);
        if (pos >= (int)sizeof(buf) - 1) { break; }
    }

    if (pos > 0) {
        buf[pos - 1] = '\n';    /* replace trailing space with newline */
        BSW_UART_Transmit(buf, (uint16_t)pos);
    }
}
