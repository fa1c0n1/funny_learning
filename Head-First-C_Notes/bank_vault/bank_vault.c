#include <stdio.h>
#include <encrypt.h>
#include <checksum.h>

int main(void)
{
    char s[] = "Speak friend and enter";
    encrypt(s);
    printf("Encrypted to '%s'\n", s);
    printf("Checksum is %d\n", checksum(s));
    encrypt(s);
    printf("Decrypted back to '%s'\n", s);
    printf("Checksum is %d\n", checksum(s));

    return 0;
}
