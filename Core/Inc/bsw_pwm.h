#ifndef INC_BSW_PWM_H_
#define INC_BSW_PWM_H_

#include "stdint.h"

/* ------------------------------------------------------------------ */
/*  BSW PWM                                                            */
/*  - TIM1 CH1 (PA8) : PWM output to H-bridge                        */
/*  - TIM1 CH2 (PA9) : Logical inversion of CH1 (OCPolarity=LOW)     */
/*  - TIM1 CH3       : OC Timing mode -> ADC external trigger         */
/*  - Center-aligned, 10 kHz, ARR = 3599                              */
/* ------------------------------------------------------------------ */

#define BSW_PWM_PERIOD      3599U   /* TIM1 ARR value */

void BSW_PWM_Start(void);                       /* Start TIM1: OC(CH3) + PWM(CH1,CH2) */
void BSW_PWM_Stop(void);                        /* Set duty to 0, outputs remain active */
void BSW_PWM_SetDutyCount(uint32_t count);      /* 0 ~ BSW_PWM_PERIOD */

#endif /* INC_BSW_PWM_H_ */
