Link Figma:
https://www.figma.com/design/FH6BTk9658nawBTz6TouQH/SP3S?node-id=0-1&m=dev&t=tKUMzKTZXveKOU6v-1


## Alur Kerja
1. Pengguna membuka aplikasi SP3S pada smartphone atau desktop. 
2. Halaman login/connection akan ditampilkan dengan informasi singkat mengenai fungsi SP3S serta ilustrasi sistem kompor pintar. 
3. Pengguna memasukkan alamat IP ESP8266 yang terhubung dengan perangkat SP3S pada kolom yang tersedia. 
4. Pengguna menekan tombol "Sambungkan" untuk melakukan koneksi dengan sistem monitoring kompor pintar. 
5. Sistem melakukan proses validasi koneksi ke ESP8266. 
   -Jika koneksi berhasil, pengguna akan diarahkan ke   halaman dashboard.  -Jika koneksi gagal, sistem akan menampilkan pesan kesalahan dan pengguna diminta memeriksa alamat IP atau jaringan yang digunakan. 
6. Pada halaman dashboard, pengguna dapat memantau kondisi sistem secara real-time, meliputi: 
   - Status gas LPG (Aman, Warning, atau Bahaya). 
   - Suhu area kompor. 
   - Status valve gas. 
   - Status alarm. 
   - Status koneksi WiFi. 
7. Sistem secara otomatis memperbarui data sensor yang diperoleh dari sensor MQ-6 dan DHT11 sehingga pengguna dapat mengetahui kondisi kompor tanpa perlu melakukan refresh secara manual.
8. Pengguna dapat melihat riwayat kejadian terbaru yang berisi catatan kondisi sistem, seperti status aman, warning, atau bahaya beserta informasi suhu yang terdeteksi pada saat kejadian. 
9. Jika diperlukan, pengguna dapat memilih menu "Lihat Semua" untuk melihat riwayat monitoring yang lebih lengkap sebagai bahan evaluasi dan pemantauan keamanan penggunaan kompor. 
10. Setelah selesai melakukan monitoring, pengguna dapat menutup aplikasi atau kembali melakukan pengecekan kondisi sistem kapan saja selama perangkat SP3S terhubung ke jaringan
