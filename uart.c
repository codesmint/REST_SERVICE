#include "uart.h"


#define htonl(a)			( (((a)>>24)&0xff) | (((a)>>8)&0xff00) |\
								(((a)<<8)&0xff0000) | (((a)<<24)&0xff000000) )
#define ntohl(a)			htonl(a)


int init_UART(void)
{
	//	��������� �������� 9600
	UBRRH=0;	//	UBRR=f/(16*band)-1 f=8000000�� band=9600, 
	UBRRL=103;	//	���������� ����������� ��������������� ����� ������
	
//			RXC			-	���������� �����
//			|TXC		-	���������� ��������
//			||UDRE 		-	���������� ������ ��� ��������
//			|||FE		-	������ �����
//			||||DOR		-	������ ������������ ������
//			|||||PE		-	������ ��������
//			||||||U2X	-	������� ��������
//			|||||||MPCM	-	����������������� �����
//			76543210
	UCSRA=0b00000000;

//			RXCIE		-	���������� ��� ����� ������
//			|TXCIE		-	���������� ��� ���������� ��������
//			||UDRIE		-	���������� ���������� ������ ��� ��������
//			|||RXEN		-	���������� �����
//			||||TXEN	-	���������� ��������
//			|||||UCSZ2	-	UCSZ0:2 ������ ����� ������
//			||||||RXB8	-	9 ��� �������� ������
//			|||||||TXB8	-	9 ��� ���������� ������
//			76543210
	UCSRB=0b00011000;	//	�������� ���� � �������� �� UART

//			URSEL		-	������ 1
//			|UMSEL		-	�����:1-���������� 0-�����������
//			||UPM1		-	UPM0:1 ��������
//			|||UPM0		-	UPM0:1 ��������
//			||||USBS	-	��� ����: 0-1, 1-2
//			|||||UCSZ1	-	UCSZ0:2 ������ ����� ������
//			||||||UCSZ0	-	UCSZ0:2 ������ ����� ������
//			|||||||UCPOL-	� ���������� ������ - ������������
//			76543210
	UCSRC=0b10000110;	//	8-������� �������
}

//	UART
void send_Uart(uint8_t c)//	�������� �����
{
	while(!(UCSRA&(1<<UDRE)))	//	���������������, ����� ������� ��������
	{}
	UDR = c;
}

void send_Uart_str(uint8_t *s)//	�������� ������
{
	while (*s != 0) send_Uart(*s++);
}

void send_int_Uart(uint8_t c)//	�������� ����� �� 000 �� 999
{
	unsigned char temp;
	c=c%1000;
	temp=c/10;//75
	send_Uart(temp/10+'0');//7
	send_Uart(temp%10+'0');//5
	temp=c%10;
	send_Uart(temp+'0');

}

void my_uitoa(uint32_t zahl, char* string) {
  int8_t i;                             // schleifenz�hler
 
  string[10]='\0';                       // String Terminator
  for(i=9; i>=0; i--) {
    string[i]=(zahl % 10) +'0';         // Modulo rechnen, dann den ASCII-Code von '0' addieren
    zahl /= 10;
  }
}

void send_int32_Uart(uint32_t c) {
	
	uint8_t wtf[11];
    my_uitoa(c,wtf);
	
	send_Uart_str(wtf);
	



}
void send_IP(uint32_t c)//	�������� �����
{
	uint32_t temp=htonl(c);

	uint8_t b1 = c;       /* first byte */
    uint8_t b2 = c >> 8;  /* etc */
    uint8_t b3 = c >> 16; /* etc */
    uint8_t b4 = c >> 24; /* etc */
	
	send_int_Uart(b1);
	send_Uart('.');
	send_int_Uart(b2);
	send_Uart('.');
	send_int_Uart(b3);
	send_Uart('.');
	send_int_Uart(b4);
	send_Uart('.');
}


void send_MAC(uint8_t mac_addr[6]) {

	send_int_Uart(mac_addr[0]);
	send_Uart('.');
	send_int_Uart(mac_addr[1]);
	send_Uart('.');
	send_int_Uart(mac_addr[2]);
	send_Uart('.');
	send_int_Uart(mac_addr[3]);
	send_Uart('.');
	send_int_Uart(mac_addr[4]);
	send_Uart('.');
	send_int_Uart(mac_addr[5]);
	send_Uart('.');
}
