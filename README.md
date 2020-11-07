tulisan ini adalah hasil translasi dari artikel: https://ctf101.org/binary-exploitation/overview/
# Binary Exploitation

Binary atau executables, adalah kode mesin yang dijalankan oleh komputer. Biasanya di dalam CTF, kategori Binary Exploitation disediakan dalam bentuk file ELF Linux atau Windows yang dapat dieksekusi. Tujuan dari Binary Exploitation adalah menemukan celah keamanan pada program dan mengeksploitasinya untuk melakukan berbagai hal, seperti memodifikasi fungsi dalam program, mengganti nilai suatu variabel, mendapatkan kendali atas shell, dsb.

Beberapa hal yang ada dalam binary exploitation meliputi:

## Registers
Register adalah lokasi di dalam prosesor yang dapat menyimpan data, seperti RAM. Namun tidak seperti RAM, akses ke register secara efektif seketika, sedangkan pembacaan dari memori utama dapat memerlukan ratusan siklus CPU untuk kembali.

Register dapat memiliki nilai apa pun: alamat (pointer), hasil dari operasi matematika, karakter, dll. Namun, beberapa register dicadangkan, artinya register memiliki tujuan khusus dan bukan "register tujuan umum" (GPR). Pada x86, satu-satunya 2 register yang dicadangkan adalah rip dan rsp yang masing-masing menyimpan alamat dari instruksi berikutnya untuk dieksekusi dan alamat stack.

Pada x86, register yang sama dapat memiliki akses ukuran berbeda untuk kompatibilitas mundur. Misalnya, register rax adalah register 64-bit penuh, eax adalah rax 32 bit rendah, ax adalah 16 bit rendah, al adalah 8 bit rendah, dan ah adalah kapak 8 bit tinggi (bit 8- 16 dari rax).
## The Stack
Dalam arsitektur komputer, stack adalah manifestasi perangkat keras dari struktur data stack (a last in, first out queue).

Di x86, stack hanyalah area dalam RAM yang dipilih sebagai stack - tidak ada perangkat keras khusus untuk menyimpan konten stack. Register esp / rsp menyimpan alamat dalam memori di mana bagian bawah stack berada. Ketika sesuatu di-push ke stack, esp berkurang 4 (atau 8 pada 64-bit x86), dan nilai yang di-push disimpan di lokasi itu dalam memori. Demikian juga, ketika instruksi pop dijalankan, nilai pada esp diambil (yaitu esp didereferensi), dan esp kemudian bertambah 4 (atau 8).

N.B. Stack "tumbuh" ke alamat memori yang lebih rendah!

Biasanya, ebp / rbp berisi alamat bagian atas frame stack saat ini, sehingga terkadang variabel lokal direferensikan sebagai offset relatif terhadap ebp daripada offset ke esp. Stack frame pada dasarnya hanyalah ruang yang digunakan pada stack oleh fungsi tertentu.

#### Kegunaan
Stack terutama digunakan untuk beberapa hal:

- Menyimpan argumen fungsi
- Menyimpan variabel lokal
- Menyimpan status prosesor di antara panggilan fungsi

lebih lengkap: https://ctf101.org/binary-exploitation/what-is-the-stack/
## Calling Conventions
Untuk dapat memanggil fungsi, perlu ada cara yang disepakati untuk menyampaikan argumen. Jika sebuah program sepenuhnya berdiri sendiri dalam biner, kompilator akan bebas memutuskan konvensi pemanggilan. Namun pada kenyataannya, shared library digunakan sehingga kode umum (misalnya libc) dapat disimpan satu kali dan secara dinamis dihubungkan ke program yang membutuhkannya, sehingga mengurangi ukuran program.

Dalam binari Linux, sebenarnya hanya ada dua konvensi pemanggilan yang umum digunakan: cdecl untuk binari 32-bit, dan SysV untuk 64-bit

#### cdecl
Dalam biner 32-bit di Linux, argumen fungsi diteruskan di stack dalam urutan terbalik. Fungsi seperti ini:

```c
int add (int a, int b, int c) {
    mengembalikan a + b + c;
}
```
akan dipanggil dengan mem-push c, lalu b, lalu a.

#### SysV
Untuk biner 64-bit, argumen fungsi pertama kali diteruskan dalam register tertentu:

```asm
RDI
RSI
RDX
RCX
R8
R9
```
lalu setiap argumen yang tersisa di-push ke stack dalam urutan terbalik, seperti dalam cdecl.

#### Konvensi Lainnya
Metode apa pun untuk meneruskan argumen dapat digunakan selama kompiler mengetahui apa konvensi tersebut. Akibatnya, ada banyak konvensi panggilan di masa lalu yang tidak sering digunakan lagi. Lihat Wikipedia untuk daftar lengkapnya.
## Global Offset Table (GOT)

Global Offset Table (GOT) adalah bagian program yang menyimpan alamat fungsi yang terhubung secara dinamis. Seperti

## Buffers
### Buffer Overflow
## Return Oriented Programming (ROP)
## The Heap
### Heap Exploitation
## Format String Vulnerability
## Sumber

- https://ctf101.org/binary-exploitation/overview/
