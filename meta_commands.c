#include "meta_commands.h"
#include "input_buffer.h"
#include "table.h"
#include <string.h>

MetaCommandResult process_meta_command(InputBuffer *input_buffer,
                                       Table *table) {
  if (strcmp(input_buffer->buffer, ".exit") == 0) {
    close_input_buffer(input_buffer);
    close_db(table);
    exit(EXIT_SUCCESS);
  } else {
    return META_COMMAND_UNRECOGNIZED;
  }
}
