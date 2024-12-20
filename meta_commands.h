#include "input_buffer.h"
#include "table.h"

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECOGNIZED,
} MetaCommandResult;

MetaCommandResult process_meta_command(InputBuffer *, Table *);
