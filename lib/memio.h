#ifndef MEMIO_H
#define MEMIO_H

#include "hardware.h"

/* Some error handling defines */
#define LOAD_OK 0
#define LOAD_ERROR 1

/* function prototypes */
extern int     load_memory(struct ddo1 *cur_ddo1, char *file);

#endif
