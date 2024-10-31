#ifndef STATEMENT_PROCESSOR_H
#define STATEMENT_PROCESSOR_H

#include "constants.h"
#include "cursor.h"
#include "input_buffer.h"
#include "table.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  PROCESSOR_SUCCESS,
  PROCESSOR_SYNTAX_ERROR,
  PROCESSOR_STR_TOO_LONG,
  PROCESSOR_ID_NEGATIVE,
  PROCESSOR_UNRECOGNIZED_COMMAND,
} ProcessorResult;

typedef enum { EXEC_SUCCESS, EXEC_TABLE_FULL } ExecuteResult;

typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
  StatementType type;
  Row row;
} Statement;

ProcessorResult process_statement(InputBuffer *, Statement *);
ExecuteResult exec_statement(Statement *, Table *);

#define STATEMENT_PROCESSOR_H
#endif // !STATEMENT_PROCESSOR_H
