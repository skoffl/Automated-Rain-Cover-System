#include <pic.h>
#define _XTAL_FREQ 4000000
// Function Declarations
void adc0();
void serial_init();
void serout(unsigned char);
void serconout(const unsigned char , unsigned char);
void gsm_init();
void mes_num1();
void delay(unsigned int);
void interrupt rcc(void);
void lcd_init();
void command(unsigned char);
void lcd_disp(unsigned char);
void write(unsigned char);
void lcd_condis(const unsigned char , unsigned int);
void hex_dec(unsigned char);
void dis_delay();
void msg_read();
// Variable Declarations
40
unsigned char k, v1, v2, v3, rc, d[80], COUNTER1 = 0, GG = 0, cu, FFF = 0, tee
= 0;
unsigned char iii, vvv[95], n, m, e, y = 0, ss = 0, PP = 0, s = 0;
unsigned int temp1;
bit gfg = 0;
// Bit Declarations
static bit ON @ ((unsigned) &PORTB 8 + 0);
static bit OFF @ ((unsigned) &PORTB 8 + 1);
static bit M @ ((unsigned) &PORTC 8 + 0);
static bit pump @ ((unsigned) &PORTB 8 + 3);
static bit key @ ((unsigned) &PORTC 8 + 2);
static bit rs @ ((unsigned) &PORTC 8 + 5);
static bit en @ ((unsigned) &PORTC 8 + 4);
// Main Function
void main() {
 __CONFIG(0X2F02);
 TRISC = 0XC5;
 ADCON1 = 0X00;
 TRISA = 0XFF;
 TRISB = 0X00;
 pump = 0;
 ON = 1;
 lcd_init();
 command(0x80);
 lcd_condis(" W/L CONTROL ", 16);
 command(0xc0);
 lcd_condis("RAIN MONRING SYS", 16);
 delay(65000);
 delay(65000);
 delay(65000);
 serial_init();
 command(0xc0);
 lcd_condis("GSM INITIALIZE..", 16);
 __delay_ms(25000);
41
 gsm_init();
 while (1) {
 CREN = 1;
 iii = 1;
 dis_delay();
 for (n = 0; n <= 85; n++) {
 if (vvv[n] == '+' && vvv[n + 1] == 'C' && vvv[n + 2] == 'M' && vvv[n +
3] == 'T' && vvv[n + 4] == 'I' && vvv[n + 5] == ':') {
 msg_read();
 }
 command(0xc0);
 lcd_condis("RAIN MONRING SYS", 16);
 adc0();
 command(0x82);
 hex_dec(v1);
 delay(20000);
 command(0x80);
 lcd_condis("S: ", 16);
 command(0xc0);
 lcd_condis("RAIN MONRING SYS", 16);
 if (v1 < 50 && GG == 0) {
 ON = 0;
 command(0x80);
 lcd_condis(" RAIN DETECTED ", 16);
 command(0xc0);
 lcd_condis("MOTOR TURN ON...", 16);
 delay(65000);
 delay(65000);
 delay(65000);
 command(0x01);
 command(0xc0);
 lcd_condis("MSG SENDING.....", 16);
 dis_delay();
 mes_num1();
42
 serconout("RAIN DETECTED SYSTEM COVERED THE CLATH ",
63);
 serout(0x1a);
 delay(65000);
 delay(65000);
 delay(65535);
 delay(65535);
 delay(65535);
 command(0xc0);
 lcd_condis("RAIN MONRING SYS", 16);
 GG = 1;
 } else if (v1 > 50 && GG == 1) {
 ON = 1;
 command(0x80);
 lcd_condis(" NO RAIN DETECTED", 16);
 command(0xc0);
 lcd_condis("MOTOR TURN OFF..", 16);
 delay(65000);
 delay(65000);
 delay(65000);
 command(0x01);
 command(0xc0);
 lcd_condis("MSG SENDING.....", 16);
 dis_delay();
 mes_num1();
 serconout("NO RAIN DETECTED SYSTEM COVER OPENED", 63);
 serout(0x1a);
 delay(65000);
 delay(65000);
 delay(65535);
 delay(65535);
 delay(65535);
 command(0xc0);
 lcd_condis("RAIN MONRING SYS", 16);
 GG = 0;
 }
 }
 }
}
43
// ADC Read Function
void adc0() {
 temp1 = 0;
 for (k = 0; k < 10; k++) {
 ADCON0 = 0X00;
 ADON = 1;
 delay(55);
 ADCON0 = 0X05;
 while (ADCON0 != 0X01);
 v1 = ADRESH;
 temp1 = temp1 + v1;
 }
 v1 = temp1 / 10;
 delay(2000);
}
// Serial Initialization Function
void serial_init() {
 GIE = PEIE = 1;
 SPBRG = 25;
 BRGH = 1;
 SYNC = 0;
 SPEN = 1;
 CREN = 0;
 RCIE = 1;
 TXEN = 1;
}
// GSM Initialization Function
void gsm_init(void) {
 serconout("AT+CMGF=1", 9);
 __delay_us(1000);
 serout(0x0d);
 __delay_ms(3000);
 serconout("AT+CPMS=", 8);
 serout('"');
 serout('S');
44
 serout('M');
 serout('"');
 serout(0x0d);
 __delay_ms(1000);
 serconout("AT+CLIP=1", 9);
 serout(0x0d);
 __delay_ms(3000);
 command(0xc0);
 lcd_condis(" Msg. sending ", 16);
 __delay_ms(1);
 mes_num1();
 __delay_us(65);
 serconout("UR SYS IS READY TO USE", 22);
 serout(' ');
 serout(0x
0d);
 serout(0x0d);
 serout(0x1a);
 delay(13000);
 command(0xc0);
 lcd_condis("UR SYS IS READY ", 16);
 __delay_ms(5000);
}
// Delay Function
void delay(unsigned int del) {
 while (del--);
}
// Interrupt Function
void interrupt rcc(void) {
 if (RCIF == 1) {
 RCIF = 0;
45
 vvv[iii] = RCREG;
 if (iii < 95) iii++;
 else iii = 0;
 }
}
// LCD Initialization Function
void lcd_init() {
 TRISD = 0;
 command(0x38);
 command(0x06);
 command(0x0c);
 command(0x01);
}
// LCD Command Function
void command(unsigned char com) {
 PORTD = com;
 en = 1;
 rs = 0;
 delay(125);
 en = 0;
 delay(125);
}
// LCD Display Function
void lcd_disp(unsigned char lr) {
 PORTD = lr;
 en = 1;
 rs = 1;
 delay(125);
 en = 0;
 delay(125);
}
// LCD Write Function
void write(unsigned char lr) {
 PORTD = lr;
 en = 1;
46
 rs = 1;
 delay(125);
 en = 0;
 delay(125);
}
// LCD Condition Display Function
void lcd_condis(const unsigned char word, unsigned int n) {
 unsigned char i;
 for (i = 0; i < n; i++) {
 lcd_disp(word[i]);
 }
}
// Hexadecimal to Decimal Conversion Function
void hex_dec(unsigned char val) {
 write(val % 1000 / 100 + 0x30);
 write(val % 100 / 10 + 0x30);
 write(val % 10 + 0x30);
}
// Display Delay Function
void dis_delay() {
 delay(65000);
 delay(65000);
 delay(65000);
 delay(65000);
 delay(65000);
 delay(65000);
}
// Message Sending Function
void mes_num1() {
 serconout("AT+CMGS=", 8);
 serout('"');
 serconout("9865106810", 10);
 serout('"');
 serout(0x0d);
 delay(65000);}
