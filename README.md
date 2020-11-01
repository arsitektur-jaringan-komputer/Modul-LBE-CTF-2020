# Forensik
Forensik dalam dunia digital adalah seni untuk memulihkan jejak-jejak digital yang tertinggal di sebuah komputer. Dalam CTF sendiri, kategori forensik mencakup challenge-challenge yang bertujuan untuk mendapatkan suatu informasi tersembunyi dengan cara memeriksa dan memproses sebuah data statis.

Salah satu hal penting dalam pengerjaan challenge forensik adalah penggunaan **tools** yang tepat. Selain itu adalah pengalaman (lol).

Beberapa hal dasar yang perlu dikenali dalam forensik CTF adalah:
+ [Format File dan Data EXIF / metadata](#format-file-dan-data-exif)
+ [Captured network traffic](#captured-network-traffic)
+ [Steganografi](#steganografi)

### Format File dan Data EXIF
Exchangeable Image File Format atau biasa disebut Exif adalah sebuah standar untuk data-data tambahan yang berada pada sebuah file atau foto. Data-data ini bisa berupa banyak hal, seperti: tipe dari sebuah file, kapan dan dimana sebuah foto diambil, kecepatan shutter kamera yang digunakan, apakah file tersebut pernah diubah, dan informasi-informasi lainnya.

Dalam forensik, informasi-informasi tersebut dapat memuat clue dari tujuan akhir kita (di sini tentu untuk mendapatkan flag).

Selain menggunakan exiftool, tipe dari sebuah file juga dapat dilihat dari **magic byte** yang dimilikinya. Magic byte sendiri adalah beberapa byte pada sebuah file yang menunjukkan tipe dari sebuah file. Magic byte ini biasanya berada di awal file dan memiliki panjang 2-4 bytes.

![Contoh metadata](images/exifdata.JPG)

##### Tools
+ exiftools (`apt-get install exiftool`)
+ http://exif.regex.info/exif.cgi
+ hexdump (linux command)
+ xxd (linux command)
+ hxd (Windows)
+ file (linux command)

### Captured Network traffic
Seperti namanya, pada tipe soal ini biasanya kalian akan diberikan file **.pcapng**. File tersebut merupakan traffic dari sebuah network yang berhasil ditangkap. Kalian akan diminta untuk mencari flag-nya di sana.

Flag bisa didapatkan se-mudah mencari string biasa ataupun harus mendapatkan suatu objek terlebih dahulu. Dimana flag-nya bisa didapatkan dari objek tersebut.

![Tampilan wireshark](images/pcapng.JPG)

##### Tools
+ Wireshark

### Steganografi
Steganografi adalah sebuah seni penyembunyian data dari mata telanjang. Pesan yang ingin disampaikan tidak dapat dilihat secara sekilas. Dibutuhkan ketelitian, pemeriksaan, dan sedikit pengubahan dari cara kita melihat media tersebut untuk bisa mendapatkan pesan yang sesungguhnya.

Challenge-challenge steganografi biasanya berhubungan dengan media (gambar, audio, dan sejenisnya) sehingga dibutuhkan tools yang dapat membaca maupun mengolah jenis-jenis file tersebut

![Contoh steganografi](contoh/stegano.png)

##### Tools
+ [Audacity](https://www.audacityteam.org/download/) (audio)
+ Gimp/Photoshop dan sejenisnya

### Tools berguna lainnya
+ binwalk (`apt-get install binwalk`)
+ foremost (`apt-get install foremost`)

### Coba-coba time
1. Dapatkan flag dari [file ini](contoh/potato_lucu.pdf)!
2. Sebutkan nama kota [foto ini](contoh/kota_apa_hayo.jpg) berasal!
3. Hayo keliatan nggak flag-nya [di sini](contoh/stegano.png)?
4. Yok dicoba [ke sini](contoh/sound.wav) kayak tadi yang dicontohin!
5. Yang jeli ya liat [ininya](contoh/video.avi)!
6. Bentuk asli [.docx](contoh/secret_doc.docx) itu sebenernya apasih?
7. Clue-nya adalah "[abu-abu tua](contoh/abu-abuGelap.pcapng)" :)

### Referensi
+ [CTF 101](https://ctf101.org/forensics/overview/)
+ [Trail of bits](https://trailofbits.github.io/ctf/forensics/)
+ [Awesome CTF for awesome tools](https://github.com/apsdehal/awesome-ctf/blob/master/README.md)
