/* 
 * File:   main.c
 * Author: Manuel Navarrete + Ramiro Sosa
 *
 * Created on October 8, 2022, 8:07 PM
 */

// CONFIG
//#pragma config FOSC = INTOSCCLK // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
#include <string.h>
#include <stdlib.h>
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

// #include <math.h>
//clk real = 2MHZ -> 500ns -> 8bit entonces el reloj cuenta hasta 0.000128s prescalar 128 = 0.000001s
//

#define _XTAL_FREQ 8000000

int count=0;
int contadorAlerta=0;
void trans(const char str[]){

unsigned char transmitir;
unsigned char i;
// const char str[]="Bienvenido al sistema\n\0";
transmitir=1;
i=0;
	while(transmitir){
		TXREG=str[i];
		if (TXREG==0)
			transmitir=0;
		else
			i++;
		while(TXIF==0); //me quedo esperando a que termine el byte
	}


   
}


int sensor() {
    
    int timer;
    // Resetear TIMER1 
        TMR1H = 0;
        TMR1L = 0;
        
        // PULSO 10us TRIG
        PORTAbits.RA0 = 1;
        __delay_us(100);
        PORTAbits.RA0 = 0;
        
        int espero=1;
        T1CONbits.TMR1ON = 1;  // cuento para ver si habra evento
        while(espero==1)
        {
            if(RA1==1)
            {   espero=0;
                 TMR1H = 0;
                 TMR1L = 0;
                 T1CONbits.TMR1ON = 1; 
            }
            if(RA1==0)
            {
                 timer = (TMR1H << 8) | TMR1L;
                 if((timer) > 20000) // 1000 us equivale a 2000 aprox
                 {espero=0;
                 T1CONbits.TMR1ON = 0;
                 }
            }
        }
        
      /*  // Resetear TIMER1 
        TMR1H = 0;
        TMR1L = 0;
        
        T1CONbits.TMR1ON = 1; */
        while(RA1==1)
        {  
        }
        T1CONbits.TMR1ON = 0; 
        
        // LEER TIMER1 
        timer = (TMR1H << 8) | TMR1L;
        // Distancia
        timer = timer/(59*2)+1;
        
        // Mas preciso
        //timer = (int)(sqrt(((timer*timer)/3460.2041)-6.25));
       if(timer >=2 && timer <=400) {
         char buffer[10];
         memset(buffer,10,0);
         sprintf(buffer,"d=%d",timer);
         trans(buffer);
         trans("\n\0");
       }
        return timer;
}

void __interrupt() timer_isr(void){
    if(T0IF==1)
    {
        count++;
        T0IF=0;
        TMR0=6;
            if(count==2000)
            { // 2000 milis
                count=0;
              //  trans("Pasasron 2 segundos \n\0");
               int distancia = sensor();
               if(distancia<10)
                   contadorAlerta++;
               else
               {
                   contadorAlerta=0;
                   PORTB=0x00;
               }
               if(contadorAlerta>4)
                   PORTB=0xff;
            }
    }
}


void main(){
    CMCONbits.CM = 0b111;
    OPTION_REG = 0b00000010;
    GIE = 1;
    PEIE = 1;
    T0IE = 1;


    BRGH=1;
    SPBRG=51;
    SPEN=1;
    TXEN=1;
    CREN=1;
    TX9=0;
    SYNC=0;
    TRISB = 6;
   // PORTB = 0xff;
    
    // Ultrasonic sensor pins
    TRISAbits.TRISA0 = 0;  // TRIG pin
    TRISAbits.TRISA1 = 1;  // ECHO pin
    
    // Prescaler = 1:1
    T1CONbits.T1CKPS = 0b00;
    // Select internal clock (FOSC/4)
    T1CONbits.TMR1CS = 0;
    while(1){
        
    }
   
}
