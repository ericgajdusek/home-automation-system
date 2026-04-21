#ifndef NODE_RUNTIME_PENDING_TABLE_H
#define NODE_RUNTIME_PENDING_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "ap_message.h"

#define MAX_PENDING_ENTRIES 16

static pending_entry_t pending_table[MAX_PENDING_ENTRIES];

int pending_table_add(const egress_item_t *item, message_id_t message_id, transaction_id_t transaction_id);
int pending_table_remove(message_id_t message_id, transaction_id_t transaction_id);
pending_entry_t* pending_table_find(message_id_t message_id, transaction_id_t transaction_id);
void pending_table_mark_acknowledged(message_id_t message_id, transaction_id_t transaction_id);
void pending_table_check_timeouts(uint64_t current_time_ms);

#endif // NODE_RUNTIME_PENDING_TABLE_H
