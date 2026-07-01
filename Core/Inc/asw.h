#ifndef INC_ASW_H_
#define INC_ASW_H_

/* ------------------------------------------------------------------ */
/*  ASW : Application Software                                         */
/*  Current stage : Voltage open-loop (V command -> duty passthrough) */
/*  Current control (C command) : not active, pending sensor HW       */
/* ------------------------------------------------------------------ */

void ASW_Init(void);
void ASW_HandleCommand(void);   /* Called from main loop  - parses command, sends ACK via UART */
void ASW_Run(void);             /* Called every 1 ms from TIM2 ISR - applies duty to PWM only */

#endif /* INC_ASW_H_ */
