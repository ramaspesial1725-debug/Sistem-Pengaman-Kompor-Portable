#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

#define THRESHOLD_GAS  400
#define THRESHOLD_SUHU 60.0

// DRIVER UART0
void UART0_Init(unsigned long baud) {
    unsigned int ubrr = (F_CPU / 16 / baud) - 1;
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1 << TXEN0); 
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART0_Transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void UART0_SendString(const char* str) {
    while (*str) {
        UART0_Transmit(*str++);
    }
}

// DRIVER UART1 (ESP8266)
void UART1_Init(unsigned long baud) {
    unsigned int ubrr = (F_CPU / 16 / baud) - 1;
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;
    UCSR1B = (1 << TXEN1) | (1 << RXEN1); 
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
}

void UART1_Transmit(char data) {
    while (!(UCSR1A & (1 << UDRE1)));
    UDR1 = data;
}

void UART1_SendString(const char* str) {
    while (*str) {
        UART1_Transmit(*str++);
    }
}

// DRIVER ADC
void ADC_Init() {
    ADMUX = (1 << REFS0); // refVCC (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC); 
    while (ADCSRA & (1 << ADSC)); 
    return ADC;
}

// DRIVER LCD 16x2
void LCD_Cmd(unsigned char cmd) {
    PORTD = (PORTD & 0xC0) | ((cmd >> 4) & 0x3C); 
    PORTD &= ~(1 << PD0); 
    PORTD |= (1 << PD1);  
    _delay_us(1);
    PORTD &= ~(1 << PD1); 
    _delay_us(200);

    PORTD = (PORTD & 0xC0) | (cmd & 0x3C);       
    PORTD |= (1 << PD1);  
    _delay_us(1);
    PORTD &= ~(1 << PD1); 
    _delay_ms(2);
}

void LCD_Char(unsigned char data) {
    PORTD = (PORTD & 0xC0) | ((data >> 4) & 0x3C); 
    PORTD |= (1 << PD0);  
    PORTD |= (1 << PD1);  
    _delay_us(1);
    PORTD &= ~(1 << PD1); 
    _delay_us(200);

    PORTD = (PORTD & 0xC0) | (data & 0x3C);       
    PORTD |= (1 << PD1); 
    _delay_us(1);
    PORTD &= ~(1 << PD1); 
    _delay_ms(2);
}

void LCD_Init() {
    DDRD |= 0x3F; 
    _delay_ms(20);
    
    LCD_Cmd(0x02); 
    LCD_Cmd(0x28); 
    LCD_Cmd(0x0C);
    LCD_Cmd(0x06); 
    LCD_Cmd(0x01); 
    _delay_ms(2);
}

void LCD_String(const char* str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_SetCursor(char row, char col) {
    char pos = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Cmd(pos);
}

// AT-COMMAND ESP8266 
void ESP_Init(const char* ssid, const char* pass) {
    char cmd[128];
    
    UART0_SendString("Resetting ESP8266...\r\n");
    UART1_SendString("AT+RST\r\n");
    _delay_ms(3000);
    
    UART0_SendString("Setting Mode Station...\r\n");
    UART1_SendString("AT+CWMODE=1\r\n");
    _delay_ms(1000);
    
    UART0_SendString("Connecting to WiFi...\r\n");
    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pass);
    UART1_SendString(cmd);
    _delay_ms(7000); 
}

void ESP_SendDataKeWeb(int gas, float suhu, const char* status) {
    char dataJSON[128];
    char httpPacket[350];
    char cmdKirim[32];
    
    // Payload Data format JSON
    sprintf(dataJSON, "{\"gas\":%d,\"suhu\":%.1f,\"status\":\"%s\"}", gas, (double)suhu, status);
    //sprintf(dataJSON, "{\"gas\":%d,\"suhu\":%d.%d,\"status\":\"%s\"}", 
        //gas, (int)suhu, (int)(suhu * 10) % 10, status);                //jaga2 serial web error
    int panjangData = strlen(dataJSON);
    
    //Koneksi TCP Web Server
    UART1_SendString("AT+CIPSTART=\"TCP\",\"192.168.1.10\",80\r\n");
    _delay_ms(1000);
    
    sprintf(httpPacket, 
            "POST /api/update-sensor HTTP/1.1\r\n"
            "Host: 192.168.1.10\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n"
            "%s", panjangData, dataJSON);
    
    sprintf(cmdKirim, "AT+CIPSEND=%d\r\n", strlen(httpPacket));
    UART1_SendString(cmdKirim);
    _delay_ms(500);
    
    UART1_SendString(httpPacket);
    _delay_ms(1000);
    
    UART1_SendString("AT+CIPCLOSE\r\n");
}

int main(void) {
    DDRB |= (1 << DDB0) | (1 << DDB1);
    
    UART0_Init(9600);     
    UART1_Init(115200);   
    ADC_Init();           
    LCD_Init();          
    
    char bufferLCD[16];
    
    LCD_String("  SYSTEM SP3S  ");
    LCD_SetCursor(1, 0);
    LCD_String("  INITIALIZING ");
    
    ESP_Init("SP3S", "Password");
    
    LCD_Cmd(0x01); 
    UART0_SendString("SP3S System Online & Running!\r\n");

    while (1) {
        uint16_t nilaiGas = ADC_Read(0); 
      
        uint16_t adcSuhu = ADC_Read(1);
        float millivolt = (adcSuhu / 1023.0) * 5000.0;
        float nilaiSuhu = millivolt / 10.0; // 10mV = 1 Derajat Celcius
        
        LCD_SetCursor(0, 0);
    
        sprintf(bufferLCD, "G:%04d S:%02d.%01dC ", nilaiGas, (int)nilaiSuhu, (int)(nilaiSuhu*10)%10);
        LCD_String(bufferLCD);
        
        LCD_SetCursor(1, 0);
        const char* statusSistem;
        
        if (nilaiGas > THRESHOLD_GAS || nilaiSuhu >= THRESHOLD_SUHU) {
            PORTB |= (1 << PB0); 
            PORTB |= (1 << PB1);  
            
            LCD_String("STATUS: BAHAYA! ");
            statusSistem = "BAHAYA";
            UART0_SendString("[PERINGATAN] Gas Bocor atau Overheat Terdeteksi!\r\n");
        } else {
            PORTB &= ~(1 << PB0); 
            PORTB &= ~(1 << PB1); 
            
            LCD_String("STATUS: AMAN    ");
            statusSistem = "AMAN";
        }
        
        ESP_SendDataKeWeb(nilaiGas, nilaiSuhu, statusSistem);     
        _delay_ms(2000); 
    }
}
