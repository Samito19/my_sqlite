#ifndef STATEMENT_PROCESSOR_H
#define STATEMENT_PROCESSOR_H

#include "input_buffer.h"

typedef enum {
  PROCESSOR_SUCCESS,
  PROCESSOR_UNRECOGNIZED_COMMAND,
} ProcessorResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

ProcessorResult process_statement(InputBuffer *, Statement *);
void exec_statement(Statement *);

#define STATEMENT_PROCESSOR_H
#endif // !STATEMENT_PROCESSOR_H
