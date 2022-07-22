/********************************** 
 * Luiz Brito da Rosa
 * Univali - Engenharia Eletrica 
 * Microcontroladores
 * Trabalho 002 
 * LCD
 **********************************/
#include <xc.h>

#include "config.h"

#define _XTAL_FREQ 8000000

int i, u, d = 0; 

void inicializaLCD(void);
void comandosLCD(char cmd);
void enviaDadosLCD(char nivel, char dado);

void __interrupt(low_priority) int_TMR0(void){

	if (TMR0IF) {

        if (i <100) {
            i++;
        } else{
            i = 0;
        }

		//Retorna o valor inicial para o registro TMR
        TMR0 = 57724;

        //Zera a flag da interrupcao (T0IF)
        TMR0IF = 0; 
        
     }
}
	
void main(void) {
    OSCCON=0b01111110; 
	INTCON=0b10100000;
    T0CON=0b10000111;

    //Desabilita A/D
    ADCON1=0x0F;
    
    /******************* Firula ****/
    inicializaLCD();
      
   //PIC18F4520
    enviaDadosLCD(1, 'V');
    enviaDadosLCD(1, 'a');
    enviaDadosLCD(1, 'l');
    enviaDadosLCD(1, 'e');
    enviaDadosLCD(1, 'u');
    enviaDadosLCD(1, ' ');
    enviaDadosLCD(1, 'P');
    enviaDadosLCD(1, 'r');
    enviaDadosLCD(1, 'o');
    enviaDadosLCD(1, 'f');
    
    __delay_ms(3000);
    
    i = 0;

    /******************* Fim da Firula ****/

    //Seta o valor inicial no registrador TMR0
    TMR0=57724;

    //Zera a flag da interrupcao (T0IF)
    TMR0IF = 0; 
    
    
    // Agora o trabalho
    
    inicializaLCD();

    while(1) {
        u = i%10;

	    d = i%100;

        d = (d/10) - ((d%10 / 10));

        enviaDadosLCD(0, 0x83);

        enviaDadosLCD(1, d+48);

        enviaDadosLCD(1, u+48);

        enviaDadosLCD(0, 0xC3);
    }
}

void inicializaLCD(void) {
    char i;
    
    //Seta todos os pinos da porta D como saida 
    TRISD = 0b00000000;

    //Seta a porta D com o nivel logico 0 
    LATD = 0b00000000;       
    
    //Seta o pino 0 da porta E como saida 
    TRISEbits.TRISE0 = 0;
    
    //Seta o pino 1 da porta E como saida 
    TRISEbits.TRISE1 = 0;
    
    //Seta o pino 0 da porta E com o nivel logico 0
    LATEbits.LATE0 = 0;

    //Seta o pino 1 da porta E com o nivel logico 0
    LATEbits.LATE1 = 0;
   
    __delay_ms(15);
   
    for(i = 0; i < 3; i++) {

       comandosLCD(0x30);

       __delay_ms(5);

    }

    //Seta a configuracao que serah 8bits e com 2 linhas
    comandosLCD(0x38);
    __delay_us(50);

    //Zera a memoria 
    comandosLCD(0x01);
    __delay_ms(4);

    //Liga o LCD, vazio 
    comandosLCD(0x0C);
    __delay_us(50);

    //Desloca o cursor a direita apohs adicao de um novo caracter
    comandosLCD(0x06);
    __delay_us(40);
}

void comandosLCD(char comando) {
    // passa para a porta D o valor passado por parametro
    LATD = comando;

    //Seta 1 no pino 1 da porta E
    LATEbits.LATE1 = 1; 

    //Seta 0 no pino 1 da porta E
    LATEbits.LATE1 = 0;
}

void enviaDadosLCD(char nivel, char dados) {
     //Habilita comando ou leitura para o LCD
    LATEbits.LATE0 = nivel; 

    __delay_us(100);

    //Seta 0 no pino 1 da porta E
    LATEbits.LATE1 = 0;

    comandosLCD(dados);
}