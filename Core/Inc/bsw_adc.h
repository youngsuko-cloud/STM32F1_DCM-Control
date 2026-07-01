#ifndef INC_BSW_ADC_H_
#define INC_BSW_ADC_H_

#include "stdint.h"

/* ------------------------------------------------------------------ */
/*  BSW ADC                                                            */
/*  - ADC1 CH0 (PA0) : Current sense input                            */
/*  - Triggered by TIM1_CC3 @ 10 kHz (synchronous with PWM peak)     */
/*  - Stores latest conversion result; upper layer reads on demand    */
/* ------------------------------------------------------------------ */

void     BSW_ADC_Start(void);           /* Calibrate and start ADC interrupt mode */
uint16_t BSW_ADC_GetLatestAD(void);     /* Returns latest AD value (0 ~ 4095) */

#endif /* INC_BSW_ADC_H_ */
