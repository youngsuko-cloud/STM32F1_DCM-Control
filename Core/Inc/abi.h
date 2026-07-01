#ifndef INC_ABI_H_
#define INC_ABI_H_

#include "stdint.h"

/* ------------------------------------------------------------------ */
/*  ABI : Application Basic Interface                                  */
/*  - Upward   : Sensed AD value, Received command                     */
/*  - Downward : Duty output (0.0 ~ 100.0 %)                          */
/*  - Monitor  : Register float variables, transmit via UART @ 10 ms  */
/* ------------------------------------------------------------------ */

#define ABI_MONITOR_MAX_VARS    6U      /* max number of monitor variables */
#define ABI_MONITOR_NAME_LEN    8U      /* max variable name length (chars) */

/* Control command types */
typedef enum {
    ABI_CMD_NONE    = 0,
    ABI_CMD_CURRENT,        /* Target current  [mA], format: "C <value>" */
    ABI_CMD_VOLTAGE,        /* Target voltage  [%],  format: "V <value>" */
} ABI_CmdType_t;

typedef struct {
    ABI_CmdType_t type;
    float         value;
} ABI_Cmd_t;

/* ------------------------------------------------------------------ */
/*  Upward Interface  (BSW -> ASW)                                     */
/* ------------------------------------------------------------------ */
uint16_t  ABI_GetSensedCurrentAD(void);   /* Returns latest AD value (0~4095) */
ABI_Cmd_t ABI_GetCommand(void);           /* Returns parsed command, type=NONE if none */

/* ------------------------------------------------------------------ */
/*  Downward Interface  (ASW -> BSW)                                   */
/* ------------------------------------------------------------------ */
void ABI_SetDuty(float duty_pct);         /* 0.0 ~ 100.0 [%] */

/* ------------------------------------------------------------------ */
/*  Monitoring Interface                                               */
/* ------------------------------------------------------------------ */
void ABI_MonitorRegister(const char *name, float *var_ptr);
void ABI_MonitorUpdate(void);             /* Call every 10 ms from main loop */

#endif /* INC_ABI_H_ */
