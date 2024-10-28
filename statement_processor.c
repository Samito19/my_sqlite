#include "statement_processor.h"

ExecuteResult exec_insert(Table *table, Row *row) {
  serialize(row_slot(table, table->num_rows), row);
  table->num_rows += 1;
  return EXEC_SUCCESS;
}

void print_row(Row *row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

ExecuteResult exec_select(Table *table, Row *row) {
  if (table->num_rows >= TABLE_MAX_ROWS) {
    return EXEC_TABLE_FULL;
  }

  for (int row_num = 0; row_num < table->num_rows; row_num++) {
    Row row;
    deserialize(&row, row_slot(table, row_num));
    print_row(&row);
  }

  return EXEC_SUCCESS;
}

ProcessorResult process_statement(InputBuffer *input_buffer,
                                  Statement *statement) {
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PROCESSOR_SUCCESS;
  } else if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    int args =
        sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row.id),
               statement->row.username, statement->row.email);
    if (args < 3) {
      return PROCESSOR_SYNTAX_ERROR;
    }

    printf("Inserted: (%d, %s, %s)\n", statement->row.id,
           statement->row.username, statement->row.email);

    return PROCESSOR_SUCCESS;
  } else {
    return PROCESSOR_UNRECOGNIZED_COMMAND;
  }
}

ExecuteResult exec_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case STATEMENT_SELECT:
    return exec_select(table, &(statement->row));
    break;
  case STATEMENT_INSERT:
    return exec_insert(table, &(statement->row));
    break;
  }
}
