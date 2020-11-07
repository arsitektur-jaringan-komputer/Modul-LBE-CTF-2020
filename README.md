# Binary Exploitation
Binary atau executables, adalah kode mesin yang dijalankan oleh komputer. Biasanya di dalam CTF, kategori Binary Exploitation disediakan dalam bentuk file ELF Linux atau Windows yang dapat dieksekusi. Tujuan dari Binary Exploitation adalah menemukan celah keamanan pada program dan mengeksploitasinya untuk melakukan berbagai hal, seperti memodifikasi fungsi dalam program, mengganti nilai suatu variabel, mendapatkan kendali atas shell, dsb.

Beberapa hal yang ada dalam binary exploitation meliputi:

## Registers
## The Stack
## Calling Conventions
## Global Offset Table (GOT)
Global Offset Table (GOT) adalah bagian program yang menyimpan alamat fungsi yang terhubung secara dinamis. Seperti yang disebutkan di bagian Calling Conventions, kebanyakan program tidak menyertakan setiap fungsi yang mereka gunakan untuk mengurangi ukuran biner. Sebaliknya, fungsi umum (seperti yang ada di libc) "dilink" ke dalam program sehingga dapat disimpan sekali di disk dan digunakan kembali oleh setiap program.

Semua dynamic library diload ke memory bersamaan saat program diluncurkan, tapi fungsi tidak dipetakan ke kode sampai saat pertama kali mereka dipanggil. Misalnya, dalam kode C berikut, put tidak akan diselesaikan ke alamat di libc sampai setelah dipanggil sekali:

```C
int main() {
    puts("Hi there!");
    puts("Ok bye now.");
    return 0;
}
```

Untuk menghindari pencarian melalui shared library setiap kali suatu fungsi dipanggil, hasil pencarian disimpan ke dalam GOT sehingga fungsi yang akan datang memanggil "sirkuit pendek" langsung ke implementasinya dengan melewati resolver dinamis.

Ini menyebabkan dua implikasi penting:
1. GOT berisi pointer ke pustaka yang berpindah karena ASLR
2. GOT dapat ditulis

Kedua fakta ini akan menjadi sangat berguna untuk digunakan dalam Return Oriented Programming.

## Buffers
Buffer adalah setiap ruang yang dialokasikan dalam memori di mana data (seringkali dari input pengguna) dapat disimpan. Misalnya, dalam nama program C berikut ini akan dianggap sebagai stack buffer:

```C
#include <stdio.h>

int main() {
    char name[64] = {0};
    read(0, name, 63);
    printf("Hello %s", name);
    return 0;
}
```

Buffer juga bisa berupa variabel global.

```C
#include <stdio.h>

char name[64] = {0};

int main() {
    read(0, name, 63);
    printf("Hello %s", name);
    return 0;
}
```

Atau dialokasikan secara dinamis di dalam heap.

```C
#include <stdio.h>
#include <stdlib.h>

int main() {
    char *name = malloc(64);
    memset(name, 0, 64);
    read(0, name, 63);
    printf("Hello %s", name);
    return 0;
}
```

#### Exploit
Mengingat bahwa buffer biasanya menampung masukan pengguna, kesalahan saat menulis ke dalam buffer dapat mengakibatkan data yang ditulis penyerang ditulis di luar ruang buffer.

### Buffer Overflow
Buffer Overflow adalah vulnerability di mana data dapat ditulis melebihi ruang yang dialokasikan, memungkinkan penyerang untuk menimpa data lain.

#### Stack Buffer Overflow
Buffer overflow yang paling sederhana dan umum adalah buffer yang ada di tumpukan. Mari kita lihat contohnya.

```C
#include <stdio.h>

int main() {
    int secret = 0xdeadbeef;
    char name[100] = {0};
    read(0, name, 0x100);
    if (secret == 0x1337) {
        puts("Wow! Here's a secret.");
    } else {
        puts("I guess you're not cool enough to see my secret");
    }
}
```

Ada kesalahan kecil dalam program ini yang memungkinkan kita untuk melihat rahasianya. name adalah desimal 100 byte, namun di sini dibaca dalam hex 100 byte (= 256 byte desimal). Mari kita lihat bagaimana kita dapat menggunakan ini untuk keuntungan kita.

Jika compiler memilih untuk mengatur susunan seperti ini:
```
        0xffff006c: 0xf7f7f7f7  // Saved EIP
        0xffff0068: 0xffff0100  // Saved EBP
        0xffff0064: 0xdeadbeef  // secret
...
        0xffff0004: 0x0
ESP ->  0xffff0000: 0x0         // name
```

mari kita lihat apa yang terjadi ketika kita membaca 'A' dalam 0x100 byte.

Desimal pertama 100 byte disimpan dengan benar:
```
        0xffff006c: 0xf7f7f7f7  // Saved EIP
        0xffff0068: 0xffff0100  // Saved EBP
        0xffff0064: 0xdeadbeef  // secret
...
        0xffff0004: 0x41414141
ESP ->  0xffff0000: 0x41414141  // name
```

Namun ketika byte ke-101 dibaca, akan ada masalah:
```
        0xffff006c: 0xf7f7f7f7  // Saved EIP
        0xffff0068: 0xffff0100  // Saved EBP
        0xffff0064: 0xdeadbe41  // secret
...
        0xffff0004: 0x41414141
ESP ->  0xffff0000: 0x41414141  // name
```

Least significant byte dari secret akan ditimpa. Jika kita mengikuti 3 byte berikutnya untuk dibaca, kita akan melihat keseluruhan secret "terhalang" dengan 'A' kita.

152 byte yang tersisa akan terus memanjat nilai tumpukan.

##### Pengecekan
Bagaimana kita bisa menggunakan ini untuk melewati pemeriksaan yang tampaknya tidak mungkin dalam program aslinya? Nah, jika kita dengan hati-hati menyusun masukan kita sehingga byte yang menimpa rahasia kebetulan adalah byte yang mewakili 0x1337 di little-endian, kita akan melihat pesan rahasianya.

Satu baris Python kecil akan bekerja dengan baik: python -c "print 'A' * 100 + '\ x31 \ x13 \ x00 \ x00'"

Ini akan mengisi buffer nama dengan 100 'A, lalu menimpa rahasia dengan encoding little-endian 32-bit dari 0x1337.

##### Hal Lain
Seperti yang dibahas di halaman tumpukan, instruksi yang harus dilompati oleh fungsi saat ini ketika selesai juga disimpan di tumpukan (dilambangkan sebagai "EIP Tersimpan" dalam diagram tumpukan di atas). Jika kita dapat menimpa ini, kita dapat mengontrol ke mana program melompat setelah selesai utama berjalan, memberi kita kemampuan untuk mengontrol apa yang dilakukan program sepenuhnya.

Biasanya, tujuan akhir dalam eksploitasi biner adalah untuk mendapatkan shell (sering disebut "popping a shell") di komputer jarak jauh. Shell memberi kita cara mudah untuk menjalankan apa pun yang kita inginkan di komputer target.

Katakanlah kebetulan ada fungsi bagus yang melakukan ini didefinisikan di tempat lain dalam program yang biasanya tidak bisa kita akses:
```C
void give_shell() {
    system("/bin/sh");
}
```

Nah dengan pengetahuan buffer overflow kita, sekarang kita bisa! Yang harus kita lakukan adalah menimpa EIP yang disimpan di stack ke alamat tempat give_shell berada. Kemudian, ketika main kembali, itu akan mengeluarkan alamat itu dari tumpukan dan melompat ke sana, menjalankan give_shell, dan memberi kita shell kita.

Dengan asumsi give_shell berada di 0x08048fd0, kita dapat menggunakan sesuatu seperti ini: python -c "print 'A' * 108 + '\ xd0 \ x8f \ x04 \ x08'"

Kami mengirim 108 'A untuk menimpa 100 byte yang dialokasikan untuk nama, 4 byte untuk rahasia, dan 4 byte untuk EBP yang disimpan. Kemudian kami cukup mengirim bentuk little-endian dari alamat give_shell, dan kami akan mendapatkan shell!

## Sumber
- https://ctf101.org/binary-exploitation/overview/