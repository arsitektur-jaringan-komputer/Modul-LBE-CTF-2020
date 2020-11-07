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

#### Contoh
Mari kita lihat seperti apa tumpukan itu tepat setelah say_hi dipanggil dalam program 32-bit x86 C ini:
```c
#include <stdio.h>

void say_hi(const char * name) {
    printf("Hello %s!\n", name);
}

int main(int argc, char ** argv) {
    char * name;
    if (argc != 2) {
        return 1;
    }
    name = argv[1];
    say_hi(name);
    return 0;
}
```
Dan assembly yang relevan:
```asm
0804840b <say_hi>:
 804840b:   55                      push   ebp
 804840c:   89 e5                   mov    ebp,esp
 804840e:   83 ec 08                sub    esp,0x8
 8048411:   83 ec 08                sub    esp,0x8
 8048414:   ff 75 08                push   DWORD PTR [ebp+0x8]
 8048417:   68 f0 84 04 08          push   0x80484f0
 804841c:   e8 bf fe ff ff          call   80482e0 <printf@plt>
 8048421:   83 c4 10                add    esp,0x10
 8048424:   90                      nop
 8048425:   c9                      leave
 8048426:   c3                      ret

08048427 <main>:
 8048427:   8d 4c 24 04             lea    ecx,[esp+0x4]
 804842b:   83 e4 f0                and    esp,0xfffffff0
 804842e:   ff 71 fc                push   DWORD PTR [ecx-0x4]
 8048431:   55                      push   ebp
 8048432:   89 e5                   mov    ebp,esp
 8048434:   51                      push   ecx
 8048435:   83 ec 14                sub    esp,0x14
 8048438:   89 c8                   mov    eax,ecx
 804843a:   83 38 02                cmp    DWORD PTR [eax],0x2
 804843d:   74 07                   je     8048446 <main+0x1f>
 804843f:   b8 01 00 00 00          mov    eax,0x1
 8048444:   eb 1c                   jmp    8048462 <main+0x3b>
 8048446:   8b 40 04                mov    eax,DWORD PTR [eax+0x4]
 8048449:   8b 40 04                mov    eax,DWORD PTR [eax+0x4]
 804844c:   89 45 f4                mov    DWORD PTR [ebp-0xc],eax
 804844f:   83 ec 0c                sub    esp,0xc
 8048452:   ff 75 f4                push   DWORD PTR [ebp-0xc]
 8048455:   e8 b1 ff ff ff          call   804840b <say_hi>
 804845a:   83 c4 10                add    esp,0x10
 804845d:   b8 00 00 00 00          mov    eax,0x0
 8048462:   8b 4d fc                mov    ecx,DWORD PTR [ebp-0x4]
 8048465:   c9                      leave
 8048466:   8d 61 fc                lea    esp,[ecx-0x4]
 8048469:   c3                      ret
 ```
Melewati sebagian besar `main`, Anda akan melihat bahwa pada `0x8048452` nama `main` local di-push ke stack karena itu adalah argumen pertama untuk `say_hi`. Kemudian, instruksi `call` dijalankan. instruksi `call` pertama-tama mem-push instruction pointer saat ini ke stack, lalu lompat ke tujuannya. Jadi ketika prosesor mulai menjalankan `say_hi` pada `0x0804840b`, stack-nya terlihat seperti ini:
```asm
EIP = 0x0804840b (push ebp)
ESP = 0xffff0000
EBP = 0xffff002c

        0xffff0004: 0xffffa0a0              // say_hi argument 1
ESP ->  0xffff0000: 0x0804845a              // Return address for say_hi
```
Sekali lagi, perhatikan bagaimana `esp` menjadi lebih kecil ketika nilai di-push ke stack.

Selanjutnya, `esp` saat ini disimpan ke `ebp`, menandai bagian atas stack frame baru.
```asm
EIP = 0x0804840e (sub esp, 0x8)
ESP = 0xfffefffc
EBP = 0xfffefffc

            0xffff0004: 0xffffa0a0              // say_hi argument 1
            0xffff0000: 0x0804845a              // Return address for say_hi
ESP, EBP -> 0xfffefffc: 0xffff002c              // Saved EBP
```
Kemudian, stack "dikembangkan" untuk mengakomodasi variabel lokal di dalam `say_hi`.
```asm
EIP = 0x08048414 (push [ebp + 0x8])
ESP = 0xfffeffec
EBP = 0xfffefffc

        0xffff0004: 0xffffa0a0              // say_hi argument 1
        0xffff0000: 0x0804845a              // Return address for say_hi
EBP ->  0xfffefffc: 0xffff002c              // Saved EBP
        0xfffefff8: UNDEFINED
        0xfffefff4: UNDEFINED
        0xfffefff0: UNDEFINED
ESP ->  0xfffefffc: UNDEFINED
```
**CATATAN: ruang tumpukan tidak secara implisit dihapus!**
Sekarang, 2 argumen untuk `printf` di-push dalam urutan terbalik.
```asm
EIP = 0x0804841c (call printf@plt)
ESP = 0xfffeffe4
EBP = 0xfffefffc

        0xffff0004: 0xffffa0a0              // say_hi argument 1
        0xffff0000: 0x0804845a              // Return address for say_hi
EBP ->  0xfffefffc: 0xffff002c              // Saved EBP
        0xfffefff8: UNDEFINED
        0xfffefff4: UNDEFINED
        0xfffefff0: UNDEFINED
        0xfffeffec: UNDEFINED
        0xfffeffe8: 0xffffa0a0              // printf argument 2
ESP ->  0xfffeffe4: 0x080484f0              // printf argument 1
```
Akhirnya, `printf` dipanggil, yang mem-push alamat dari instruksi berikutnya untuk dieksekusi.
```asm
EIP = 0x080482e0
ESP = 0xfffeffe4
EBP = 0xfffefffc

        0xffff0004: 0xffffa0a0              // say_hi argument 1
        0xffff0000: 0x0804845a              // Return address for say_hi
EBP ->  0xfffefffc: 0xffff002c              // Saved EBP
        0xfffefff8: UNDEFINED
        0xfffefff4: UNDEFINED
        0xfffefff0: UNDEFINED
        0xfffeffec: UNDEFINED
        0xfffeffe8: 0xffffa0a0              // printf argument 2
        0xfffeffe4: 0x080484f0              // printf argument 1
ESP ->  0xfffeffe0: 0x08048421              // Return address for printf
```
Setelah `printf` kembali, instruksi `leave` memindahkan `ebp` ke `esp`, dan memunculkan `EBP` yang disimpan.
```asm
EIP = 0x08048426 (ret)
ESP = 0xfffefffc
EBP = 0xffff002c

        0xffff0004: 0xffffa0a0              // say_hi argument 1
ESP ->  0xffff0000: 0x0804845a              // Return address for say_hi
```
Dan akhirnya, `ret` memunculkan pointer instruksi yang disimpan ke dalam `eip` yang menyebabkan program kembali ke `main` dengan konten `esp`, `ebp`, dan stack yang sama seperti ketika `say_hi` awalnya dipanggil.
```asm
EIP = 0x0804845a (add esp, 0x10)
ESP = 0xffff0000
EBP = 0xffff002c

ESP ->  0xffff0004: 0xffffa0a0              // say_hi argument 1
```
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
