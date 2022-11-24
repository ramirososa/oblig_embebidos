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


//clk real = 2MHZ -> 500ns -> 8bit entonces el reloj cuenta hasta 0.000128s prescalar 128 = 0.000001s
//

#define _XTAL_FREQ 8000000
unsigned int velocidad = 340;
int count=0;
unsigned char contadorAlerta=0;

void trans(const char str[]){

unsigned char transmitir;
unsigned char i;
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

   unsigned long timer;
   unsigned long time;
    // Resetear TIMER1
        TMR1H = 0;
        TMR1L = 0;
       
        // PULSO 10us TRIG
        PORTAbits.RA0 = 1;
        __delay_us(100);
        PORTAbits.RA0 = 0;
       
        char espero=1;
        char pasoTiempo=0; // paso tiempo es uno si estuve esperando ya 2000us por un pulso en RA1 y no ha llegado nada
        // me permite no seguir esperando y protegerme en caso de echo desconectado.
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
                 pasoTiempo=1;
                 T1CONbits.TMR1ON = 0;
                 }
            }
        }
        if(pasoTiempo==0)
        {
        while(RA1==1)
        {  
        }
        T1CONbits.TMR1ON = 0;
       
        // LEER TIMER1
       
         time = (TMR1H << 8) | TMR1L;
        // Distancia
         timer = velocidad*time;
         timer = timer / 40000;

        if(timer >=2 && timer <=400) {
         char buffer[10];
         memset(buffer,10,0);
         sprintf(buffer,"%d",timer);  // Consume un monton de memoria
         trans(buffer);
         char bufferNuevo[10];
         memset(bufferNuevo,10,0);
         sprintf(bufferNuevo,"%d",velocidad);
	 delay_ms(1000);
         trans(bufferNuevo);
        }
        return timer;
        }
        return 0;
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
               int distancia = sensor();
               if(distancia<30)
                   contadorAlerta++;
               else
               {
                   contadorAlerta=0;
               }
               if(contadorAlerta==15)
               {
                   PORTB=0xff;
                 //trans("Alerta\n");
               }
            }
    }
}

void guardar_EEPROM(unsigned int v){
    EEADR = 0x00;
    EEDATA = v-300;
    EECON1bits.WREN = 1;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = 1;
}


unsigned int leer_EEPROM(){
    unsigned int data;
    EEADR = 0x00;
    EECON1bits.RD = 1;
    data = EEDATA;
    data+=300;
    if(data<300 || data>400)data=340;
    EECON1bits.RD = 0;  
    return data;
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
   
    // Ultrasonic sensor pins
    TRISAbits.TRISA0 = 0;  // TRIG pin
    TRISAbits.TRISA1 = 1;  // ECHO pin
   
    // Prescaler = 1:1
    T1CONbits.T1CKPS = 0b00;
    // Select internal clock (FOSC/4)
    T1CONbits.TMR1CS = 0;
   
   
   
    // RECIBIR
    velocidad = leer_EEPROM();

    while(1)
    {
        unsigned char recibir;
        unsigned char i;
        char Buf[32];
        recibir=1;
        unsigned int velocidadNueva=0;
        i=0;
        unsigned int numeroActual;
    while(recibir){
        while(RCIF==0); // Loop Bytes
        Buf[i]=RCREG;
            if(RCREG>='0' && RCREG<='9') // Entra un digito
            {
                // trans("Entro un digito\n\0");
                numeroActual= RCREG - 48;
                velocidadNueva = velocidadNueva*10;
                velocidadNueva += numeroActual;
            }
            else if (RCREG=='\r') // Entra un ENTER
            {
                recibir=0;
                if(velocidadNueva>300 && velocidadNueva<400)
                {
                velocidad = velocidadNueva;
                //trans("VS");
                //char bufferNuevo[10];
                //memset(bufferNuevo,10,0);
                //sprintf(bufferNuevo,"%d",velocidad);
                //trans(bufferNuevo);
                //trans("\n");
                guardar_EEPROM(velocidad);
                }
                else
                {
                   //trans("inv\n");
                }
            }
            else if (RCREG=='x') // Apaga alarma
            {
                PORTB=0x00;
                recibir =0;
            }
            else if (RCREG=='X') // Apaga alarma
            {
                PORTB=0x00;
                recibir =0;
            }
            else // me ingresaron cualquier cosa
            {
                recibir=0;
                //trans("Cinv\n\0");
            }
            i++;
            if (i>=32) {recibir=0;} // Reviso no llenar buffer
                    }
    }
}
