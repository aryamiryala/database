#pragma once

typedef struct CustomerNode {
  char *email; //key
  char *name; //value
  int shoe_size;//value
  char *fav_food;//value
  struct CustomerNode *next;
} CustomerNode;

typedef struct CustomerTable {
  // the actual buckets
  // an array of linked lists.
  CustomerNode **buckets;
  // num_buckets
  size_t num_buckets;
} CustomerTable;

unsigned long hash(char *str);

bool search(CustomerTable *table, char* email);
bool delete(CustomerTable* table, char* email);

CustomerNode *add_customer_to_list(char *email, char *name, char *food, int shoe_size, CustomerNode *bucket);
CustomerNode* delete_customer_from_list(char *email, CustomerNode *linkedlist);
CustomerTable *build_customer_table(size_t num_buckets);



void add_customer_to_table(char *email,char *name, char *food, int shoe_size, CustomerTable *table);
void lookup(CustomerTable* table, char* email);
void list(CustomerTable* table);
void save(char *filename, CustomerTable *table);
void load_customers(char *filename, CustomerTable *table);
void delete_customer_table(CustomerTable *table);
