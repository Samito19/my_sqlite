#include "statement_processor.h"
#include "btree.h"

void leaf_node_insert(Cursor *cursor, uint32_t key, Row *row) {
  void *node = get_page(cursor->table->pager, cursor->page_num);
  uint32_t num_cells = *leaf_node_num_cells(node);
  memcpy(leaf_node_cell(node, key), row, LEAF_NODE_CELL_SIZE);
  serialize(leaf_node_value(node, cursor->cell_num), row);
  *(leaf_node_num_cells(node)) += 1;
  *leaf_node_key(node, cursor->cell_num) = key;
}
ExecuteResult exec_insert(Table *table, Row *row) {
  Cursor *cursor = table_end(table);

  if (cursor->cell_num >= LEAF_NODE_MAX_CELLS) {
    printf("Leaf node is full. Need to implement node splitting.\n");
    exit(EXIT_FAILURE);
  }
  leaf_node_insert(cursor, row->id, row);
  free(cursor);
  return EXEC_SUCCESS;
}

void print_row(Row *row) {
  printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

ExecuteResult exec_select(Table *table, Row *row) {
  Cursor *cursor = table_start(table);

  while (!(cursor->end_of_table)) {
    Row row;
    deserialize(
        &row, leaf_node_value(get_page(cursor->table->pager, cursor->page_num),
                              cursor->cell_num));
    print_row(&row);
    cursor_advance(cursor);
  }

  free(cursor);
  return EXEC_SUCCESS;
}

ProcessorResult process_insert(InputBuffer *input_buffer,
                               Statement *statement) {
  statement->type = STATEMENT_INSERT;

  char *_keyword = strtok(input_buffer->buffer, " ");
  char *id_string = strtok(NULL, " ");
  char *username = strtok(NULL, " ");
  char *email = strtok(NULL, " ");

  if (id_string == NULL || username == NULL || email == NULL) {
    return PROCESSOR_SYNTAX_ERROR;
  }

  int id = atoi(id_string);
  if (id < 0) {
    return PROCESSOR_ID_NEGATIVE;
  }

  if (strlen(username) > COLUMN_USERNAME_SIZE) {
    return PROCESSOR_STR_TOO_LONG;
  }

  if (strlen(email) > COLUMN_EMAIL_SIZE) {
    return PROCESSOR_STR_TOO_LONG;
  }

  statement->row.id = id;
  strcpy(statement->row.username, username);
  strcpy(statement->row.email, email);

  printf("Inserted: (%d, %s, %s)\n", statement->row.id, statement->row.username,
         statement->row.email);
  return PROCESSOR_SUCCESS;
}

ProcessorResult process_statement(InputBuffer *input_buffer,
                                  Statement *statement) {
  if (strcmp(input_buffer->buffer, "select") == 0) {
    statement->type = STATEMENT_SELECT;
    return PROCESSOR_SUCCESS;
  } else if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
    return process_insert(input_buffer, statement);
  } else {
    return PROCESSOR_UNRECOGNIZED_COMMAND;
  }
}

ExecuteResult exec_statement(Statement *statement, Table *table) {
  switch (statement->type) {
  case STATEMENT_SELECT:
    return exec_select(table, &(statement->row));
  case STATEMENT_INSERT:
    return exec_insert(table, &(statement->row));
  }
}
