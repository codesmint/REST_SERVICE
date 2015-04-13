#pragma once
#include <avr/io.h>
#include <util/delay.h>


int init_UART(void);

void send_Uart(uint8_t c);//	Отправка байта

void send_Uart_str(uint8_t *s);//	Отправка строки

void send_int_Uart(uint8_t c);//	Отправка числа 8 битов

void send_int32_Uart(uint32_t c);//	Отправка числа 32 бита

void send_IP(uint32_t c);

void send_MAC(uint8_t mac_addr[6]);
