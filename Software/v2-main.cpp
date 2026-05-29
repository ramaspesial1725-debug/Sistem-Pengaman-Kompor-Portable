#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define BATAS_GAS   400
#define BATAS_SUHU  60.0

void UART0_Init(unsigned long baud) {
    unsigned int ubrr = (F_CPU / 16 / baud) - 1;
    UBRR0H = (unsigned char)(ubrr >> 8); UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0); UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART0_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); UDR0 = data;
}

void UART0_SendString(const char* str) {
    while (*str) UART0_Transmit(*str++);
}

void ADC_Init() {
    ADMUX = (1 << REFS0); 
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void DHT_Init() {
    DDRH |= (1 << DDH6); PORTH |= (1 << PH6);  
}

uint8_t DHT_ReadData(float *temperature, float *humidity) {
    uint8_t bits[5] = {0, 0, 0, 0, 0};
    uint8_t i, j = 0;

    DDRH |= (1 << DDH6);  
    PORTH &= ~(1 << PH6); 
    _delay_ms(2);         
    PORTH |= (1 << PH6);  
    _delay_us(30);
    
    DDRH &= ~(1 << DDH6); 
    _delay_us(40);
    
    if (PINH & (1 << PINH6)) {
        return 0; 
    }
    _delay_us(80);
    
    if (!(PINH & (1 << PINH6))) {
        return 0; 
    }
    _delay_us(80);

    for (j = 0; j < 5; j++) {
        for (i = 0; i < 8; i++) {
            while (!(PINH & (1 << PINH6))); 
            _delay_us(40);                  
            if (PINH & (1 << PINH6)) { 
                bits[j] |= (1 << (7 - i)); 
                while (PINH & (1 << PINH6)); 
            }
        }
    }

    if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
        *humidity = ((bits[0] << 8) + bits[1]) * 0.1;
        *temperature = (((bits[2] & 0x7F) << 8) + bits[3]) * 0.1;
        if (bits[2] & 0x80) *temperature *= -1; 
        return 1; 
    }
    return 0; 
}

void LCD_PulseEnable() {
    PORTA |= (1 << PA1);  _delay_us(10);
    PORTA &= ~(1 << PA1); _delay_us(100);
}

void LCD_Write4Bit(unsigned char val) {
    PORTA = (PORTA & 0xC3) | ((val & 0x0F) << 2); 
    LCD_PulseEnable();
}

void LCD_Cmd(unsigned char cmd) {
    PORTA &= ~(1 << PA0); 
    LCD_Write4Bit(cmd >> 4);
    LCD_Write4Bit(cmd & 0x0F);
    _delay_ms(2);
}

void LCD_Char(unsigned char data) {
    PORTA |= (1 << PA0);  
    LCD_Write4Bit(data >> 4);
    LCD_Write4Bit(data & 0x0F);
    _delay_us(200);
}

void LCD_Init() {
    DDRA |= 0x3F; 
    _delay_ms(50);
    LCD_Write4Bit(0x03); _delay_ms(5);
    LCD_Write4Bit(0x03); _delay_us(150);
    LCD_Write4Bit(0x03); LCD_Write4Bit(0x02); 

    LCD_Cmd(0x28); LCD_Cmd(0x0C); LCD_Cmd(0x01); _delay_ms(2);
}

void LCD_String(const char* str) {
    while (*str) LCD_Char(*str++);
}

void LCD_SetCursor(char row, char col) {
    LCD_Cmd((row == 0) ? (0x80 + col) : (0xC0 + col));
}

int main(void) {
    DDRH |= (1 << DDH3) | (1 << DDH4) | (1 << DDH5);
    DDRB |= (1 << DDB4); 

    UART0_Init(9600);
    ADC_Init();
    DHT_Init();
    LCD_Init();
    
    char bufferLCD[16];
    char bufferUART[128];
    float nilaiSuhu = 0.0, kelembaban = 0.0;

    LCD_SetCursor(0, 0);  LCD_String("  SYSTEM SP3S  ");
    LCD_SetCursor(1, 0);  LCD_String("BOOTING...     ");
    _delay_ms(2000);
    LCD_Cmd(0x01);

    while (1) {
        uint16_t nilaiGas = ADC_Read(0);
        
        float tempSuhu, tempHum;
        if (DHT_ReadData(&tempSuhu, &tempHum)) {
            nilaiSuhu = tempSuhu; kelembaban = tempHum;
        }

        LCD_SetCursor(0, 0);
        sprintf(bufferLCD, "G:%04d T:%02d.%01dC ", nilaiGas, (int)nilaiSuhu, (int)(nilaiSuhu * 10) % 10);
        LCD_String(bufferLCD);
        
        LCD_SetCursor(1, 0);
        const char* statusSistem;
        
        if (nilaiGas > BATAS_GAS || nilaiSuhu >= BATAS_SUHU) {
            PORTB |= (1 << PB4);   
            PORTH |= (1 << PH5);   
            PORTH |= (1 << PH3);   
            PORTH &= ~(1 << PH4);  
            LCD_String("GAS BOCOR/HOT!!! ");
            statusSistem = "BAHAYA";
        } 
        else {
            PORTB &= ~(1 << PB4);  
            PORTH &= ~(1 << PH5);  
            PORTH &= ~(1 << PH3);  
            PORTH |= (1 << PH4);   
            LCD_String("AMAN            ");
            statusSistem = "AMAN";
        }
        
        sprintf(bufferUART, "{\"gas\":%d,\"suhu\":%d.%d,\"hum\":%d,\"status\":\"%s\"}\n", 
        nilaiGas, (int)nilaiSuhu, (int)(nilaiSuhu * 10) % 10, (int)kelembaban, statusSistem);
        UART0_SendString(bufferUART);
        
        _delay_ms(1000); 
    }
}
