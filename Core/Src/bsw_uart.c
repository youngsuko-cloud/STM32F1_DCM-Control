#include "bsw_uart.h"
#include "usart.h"

#include <string.h>

/* ------------------------------------------------------------------ */
/*  Internal buffers                                                   */
/* ------------------------------------------------------------------ */
static uint8_t  s_rx_byte;                          /* single-byte DMA target */
static char     s_rx_build[BSW_UART_RX_BUF_LEN];   /* line being built */
static uint16_t s_rx_idx    = 0U;
static char     s_rx_line[BSW_UART_RX_BUF_LEN];    /* completed line */
static volatile uint8_t s_line_ready = 0U;

/* ------------------------------------------------------------------ */
/*  Public functions                                                   */
/* ------------------------------------------------------------------ */

void BSW_UART_Start(void)
{
    HAL_UART_Receive_IT(&huart2, &s_rx_byte, 1U);
}

uint8_t BSW_UART_GetLine(char *buf, uint16_t max_len)
{
    if (!s_line_ready) { return 0U; }

    strncpy(buf, s_rx_line, max_len - 1U);
    buf[max_len - 1U] = '\0';
    s_line_ready = 0U;
    return 1U;
}

void BSW_UART_Transmit(const char *buf, uint16_t len)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, len, 20U);
}

/* ------------------------------------------------------------------ */
/*  HAL callback : called after each received byte                    */
/* ------------------------------------------------------------------ */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != USART2) { return; }

    uint8_t c = s_rx_byte;

    if ((c == '\r') || (c == '\n')) {
        if (s_rx_idx > 0U) {
            s_rx_build[s_rx_idx] = '\0';
            strncpy(s_rx_line, s_rx_build, BSW_UART_RX_BUF_LEN - 1U);
            s_rx_line[BSW_UART_RX_BUF_LEN - 1U] = '\0';
            s_line_ready = 1U;
            s_rx_idx     = 0U;
        }
    } else if (s_rx_idx < (BSW_UART_RX_BUF_LEN - 1U)) {
        s_rx_build[s_rx_idx++] = (char)c;
    }

    /* Re-arm for next byte */
    HAL_UART_Receive_IT(huart, &s_rx_byte, 1U);
}
