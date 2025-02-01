#include <stdint.h>
#include <stdio.h>
#include "hardware.h"
#include "memio.h"

int
load_memory(struct ddo1 *cur_ddo1, char *file)
{
        WORDTYPE hb, lb;
        int memloc;
        FILE *f;
        
        /* Try to open the file */
        f = fopen(file, "r");
        if (f == NULL) {
                /* File failed to open */
                return LOAD_ERROR;
        }
        /* Load the contents of the file into memory location 0 */
        memloc = 0;
        /* Load in bytes two at a time - these are stored little endian */
        while ((char) (lb = fgetc(f)) != EOF) {
                /* If there are an odd number of bytes, the file didn't load right */
                if ((char) (hb = fgetc(f)) == EOF) {
                        fclose(f);
                        return LOAD_ERROR;
                }
                /* If the file is too big, it won't load correctly */
                if (memloc > MEMSIZE) {
                        fclose(f);
                        return LOAD_ERROR;
                }
                /* Load in the bytes in the correct positions */
                cur_ddo1->memory[memloc] = (hb << 8) | lb;
                /* Increase memory location by one */
                memloc += 1;
        }
        fclose(f);
        /* Set the program counter to page 1, mem 0, then activate the computer */
        cur_ddo1->PC = PAGE_1;
        cur_ddo1->run = DDO1_ON;
        return LOAD_OK;
}
