# Cryptography

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/crypto%20meme.png)

**Crypto** : menyembunyikan,

**Graphy** : tulisan

=> Teknik yang mengenkripsi plain text menjadi non-readable text.

Cryptography adalah ilmu yang berhubungan dengan aspek keamanan informasi yang memiliki 4 tujuan : kerahasiaan data, keabsahan data, integritas data, serta autentikasi data.

Dalam konteks CTF, goalnya adalah untuk mencari dan memecahkan algoritma cryptography dan mendapatkan flag.

4 Komponen Utama Cryptography :
1. Plaintext, yaitu pesan yang dapat dibaca
2. Ciphertext, yaitu pesan acak yang tidak dapat dibaca
3. Key, yaitu kunci untuk melakukan teknik kriptografi
4. Algorithm, yaitu metode untuk melakukan enkrispi dan dekripsi

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/encrypt-decrypt.PNG)

## Symmetric Encryption
Digunakan untuk menyembunyikan konten blok atau stream data dengan berbagain ukuran, termasuk pesan, file, encryption key, dan password.

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/symmetric.gif)

## Asymmetric Encryption
Digunakan untuk menyembunyikan blok data kecil, seperti encryption key dan nilai fungsi hash yang digunakan dalam tanda tangan digital.

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/asymmetric.png)

## Teknik Substitusi
Teknik dimana huruf pada plain text dihganti/disubstitusi dengan huruf atau angka atau simbol lain.

https://www.net-force.nl/challenge/level302/

### 1. Caesar Cipher
Pada caesar cipher ini menggeser serangkaian huruf pada posisi tertentu ke kanan atau ke kiri bergantung pada key yang digunakan.

Misal : key di shift ke kanan sebanyak 7

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/CAESAR%207%20RIGHT.PNG)

Plaintext : HELLO WORLD
Cipher     : AXEEH PHKEW

### 2. Playfair cipher
Diciptakan oleh Sir Charles Wheatstone tahun 1854 menggunakan matrix 5x5 yang dikonstruksi berdasarkan sebuah keyword.

Misal : keyword FOOTBALL

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/playfair.PNG)

Plaintext : Hello World => HE LX LO WO RL DX

Cipher     : ML DV CF OC PD KT

### 3. Vigenere Cipher
Pada vigenere cipher, dibutuhkan key yang memiliki panjang yang sama dengan panjang pesan.

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/vigenere.png)

Misal : keyword ASUS

asusasusasusasusasusa		=> keyword

wearediscoverrunplanb	=> plaintext

wwujevckcgpwrjofpdufb	=> ciphertext

https://www.net-force.nl/challenge/level308/

## Teknik Transposisi

### 1. Rail Fence Cipher
Transposition cipher yang paling simpel. Plaintext dituliskan dalam bentuk diagonal kemudian dibaca dalam urutan baris.

Contoh : we are discover run plan b

Depth 4

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/cipher%20rail.PNG)

Ciphertext : WIRAEDSERLNAECVUPBRON

### 2. Row Transposition Cipher
Transposition cipher yang lebih kompleks.

![](https://github.com/arsitektur-jaringan-komputer/Modul-LBE-CTF-2020/blob/cryptography/img/row%20transposition.PNG)

Ciphertext : EIEPWRCRAYWDVNBEOUNZASRLX
