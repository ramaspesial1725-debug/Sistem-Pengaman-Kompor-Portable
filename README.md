# SP3S - Smart Portable Stove Safety System
# Rancang Bangun Sistem Pengaman Otomatis Kebocoran Gas dan Overheat pada Kompor Portable
![SP3S](https://github.com/ramaspesial1725-debug/Sistem-Pengaman-Kompor-Portable/blob/main/Assets/Background%20Proyek.png)

## Deskripsi Proyek
Proyek ini merupakan sistem keamanan otomatis pada kompor portable yang dirancang untuk mendeteksi kebocoran gas LPG dan kondisi overheat pada tabung gas secara real-time. Sistem menggunakan sensor gas MQ-6 untuk mendeteksi kebocoran gas serta sensor DHT-11 untuk memonitor suhu pada area tabung gas.

Apabila terdeteksi kebocoran gas atau suhu melebihi batas aman, sistem akan:
- Mengaktifkan alarm buzzer
- Menutup aliran gas secara otomatis menggunakan solenoid
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
![Tim Pengembang](https://github.com/ramaspesial1725-debug/Sistem-Pengaman-Kompor-Portable/blob/main/Assets/Foto%20Tim/Tim.jpg)

| No | Nama | NRP | Tugas | Akun |
|----|------|------|------|------|
| 1 | Gilang Ramadhan | 2124600001 | 📌 Project Manager | https://github.com/ramaspesial1725-debug |
| 2 | Rico Ivanada Y.P | 2124600013 | 🎨 UI/UX Designer | https://github.com/ricoivanadayp |
| 3 | Ardika Purna Atmaja | 2124600003 | 💻 Software Engineer | https://github.com/ArdikaPA |
| 4 | Andrey Khusuma Prasetya | 2124600006 | 🧩 3D Designer | https://github.com/andreykhusuma30-ai |
| 5 | Siti Nur Fadilla | 2124600029 | 🎨 UI/UX Designer | https://github.com/dillaf836 |
| 6 | Hanif Agustama Ikhsan | 2124600027 | 🔧 Hardware Engineer | https://github.com/hanifikhsan13 |

---

## Fitur Sistem
- Deteksi kebocoran gas LPG menggunakan sensor MQ-6
- Monitoring suhu kompor menggunakan sensor DHT-11
- Alarm buzzer otomatis saat kondisi bahaya
- Pemutus aliran gas otomatis menggunakan selenoid
- Monitoring kondisi sistem secara real-time (LCD & APK Web Base)
- Sistem keamanan otomatis berbasis Arduino Mega2560

---

## Komponen yang Digunakan
- Arduino Mega2560
- ESP8266 ESP-01 Wi-Fi Module
- Sensor Gas MQ-6
- Sensor Suhu DHT-11
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
2. Sensor Suhu DHT-11 memonitor suhu tabung gas secara real-time.
3. Data sensor diproses oleh Arduino.
4. Jika terdeteksi kebocoran gas atau suhu melebihi batas aman:
   - Buzzer akan aktif sebagai alarm peringatan
   - Solenoid akan menutup aliran gas secara otomatis
5. Sistem terus melakukan monitoring dengan mengirimkan data sensor secara real-time ke aplikasi yang terhubung dengan perangkat pengguna untuk menjaga keamanan pengguna.

---

## Diagram Sistem
![Diagram Sistem](https://github.com/ramaspesial1725-debug/Sistem-Pengaman-Kompor-Portable/blob/main/Blok%20Diagram/Diagram%20System%20Architecture.jpg)

---

## Hardware

---

## Design 3D

---

## Simulasi

Video Simulasi:


https://github.com/user-attachments/assets/034c19e1-1574-49c7-9b3f-ac5ec29b052b



Link Wokwi:
https://wokwi.com/projects/465650630948624385

---

## Design UI/UX
Link Figma: https://www.figma.com/design/FH6BTk9658nawBTz6TouQH/SP3S?node-id=0-1&m=dev&t=tKUMzKTZXveKOU6v-1

1. Tampilan Handphone
   
![Design UI/UX](https://github.com/ramaspesial1725-debug/Sistem-Pengaman-Kompor-Portable/blob/main/Design%20UI%20UX/Tampilan%20Handphone%201.png)

2. Tampilan Dekstop

![Design UI/UX](https://github.com/ramaspesial1725-debug/Sistem-Pengaman-Kompor-Portable/blob/main/Design%20UI%20UX/TAMPILAN%20DEKSTOP%20FIGMA%201.PNG.jpeg)

---

## PPT
Link Canva: https://canva.link/frm59njammdw9kd

---

## Iklan Produk
<video src="https://github.com/user-attachments/assets/f96aa613-9f6f-4be7-9b30-682074369cd5" controls="controls" width="100%"></video

---


