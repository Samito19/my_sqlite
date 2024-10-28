#ifndef INPUT_BUFFER_H
#define INPUT_BUFFER_H

#include <stdlib.h>

typedef struct {
  char *buffer;
  size_t buffer_length;
  size_t input_length;
} InputBuffer;

InputBuffer *new_input_buffer(void);
void close_input_buffer(InputBuffer *);

#endif // !INPUT_BUFFER_H
