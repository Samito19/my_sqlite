#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "input_buffer.h"
#include "meta_commands.h"
#include "prepare_statements.h"

void printprompt() { printf("my_sqlite > "); }

void read_input(InputBuffer *input_buffer) {
  ssize_t bytes_read =
      getline(&(input_buffer->buffer), &(input_buffer->buffer_length), stdin);

  if (bytes_read <= 0) {
    printf("Error reading input.\n");
    exit(EXIT_FAILURE);
  }

  // Remove the trailing newline character from the input buffer and resize
  input_buffer->input_length = bytes_read - 1;
  input_buffer->buffer[bytes_read - 1] = 0;
}

int main(int argc, char *argv[]) {

  InputBuffer *input_buffer = new_input_buffer();
  while (true) {
    printprompt();
    read_input(input_buffer);
    if (input_buffer->buffer[0] == '.') {
      switch (process_meta_command(input_buffer)) {
      case META_COMMAND_SUCCESS:
        continue;
      case META_COMMAND_UNRECOGNIZED:
        printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        continue;
      }
    }

    Statement *statement;
    switch (process_statement(input_buffer, statement)) {
    case PREPARE_SUCCESS:
      exec_statement(statement);
      continue;
    case PREPARE_UNRECOGNIZED_COMMAND:
      printf("Unrecognized keyword at the start of '%s'.\n",
             input_buffer->buffer);
      continue;
    }
  }
}
