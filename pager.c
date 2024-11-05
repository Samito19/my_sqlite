#include "pager.h"

Pager *open_pager() {
  Pager *pager = (Pager *)malloc(sizeof(Pager));

  int file_descriptor = open("db", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (file_descriptor == -1) {
    printf("Could not open the db file.\n");
    exit(EXIT_FAILURE);
  }

  off_t file_length = lseek(file_descriptor, 0, SEEK_END);
  if (file_length == -1) {
    printf("Failed to seek file: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  pager->file_length = file_length;

  if ((file_length % PAGE_SIZE) != 0) {
    printf("DB file is not a whole number of pages. The file is corrupt.\n");
    exit(EXIT_FAILURE);
  }

  pager->num_pages = file_length / PAGE_SIZE;
  pager->file_descriptor = file_descriptor;

  for (int page_index = 0; page_index < MAX_PAGES; page_index++) {
    pager->pages[page_index] = NULL;
  }

  return pager;
}

void flush_page(Pager *pager, uint32_t page_num) {
  off_t offset = lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
  if (offset == -1) {
    printf("Failed to seek file: %d\n", errno);
    exit(EXIT_FAILURE);
  }

  size_t bytes_written =
      write(pager->file_descriptor, pager->pages[page_num], PAGE_SIZE);
  if (bytes_written == -1) {
    printf("Failed to write data to db file: %d\n", errno);
    exit(EXIT_FAILURE);
  }
}

void *get_page(Pager *pager, uint32_t page_num) {
  // Handle memory cache miss
  if (pager->pages[page_num] == NULL) {
    void *page = malloc(PAGE_SIZE);
    uint32_t num_pages = pager->file_length / PAGE_SIZE;

    // We check if there are existing pages in the file, if so we read them into
    // memory and keep them in cache
    if (pager->file_length % PAGE_SIZE) {
      num_pages += 1;
    }
    if (page_num <= num_pages) {
      off_t bytes_offset =
          lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
      if (bytes_offset == -1) {
        printf("Failed to seek file: %d\n", errno);
      }
      size_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE);
      if (bytes_read == -1) {
        printf("Failed to read data from db file: %d\n", errno);
        exit(EXIT_FAILURE);
      }
    }
    pager->pages[page_num] = page;

    if (page_num >= pager->num_pages) {
      pager->num_pages += 1;
    }
  }

  return pager->pages[page_num];
}
