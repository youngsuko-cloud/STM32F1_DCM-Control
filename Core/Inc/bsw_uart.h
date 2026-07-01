#ifndef INC_BSW_UART_H_
#define INC_BSW_UART_H_

#include "stdint.h"

/* ------------------------------------------------------------------ */
/*  BSW UART                                                           */
/*  - USART2 : PA2 (TX), PA3 (RX), 115200 bps 8N1                    */
/*  - RX : interrupt-driven, line-buffered (\r or \n as delimiter)    */
/*  - TX : blocking with short timeout                                 */
/* ------------------------------------------------------------------ */

#define BSW_UART_RX_BUF_LEN    32U     /* max command line length (chars) */

void    BSW_UART_Start(void);                               /* Start UART RX interrupt */
uint8_t BSW_UART_GetLine(char *buf, uint16_t max_len);      /* 1 if new line available */
void    BSW_UART_Transmit(const char *buf, uint16_t len);   /* Blocking TX */

#endif /* INC_BSW_UART_H_ */
