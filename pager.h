#ifndef PAGER_H
#define PAGER_H

#include "constants.h"
#include "serializers.h"
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
  uint32_t file_descriptor;
  uint32_t file_length;
  uint32_t num_pages;
  void *pages[MAX_PAGES];
} Pager;

Pager *open_pager(void);
void flush_page(Pager *, uint32_t);
void *get_page(Pager *, uint32_t);

#endif // !PAGER_H
