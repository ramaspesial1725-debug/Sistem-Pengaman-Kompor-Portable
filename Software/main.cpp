#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>

// --- KONFIGURASI AMBANG BATAS ---
#define THRESHOLD_GAS  400
#define THRESHOLD_SUHU 60.0

// --- DRIVER UART0 (Komunikasi ke PC / Debugging) ---
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

// --- DRIVER UART1 (Komunikasi ke ESP8266) ---
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

// --- DRIVER ADC (Sensor MQ-6 & LM35) ---
void ADC_Init() {
    ADMUX = (1 << REFS0); // Tegangan referensi AVCC (5V)
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

uint16_t ADC_Read(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
    ADCSRA |= (1 << ADSC); // Mulai konversi
    while (ADCSRA & (1 << ADSC)); // Tunggu selesai
    return ADC;
}

// --- DRIVER LCD 16x2 (MODE 4-BIT) ---
void LCD_Cmd(unsigned char cmd) {
    PORTD = (PORTD & 0xC0) | ((cmd >> 4) & 0x3C); // Kirim 4-bit MSB ke PD2-PD5
    PORTD &= ~(1 << PD0); // RS = 0 (Command)
    PORTD |= (1 << PD1);  // EN = 1
    _delay_us(1);
    PORTD &= ~(1 << PD1); // EN = 0
    _delay_us(200);

    PORTD = (PORTD & 0xC0) | (cmd & 0x3C);        // Kirim 4-bit LSB ke PD2-PD5
    PORTD |= (1 << PD1);  // EN = 1
    _delay_us(1);
    PORTD &= ~(1 << PD1); // EN = 0
    _delay_ms(2);
}

void LCD_Char(unsigned char data) {
    PORTD = (PORTD & 0xC0) | ((data >> 4) & 0x3C); // Kirim 4-bit MSB
    PORTD |= (1 << PD0);  // RS = 1 (Data)
    PORTD |= (1 << PD1);  // EN = 1
    _delay_us(1);
    PORTD &= ~(1 << PD1); // EN = 0
    _delay_us(200);

    PORTD = (PORTD & 0xC0) | (data & 0x3C);        // Kirim 4-bit LSB
    PORTD |= (1 << PD1);  // EN = 1
    _delay_us(1);
    PORTD &= ~(1 << PD1); // EN = 0
    _delay_ms(2);
}

void LCD_Init() {
    DDRD |= 0x3F; // Set PD0-PD5 sebagai Output data & kontrol LCD
    _delay_ms(20);
    
    LCD_Cmd(0x02); // Mode 4-bit
    LCD_Cmd(0x28); // 2 Baris, matriks 5x7
    LCD_Cmd(0x0C); // Display ON, Cursor OFF
    LCD_Cmd(0x06); // Shift ke kanan
    LCD_Cmd(0x01); // Clear Screen
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

// --- FUNGSI AT-COMMAND ESP8266 (Koneksi WiFi & Web Server) ---
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
    _delay_ms(7000); // Beri waktu proses autentikasi router
}

void ESP_SendDataKeWeb(int gas, float suhu, const char* status) {
    char dataJSON[128];
    char httpPacket[350];
    char cmdKirim[32];
    
    // 1. Payload Data format JSON
    sprintf(dataJSON, "{\"gas\":%d,\"suhu\":%.1f,\"status\":\"%s\"}", gas, (double)suhu, status);
    //sprintf(dataJSON, "{\"gas\":%d,\"suhu\":%d.%d,\"status\":\"%s\"}", 
        //gas, (int)suhu, (int)(suhu * 10) % 10, status);
    int panjangData = strlen(dataJSON);
    
    // 2. Buka Koneksi TCP ke Web Server (Ganti IP & Port sesuai server tim kamu!)
    UART1_SendString("AT+CIPSTART=\"TCP\",\"192.168.1.10\",80\r\n");
    _delay_ms(1000);
    
    // 3. Susun Paket HTTP POST Manual
    sprintf(httpPacket, 
            "POST /api/update-sensor HTTP/1.1\r\n"
            "Host: 192.168.1.10\r\n"
            "Content-Type: application/json\r\n"
            "Content-Length: %d\r\n\r\n"
            "%s", panjangData, dataJSON);
    
    // 4. Perintah kirim jumlah byte
    sprintf(cmdKirim, "AT+CIPSEND=%d\r\n", strlen(httpPacket));
    UART1_SendString(cmdKirim);
    _delay_ms(500);
    
    // 5. Kirim data string HTTP
    UART1_SendString(httpPacket);
    _delay_ms(1000);
    
    // 6. Tutup Koneksi TCP
    UART1_SendString("AT+CIPCLOSE\r\n");
}

// --- MAIN PROGRAM ---
int main(void) {
    // Inisialisasi Output Fisik:
    // PB0 = Solenoid Valve (via Transistor/Relay)
    // PB1 = Buzzer
    DDRB |= (1 << DDB0) | (1 << DDB1);
    
    // Inisialisasi Sistem Periferal
    UART0_Init(9600);     // PC Debugging
    UART1_Init(115200);   // Komunikasi WiFi ESP8266
    ADC_Init();           // Pembacaan Analog Sensor
    LCD_Init();           // Layar Monitor Karakter
    
    char bufferLCD[16];
    
    // Splash Screen Pembuka di LCD
    LCD_String("  SYSTEM SP3S  ");
    LCD_SetCursor(1, 0);
    LCD_String("  INITIALIZING ");
    
    // Ganti dengan SSID Hotspot HP/Router dan Password tim kamu!
    ESP_Init("Nama_WiFi_Kamu", "Password_WiFi_Kamu");
    
    LCD_Cmd(0x01); // Clear screen setelah setup selesai
    UART0_SendString("SP3S System Online & Running!\r\n");

    while (1) {
        // ================= 1. BACA DATA SENSOR =================
        // MQ-6 terhubung ke pin A0 (ADC0)
        uint16_t nilaiGas = ADC_Read(0); 
        
        // LM35 terhubung ke pin A1 (ADC1)
        uint16_t adcSuhu = ADC_Read(1);
        float millivolt = (adcSuhu / 1023.0) * 5000.0;
        float nilaiSuhu = millivolt / 10.0; // 10mV = 1 Derajat Celcius
        
        // ================= 2. PRINT KE LAYAR LCD 16x2 =================
        LCD_SetCursor(0, 0);
        // Menampilkan data Gas dan Suhu secara real-time di baris ke-1
        sprintf(bufferLCD, "G:%04d S:%02d.%01dC ", nilaiGas, (int)nilaiSuhu, (int)(nilaiSuhu*10)%10);
        LCD_String(bufferLCD);
        
        // ================= 3. LOGIKA KEPUTUSAN & TINDAKAN =================
        LCD_SetCursor(1, 0);
        const char* statusSistem;
        
        if (nilaiGas > THRESHOLD_GAS || nilaiSuhu >= THRESHOLD_SUHU) {
            // KONDISI BAHAYA! (Terjadi Kebocoran ATAU Overheat)
            PORTB |= (1 << PB0);  // Trigger Solenoid ON (Menarik tuas gas untuk memutus aliran)
            PORTB |= (1 << PB1);  // Buzzer Bunyi Nyaring berkelanjutan
            
            LCD_String("STATUS: BAHAYA! ");
            statusSistem = "BAHAYA";
            UART0_SendString("[PERINGATAN] Gas Bocor atau Overheat Terdeteksi!\r\n");
        } else {
            // KONDISI AMAN NORMAL
            PORTB &= ~(1 << PB0); // Solenoid Standby (Aliran Gas Terbuka)
            PORTB &= ~(1 << PB1); // Buzzer OFF
            
            LCD_String("STATUS: AMAN    ");
            statusSistem = "AMAN";
        }
        
        // ================= 4. KIRIM DATA KE WEBSITE VIA ESP =================
        ESP_SendDataKeWeb(nilaiGas, nilaiSuhu, statusSistem);
        
        _delay_ms(2000); // Sampling rate data diperbarui tiap 2 detik
    }
}