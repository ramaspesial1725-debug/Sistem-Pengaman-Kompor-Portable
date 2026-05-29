# SP3S - Smart Portable Stove Safety System
# Rancang Bangun Sistem Pengaman Otomatis Kebocoran Gas dan Overheat pada Kompor Portable

## Deskripsi Proyek
Proyek ini merupakan sistem keamanan otomatis pada kompor portable yang dirancang untuk mendeteksi kebocoran gas LPG dan kondisi overheat pada tabung gas secara real-time. Sistem menggunakan sensor gas MQ-6 untuk mendeteksi kebocoran gas serta thermistor NTC 100K untuk memonitor suhu pada area tabung gas.

Apabila terdeteksi kebocoran gas atau suhu melebihi batas aman, sistem akan:
- Mengaktifkan alarm buzzer
- Menutup aliran gas secara otomatis menggunakan servo motor
- Memberikan monitoring kondisi sistem secara real-time

Dengan adanya sistem ini, diharapkan risiko kebakaran dan ledakan akibat kebocoran gas maupun overheat pada kompor portable dapat diminimalkan.

---

## Tujuan Proyek
- Meningkatkan keamanan penggunaan kompor portable
- Mendeteksi kebocoran gas LPG secara otomatis
- Mendeteksi suhu berlebih (overheat) pada tabung gas
- Meminimalkan risiko kebakaran dan ledakan
- Mengembangkan sistem monitoring keamanan berbasis mikrokontroler

---

## 👨‍💻 Tim Pengembang

| No | Nama | NRP | Tugas |
|----|------|------|------|
| 1 | Gilang Ramadhan | 2124600001 | 📌 Project Manager |
| 2 | Rico Ivanada Y.P | 2124600013 | 🎨 UI/UX Designer |
| 3 | Ardika Purna Atmaja | 2124600003 | 💻 Software Engineer |
| 4 | Andrey Khusuma Prasetya | 2124600006 | 🧩 3D Designer |
| 5 | Siti Nur Fadilla | 2124600029 | 🎨 UI/UX Designer |
| 6 | Hanif Agustama Ikhsan | 2124600027 | 🔧 Hardware Engineer |

---

## Fitur Sistem
- Deteksi kebocoran gas LPG menggunakan sensor MQ-6
- Monitoring suhu kompor menggunakan sensor LM35
- Alarm buzzer otomatis saat kondisi bahaya
- Pemutus aliran gas otomatis menggunakan selenoid
- Monitoring kondisi sistem secara real-time (LCD & APK Web Base)
- Sistem keamanan otomatis berbasis Arduino Mega2560

---

## Komponen yang Digunakan
- Arduino Mega2560
- ESP8266 ESP-01 Wi-Fi Module
- Sensor Gas MQ-6
- Sensor Suhu DHT11
- Relay
- Selenoid
- Buzzer
- Baterai 18650
- Kabel Jumper
- Breadboard / PCB
- Regulator Tegangan

---

## Cara Kerja Sistem
1. Sensor MQ-6 mendeteksi kadar gas LPG di sekitar kompor portable.
2. Sensor Suhu LM35 memonitor suhu tabung gas secara real-time.
3. Data sensor diproses oleh Arduino.
4. Jika terdeteksi kebocoran gas atau suhu melebihi batas aman:
   - Buzzer akan aktif sebagai alarm peringatan
   - Servo motor akan menutup aliran gas secara otomatis
5. Sistem terus melakukan monitoring dengan mengirimkan data sensor secara real-time ke aplikasi yang terhubung dengan perangkat pengguna untuk menjaga keamanan pengguna.

---

## Diagram Sistem
> Tambahkan gambar diagram sistem di sini

