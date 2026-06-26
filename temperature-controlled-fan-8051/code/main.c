#include <reg52.h> 
#include <string.h> 
#define ldata P2 
void lcddata(unsigned char *str); 
void lcdcmd(unsigned char cmd); 
void msdelay(unsigned int itime); 
void usdelay(unsigned int itime); 
void datawrt(unsigned char); 
unsigned char adc(); 
void set_pwm_duty(unsigned char duty); 
void fan_off(); 
void fan_on(); 
unsigned char pwm_duty    
= 0; 
unsigned char pwm_counter = 0; 
unsigned char fan_running = 0; 
sbit rs = P3^3; 
sbit rw = P3^4; 
sbit en = P3^5; 
sbit rd_adc   = P3^0; 
sbit wr_adc   = P3^1; 
sbit intr_adc = P3^2; 
void Timer0_ISR() interrupt 1 
{ 
if(fan_running == 0) 
{ 
fanIN2      
fanIN1      
= 0; 
= 0; 
pwm_counter = 0; 
return; 
} 
fanIN1 = 0; 
pwm_counter++; 
if(pwm_counter >= 100) 
pwm_counter = 0; 
if(pwm_duty == 0) 
fanIN2 = 0; 
else if(pwm_duty >= 100) 
fanIN2 = 1; 
else 
fanIN2 = (pwm_counter < pwm_duty) ? 1 : 0; 
} 
void set_pwm_duty(unsigned char duty) 
{ 
if(duty > 100) duty = 100; 
pwm_duty = duty; 
} 
void fan_off() 
{ 
fan_running = 0; 
pwm_duty    
fanIN1      
fanIN2      
} 
= 0; 
= 0; 
= 0; 
void fan_on() 
{ 
fanIN1      
= 0; 
fan_running = 1; 
} 
unsigned char adc() 
{ 
unsigned char conv; 
unsigned int  timeout; 
rd_adc = 1; 
wr_adc = 1; 
usdelay(10); 
wr_adc = 0; 
    usdelay(10); 
    wr_adc = 1; 
    timeout = 10000; 
    while(intr_adc == 1) 
    { 
        timeout--; 
        if(timeout == 0) return 0; 
    } 
    rd_adc = 0; 
    usdelay(10); 
    conv   = P1; 
    rd_adc = 1; 
    wr_adc = 0; 
    usdelay(5); 
    wr_adc = 1; 
    return conv; 
} 
void main() 
{ 
    unsigned char raw_adc; 
    unsigned int  temp_c; 
    unsigned char x1, x2, x3; 
    ldata    = 0x00; 
    rs = rw = en = 0; 
    P1       = 0xFF; 
    intr_adc = 1; 
    rd_adc   = 1; 
    wr_adc   = 1; 
    fan_off(); 
    /* Timer0 Mode 2 — 112Hz PWM */ 
    TMOD = 0x02; 
    TH0  = 174; 
    TL0  = 174; 
    ET0  = 1; 
    EA   = 1; 
    TR0  = 1; 
    msdelay(20); 
    lcdcmd(0x38); 
    lcdcmd(0x0E); 
    lcdcmd(0x06); 
    lcdcmd(0x01); 
    msdelay(5); 
    lcdcmd(0x0C); 
    lcdcmd(0x80); 
    lcddata("Welcome..."); 
    msdelay(1500); 
    lcdcmd(0x01); 
    msdelay(5); 
    lcdcmd(0x80); 
    lcddata("Temp:"); 
    lcdcmd(0xC0); 
    lcddata("Fan:"); 
    while(1) 
    { 
        raw_adc = adc(); 
        temp_c  = ((unsigned int)raw_adc * 100) / 51; 
        if(temp_c > 199) temp_c = 199; 
        lcdcmd(0x86); 
        x1 = (temp_c / 100) + '0';        datawrt(x1); 
        x2 = ((temp_c / 10) % 10) + '0';  datawrt(x2); 
        x3 = (temp_c % 10) + '0';         datawrt(x3); 
        datawrt(0xDF); 
        datawrt('C'); 
        if(temp_c < 30) 
        { 
            lcdcmd(0xC5); 
            lcddata("OFF      "); 
            fan_off(); 
        } 
        else if(temp_c < 50) 
        { 
            lcdcmd(0xC5); 
            lcddata("SLOW     "); 
            fan_on(); 
            set_pwm_duty(30); 
        } 
        else if(temp_c < 70) 
        { 
            lcdcmd(0xC5); 
            lcddata("MEDIUM   "); 
            fan_on(); 
            set_pwm_duty(65); 
        } 
else 
{ 
lcdcmd(0xC5); 
lcddata("FAST     "); 
fan_on(); 
set_pwm_duty(100); 
} 
msdelay(500); 
} 
} 
void datawrt(unsigned char y) 
{ 
ldata = y; rs = 1; rw = 0; en = 1; 
msdelay(1); en = 0; 
} 
void lcddata(unsigned char *str) 
{ 
while(*str != '\0') 
{ 
ldata = *str; rs = 1; rw = 0; en = 1; 
msdelay(1); en = 0; 
str++; 
} 
} 
void lcdcmd(unsigned char cmd) 
{ 
ldata = cmd; rs = 0; rw = 0; en = 1; 
msdelay(1); en = 0; 
} 
void msdelay(unsigned int itime) 
{ 
unsigned int i, j; 
for(i = 0; i < itime; i++) 
for(j = 0; j < 125; j++); 
} 
void usdelay(unsigned int itime) 
{ 
unsigned int i; 
for(i = 0; i < itime; i++); 
} 