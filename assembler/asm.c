#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "asm.h"

/* function prototypes */
void            init_memory(void);
void            set_start(FILE *f);
void            start_location(char buf[1024]);
void            find_labels(FILE *f);
int             contains_label(char buf[1024]);
void            add_label(char buf[1024], int lineloc);
int             calc_page(uint16_t address);
int             is_more_with_label(char buf[1024]);
void            parse_instructions(FILE *f);
int             is_code(char *word, struct opcodes *code, int n_codes);
void            handle_opcode(char *word, int n);
uint16_t        label_address(char *word);
uint16_t        fix_page(uint16_t address);
void            handle_group1_opr(char *word, int n);
void            handle_group2_opr(char *word, int n);
void            handle_label(char *word);
void            output_binary(void);

/* memory storage */
#define MEMSIZE 65536
uint16_t MEMORY[MEMSIZE];
int MEMLOC;
int START_LOC;
/* the labels table */
struct label_table {
        char name[64];
        uint16_t memloc;
        int page;
} LABEL_TABLE[1024];
int N_LABELS;

int
main(int argc, char *argv[])
{
        FILE *f;

        /* Make sure the user specified a file */
        if (argc != 2) {
                fprintf(stderr, "*** Error: wrong number of arguments\n");
                exit(1);
        }
        /* Load the file and make sure it exists */
        f = fopen(argv[1], "r");
        if (f == NULL) {
                fprintf(stderr, "*** Error: file access error\n");
                exit(1);
        }
        /* initialize memory */
        init_memory();
        /* Start the memory location */
        set_start(f);
        /* Fill the labels table */
        find_labels(f);
        /* Parse instructions */
        parse_instructions(f);
        /* Let's get up on outta here */
        fclose(f);
        output_binary();
        return 0;
}

void
init_memory(void)
{
        int i;
        for (i = 0; i < MEMSIZE; i += 1) MEMORY[i] = 0;
        MEMLOC = 0;
        START_LOC = 0;
        N_LABELS = 0;
}

void
set_start(FILE *f)
{
        char buf[1024];

        /* Load the START position */
        if (fgets(buf, 1024, f) == NULL) {
                fprintf(stderr, "*** Error: did not read from file\n");
                exit(1);
        }
        if (strncmp("START ", buf, 6) == 0) {
                start_location(buf);
        } else {
                fprintf(stderr, "*** Error: file did not contain start location\n");
                exit(1);
        }
}

void
start_location(char buf[1024])
{
        char *word;

        /* Receiving a line like "START 2048 ; blah blah", so I'm going to *
         * use strsep twice to isolate the number                          */
        word = buf;
        strsep(&word, " ");
        word = strsep(&word, " ");
        /* Convert to an integer and change memory location */
        MEMLOC = atoi(word);
        START_LOC = MEMLOC;
}

void
find_labels(FILE *f)
{
        char buf[1024];
        int lineloc;
        char *word;
        /* Fill the label table */
        while (fgets(buf, 1024, f) != NULL)
        {
                if (strncmp("END", buf, 3) == 0) break;
                if (strncmp("PAD", buf, 3) == 0) {
                        /* Are we padding? Need to adjust memory */
                        word = buf;
                        strsep(&word, " ");
                        MEMLOC += atoi(word);
                } else if (strncmp("PPD", buf, 3) == 0) {
                        MEMLOC = 2048 * (calc_page(MEMLOC) + 1);
                } else if ((lineloc = contains_label(buf)) != 0) {
                        add_label(buf, lineloc);
                        /* Was there more content after the label? If so,  *
                         * increase memory location by 1                  */
                        if (is_more_with_label(buf)) {
                                MEMLOC += 1;
                        }
                } else {
                        /* If was an operation, increase memory location */
                        if (buf[0] != ';' && buf[0] != '\n') {
                                MEMLOC += 1;
                        }
                }
        }
        /* Seek back to the beginning of the file and go to beginning of memory */
        fseek(f, 0, SEEK_SET);
        MEMLOC = START_LOC;
}

int
contains_label(char buf[1024])
{
        int i;
        /* Check if this line contains a label - look for a comma */
        for (i = 0; buf[i] != '\0'; i += 1) {
                if (buf[i] == ',') {
                        /* Found label indicator */
                        return i;
                } else if (buf[i] == ';') {
                        /* Reached comments with no label indicator */
                        return 0;
                }
        }
        return 0;
}

void
add_label(char buf[1024], int lineloc)
{
        /* Add the label to the table */
        LABEL_TABLE[N_LABELS].memloc = MEMLOC;
        strncpy(LABEL_TABLE[N_LABELS].name, buf, lineloc);
        LABEL_TABLE[N_LABELS].page = calc_page(MEMLOC);
        /* Increase label count */
        N_LABELS += 1;
}

int
calc_page(uint16_t address)
{
        uint16_t i;
        int page;

        for (i = 0, page = 0; page < 32; page += 1, i += 2048) {
                if (address >= i && address < i + 2048) {
                        return page;
                }
        }
        return -1;
}

int
is_more_with_label(char buf[1024])
{
        char *word;

        /* Is there a space after the label or not, i.e.:     *
         * ; label by itself without comments:
         * LABEL,
         * LABEL,                  ; label by itself with comments
         * LABEL, blahblah         ; label with operation
         */
        word = buf;
        strsep(&word, " ");
        if (word == NULL || word[0] == ' ' || word[0] == '\n') {
                return 0;
        } else {
                return 1;
        }
}

void
parse_instructions(FILE *f)
{
        char buf[1024];
        char *word1, *word2;
        int i;

        /* parse every line for commands */
        while (fgets(buf, 1024, f) != NULL) {
                /* Break if end, or skip if empty line or comment */
                if (strncmp("END", buf, 3) == 0) return;
                if (buf[0] == '\n' || buf[0] == ';') continue;
                /* Pull the two words out of the line */
                word2 = buf;
                word1 = strsep(&word2, " ");
                /* Remove characters after second word */
                if (word2 != NULL) word2 = strsep(&word2, " "); 
                if (strncmp("PAD", word1, 3) == 0) {
                        /* Pad with 0s - just increase current memory location */
                        MEMLOC += atoi(word2);
                } else if (strncmp("PPD", word1, 3) == 0) {
                        printf("%u\n", MEMLOC);
                        MEMLOC = 2048 * (calc_page(MEMLOC) + 1);
                        printf("%u\n", MEMLOC);
                } else if ((i = is_code(word1, OPCODES, N_OPCODES)) >= 0) {
                        handle_opcode(word2, i);
                        MEMLOC += 1;
                } else if ((i = is_code(word1, GROUP1_OPRS, N_GROUP1_OPRS)) >= 0) {
                        handle_group1_opr(word2, i);
                        MEMLOC += 1;
                } else if ((i = is_code(word1, GROUP2_OPRS, N_GROUP2_OPRS)) >= 0) {
                        handle_group2_opr(word2, i);
                        MEMLOC += 1;
                } else if ((i = contains_label(word1)) != 0) {
                        if (word2 != NULL && word2[0] != ' ' && word2[0] != 0) {
                                handle_label(word2);
                                MEMLOC += 1;
                        }
                }
        }
}

int is_code(char *word, struct opcodes *codes, int n_codes)
{
        int i;

        /* Check if the first word is an opcode */
        for (i = 0; i < n_codes; i += 1) {
                if (strncmp(word, codes[i].code, 3) == 0) {
                        return i;
                }
        }
        /* Was not a word */
        return -1;
}

void handle_opcode(char *word, int n)
{
        uint16_t operation;

        /* Assign the value of the op code to the operation */
        operation = OPCODES[n].binary;
        /* Check if indirection is used */
        if (word[0] == '(') {
                /* Yes, so OR the indrection code and increment the word by 1 */
                operation |= INDIRECT_BIT;
                word += 1;
        }
        /* OR the address of the label specified by word2 to    *
         * the operation                                        */
        operation |= (label_address(word));
        /* Assign it to the current memory location */
        MEMORY[MEMLOC] = operation;
}

uint16_t label_address(char *word)
{
        uint16_t address;
        int i;
        size_t label_len;

        /* Cycle through labels until you find the right one,   *
         * then return the address                              */
        for (i = 0; i < N_LABELS; i += 1) {
                label_len = strnlen(LABEL_TABLE[i].name, 64);
                if (strncmp(word, LABEL_TABLE[i].name, label_len) == 0) {
                        /* Found it, make sure it's accessible to the current page */
                        return fix_page(LABEL_TABLE[i].memloc);
                }
        }
        /* Couldn't find a label so it must be a number instead */
        address = (uint16_t) atoi(word);
        /* Make sure it's accessible to the current page */
        return fix_page(address);        
}

uint16_t
fix_page(uint16_t address)
{
        int cur_page, word_page;

        /* Calculate current and word page */
        cur_page = calc_page(MEMLOC);
        word_page = calc_page(address);
        /* Make sure it's accessible to the current page */
        if (word_page == cur_page) {
                /* Remove page and OR current page bit */
                address &= 0b0000011111111111;
                address |= PAGE_BIT;
                return address;
        } else if (word_page == 0) {
                /* Should be fine to return as is */
                return address;
        } else {
                /* The word is on a page not accessible */
                fprintf(stderr, "*** Error: at memloc %u (page %d) instruction calls memloc %u (%d), not accessible. Try storing this address in page 0, which can be accessed anywhere\n", MEMLOC, cur_page, address, word_page);
                exit(1);
        }
}

void handle_group1_opr(char *word, int n)
{
        uint16_t operation;

        /* Assign the value of word1 to the operation */
        operation = GROUP1_OPRS[n].binary;
        /* If word2 is a group1 operation, OR it to the operation */
        if (word != NULL && (n = is_code(word, GROUP1_OPRS, N_GROUP1_OPRS)) >= 0) {
                operation |= GROUP1_OPRS[n].binary;
        }
        /* Assign it to the current memory location */
        MEMORY[MEMLOC] = operation;
}

void handle_group2_opr(char *word, int n)
{
        uint16_t operation;

        /* Assign the value of word1 to the operation */
        operation = GROUP2_OPRS[n].binary;
        /* If word is a group2 operation, OR it to the operation */
        if (word != NULL && (n = is_code(word, GROUP2_OPRS, N_GROUP2_OPRS)) >= 0) {
                operation |= GROUP2_OPRS[n].binary;
        }
        MEMORY[MEMLOC] = operation;
}

void
handle_label(char *word)
{
        /* Word must be a value, so add it to current memory location */
        MEMORY[MEMLOC] = (uint16_t) atoi(word);
}

void
output_binary(void)
{
        uint16_t lb, hb;
        int i;
        FILE *f;

        /* Open the damn file */
        f = fopen("a.bin", "w");
        if (f == NULL) {
                fprintf(stderr, "*** Cannot open output file\n");
                exit(1);
        }
        /* Go through and write out memory up to the final memory location */
        for (i = 0; i < MEMLOC; i += 1) {
                lb = 0b0000000011111111 & MEMORY[i];
                hb = (0b1111111100000000 & MEMORY[i]) >> 8;
                fputc((char) lb, f);
                fputc((char) hb, f);
        }
        /* Close the damn file */
        fclose(f);
}
