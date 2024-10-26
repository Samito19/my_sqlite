#ifndef PREPARE_STATEMENT_H
#define PREPARE_STATEMENT_H

#include "input_buffer.h"

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_COMMAND,
} PrepareResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
} Statement;

PrepareResult process_statement(InputBuffer *, Statement *);
void exec_statement(Statement *);

#define PREPARE_STATEMENT_H
#endif // !PREPARE_STATEMENT_H
