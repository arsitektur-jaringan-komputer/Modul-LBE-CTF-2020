#include <stdio.h>

int main() {
    int secret = 0xdeadbeef;
    char name[9] = {0};
    read(0, name, 0x10);
    if (secret == 0x1331) {
        puts("Wow! Here's a secret.");
    } else {
        puts("I guess you're not cool enough to see my secret");
        printf("%x\n", secret);
    }
}
