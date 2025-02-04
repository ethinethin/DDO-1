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
void            tokenize_line(char buf[1024], char *word[4]);
int             is_code(char *word, struct opcodes *code, int n_codes);
void            handle_opcode(char *word, int n);
uint16_t        label_address(char *word);
uint16_t        fix_page(uint16_t address);
void            handle_group1_opr(char *word[4]);
void            handle_group2_opr(char *word[4]);
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
        char *word[4];

        /* Load the START position */
        if (fgets(buf, 1024, f) == NULL) {
                fprintf(stderr, "*** Error: did not read from file\n");
                exit(1);
        }
        /* Read until you get a line with a command */
        tokenize_line(buf, word);
        while (word[0] == NULL && fgets(buf, 1024, f) != NULL) {
                tokenize_line(buf, word);
        }
        if (word[0] == NULL) {
                fprintf(stderr, "*** Error: file did not contain any commands\n");
        } else if (strncmp("START", word[0], 5) == 0) {
                start_location(word[1]);
        } else {
                fprintf(stderr, "*** Error: file did not being with START location\n");
                exit(1);
        }
}

void
start_location(char *word)
{
        if (word == NULL) {
                fprintf(stderr, "*** Error: did not contain START coordinates\n");
                exit(1);
        }
        /* Convert to an integer and change memory location */
        MEMLOC = atoi(word);
        START_LOC = MEMLOC;
}

void
find_labels(FILE *f)
{
        char buf[1024];
        int lineloc;
        char *word[4];

        /* Fill the label table */
        while (fgets(buf, 1024, f) != NULL)
        {
                /* Tokenize the line */
                tokenize_line(buf, word);
                if (word[0] == NULL) continue;
                if (strncmp("END", word[0], 3) == 0) break;
                if (strncmp("PAD", word[0], 3) == 0) {
                        /* Are we padding? Need to adjust memory */
                        MEMLOC += atoi(word[1]);
                } else if (strncmp("PPD", word[0], 3) == 0) {
                        MEMLOC = 2048 * (calc_page(MEMLOC) + 1);
                } else if ((lineloc = contains_label(word[0])) != 0) {
                        add_label(word[0], lineloc);
                        /* Was there more content after the label? If so,  *
                         * increase memory location by 1                  */
                        if (word[1] != NULL) {
                                MEMLOC += 1;
                        }
                } else {
                        /* If was an operation, increase memory location */
                        MEMLOC += 1;
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
        char *word[4];
        int i;

        /* parse every line for commands */
        while (fgets(buf, 1024, f) != NULL) {
                /* Tokenize the line */
                tokenize_line(buf, word);
                /* Break if end, or skip if empty line or comment */
                if (strncmp("END", buf, 3) == 0) return;
                if (word[0] == NULL) continue;
                if (strncmp("PAD", word[0], 3) == 0) {
                        /* Pad with 0s - just increase current memory location */
                        MEMLOC += atoi(word[1]);
                } else if (strncmp("PPD", word[0], 3) == 0) {
                        MEMLOC = 2048 * (calc_page(MEMLOC) + 1);
                } else if ((i = is_code(word[0], OPCODES, N_OPCODES)) >= 0) {
                        handle_opcode(word[1], i);
                        MEMLOC += 1;
                } else if ((i = is_code(word[0], GROUP1_OPRS, N_GROUP1_OPRS)) >= 0) {
                        handle_group1_opr(word);
                        MEMLOC += 1;
                } else if ((i = is_code(word[0], GROUP2_OPRS, N_GROUP2_OPRS)) >= 0) {
                        handle_group2_opr(word);
                        MEMLOC += 1;
                } else if ((i = contains_label(word[0])) != 0) {
                        if (word[1] != NULL) {
                                handle_label(word[1]);
                                MEMLOC += 1;
                        }
                }
        }
}

void
tokenize_line(char buf[1024], char *word[4])
{
        char delim;
        int i, token;
        char *p;

        /* Initialize all pointers to NULL */
        for (i = 0; i < 4; i += 1) word[i] = NULL;
        /* Skip leading whitespace */
        p = buf;
        token = 0;
        while (*p == ' ' || *p == '\t') p += 1;
        /* If the line is empty, do not continue */
        if (*p == ';' || *p == '\n' || *p == '\0') return;
        /* Process up to 4 tokens */
        while (token < 4 && *p != '\0') {
                /* Break on comment or newline */
                if (*p == ';' || *p == '\n') break;
                /* Point to this token */
                word[token] = p;
                token += 1;
                /* Move forward until we hit a delimiter */
                while (*p != '\0' &&  *p != ' ' && *p != '\t' && *p != '\n' && *p != ';') p += 1;
                /* Terminate the token */
                if (*p == ' ' || *p == '\t' || *p == '\n' || *p == ';') {
                        delim = *p;
                        *p = '\0';
                        /* If at end of line, do not continue */
                        if (delim == '\n' || delim == ';') break;
                        /* Move past the delimiter */
                        p += 1;
                }
                /* Move past whitespace */
                while (*p == ' ' || *p == '\t') p += 1;
                /* If there is comment or newline, do not continue */
                if (*p == '\n' || *p == ';') break;
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

void handle_group1_opr(char *word[4])
{
        int i, j;
        uint16_t operation;

        /* Assign the value of words to the operation */
        for (i = 0, operation = 0; i < 4; i += 1) {
                if (word[i] == NULL) break;
                j = is_code(word[i], GROUP1_OPRS, N_GROUP1_OPRS);
                if (j >= 0) {
                        operation |= GROUP1_OPRS[j].binary;
                } else {
                        fprintf(stderr, "*** Error: microcoded instruction group mismatch:\n");
                        for (j = 0; j <= i; j += 1) fprintf(stderr, "***\t%s\n", word[j]);
                        fprintf(stderr, "*** If CLA is causing this, place it at the end of the chain.\n");
                        exit(1);
                }
        }
        /* Assign it to the current memory location */
        MEMORY[MEMLOC] = operation;
}

void handle_group2_opr(char *word[4])
{
        int i, j;
        uint16_t operation;

        /* Assign the value of words to the operation */
        for (i = 0, operation = 0; i < 4; i += 1) {
                if (word[i] == NULL) break;
                j = is_code(word[i], GROUP2_OPRS, N_GROUP2_OPRS);
                if (j >= 0) {
                        operation |= GROUP2_OPRS[j].binary;
                } else {
                        fprintf(stderr, "*** Error: microcoded instruction group mismatch:\n");
                        for (j = 0; j < i; j += 1) fprintf(stderr, "***\t%s\n", word[j]);
                        exit(1);
                }
        }
        /* Assign it to the current memory location */
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
        f = fopen("a.out", "w");
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
