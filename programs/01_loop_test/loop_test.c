#include <stdio.h>

int
main(void)
{
        int i;

        // START 2048
        for (i = 0; i < 2048; i += 1) {
                printf("%c%c", 0, 0);
        }
        // LOOP (memory location 2048, or page 1 mem 0)
        // CML (0b1110000100000000)
        printf("%c%c", 0b00000000, 0b11100001);
        // CLA (0b1110100000000000)
        printf("%c%c", 0b00000000, 0b11101000);
        // TAD AMOUNT (0b0010100000000110) (mem location 2054, or page 1 mem 6)
        printf("%c%c", 0b00000110, 0b00101000);
        // ISZ AMOUNT (0b0100100000000110) (mem location 2054, or page 1 mem 6)
        printf("%c%c", 0b00000110, 0b01001000);
        // JMP LOOP (0b1010100000000000)
        printf("%c%c", 0b00000000, 0b10101000);
        // HLT (0b1111000000100000)
        printf("%c%c", 0b00100000, 0b11110000);
        // AMOUNT, -1000 (0b1111110000011000)
        printf("%c%c", 0b00011000, 0b11111100);
        return 0;
}