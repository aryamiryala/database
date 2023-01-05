#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "businessdb.h"
#define BUF_SIZE 1024

//hash function
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while (*str != '\0') {
    c = *str;
    hash = ((hash << 5) + hash) + (unsigned char)c; /* hash * 33 + c */
    str++;
  }
  return hash;
}
CustomerNode *add_customer_to_list(char *email, char *name, char *food, int shoe_size, CustomerNode *bucket) {
  CustomerNode* new_customer;

  new_customer = calloc(1, sizeof(CustomerNode));
  new_customer->email = strdup(email);
  new_customer->name = strdup(name);
  new_customer->fav_food = strdup(food);
  new_customer->shoe_size = shoe_size;
  new_customer->next = bucket;

  return new_customer;
}

void add_customer_to_table(char *email,char *name, char *food, int shoe_size, CustomerTable *table){
  unsigned long hashvalue = hash(email); //email is the key
  size_t bucket = hashvalue % table->num_buckets; //hash value mod total number of buckets to get specific bucket

  CustomerNode *linkedlist = table->buckets[bucket];
  table->buckets[bucket] = add_customer_to_list(email, name, food, shoe_size, linkedlist);
  return;

}
CustomerTable *build_customer_table(size_t num_buckets) {
  CustomerTable* output = calloc(1, sizeof(CustomerTable));
  output->num_buckets = num_buckets;

  // allocate space for num_buckets CustomerNode pointers.
  // THEY ARE ALREADY NULL POINTERS.
  output->buckets = calloc(num_buckets, sizeof(CustomerNode *));

  return output;
}
bool search(CustomerTable *table, char* email){
  unsigned long hashvalue = hash(email);
  size_t which_bucket = hashvalue % table->num_buckets;
  CustomerNode* node = table->buckets[which_bucket];
  CustomerNode* here = node;
  while(here){
    if(!strcmp(here->email, email)){
      return true;
    }

  }
  return false;
}
CustomerNode* delete_customer_from_list(char *email, CustomerNode *linkedlist) {
  if (!linkedlist) {
    return NULL;
  } else if (!strcmp(email, linkedlist->email)) {
    // match!!

    // need to free this node!
    CustomerNode *next = linkedlist->next;
    // now delete this node.
    free(linkedlist->email);
    free(linkedlist->name);
    free(linkedlist->fav_food);
    free(linkedlist);
    return next;
  } else {
    // recursive case!!
    linkedlist->next = delete_customer_from_list(email, linkedlist->next);
    return linkedlist;
  }
}
bool delete(CustomerTable* table, char* email){
  unsigned long hashvalue = hash(email);
  size_t which_bucket = hashvalue % table->num_buckets;
  CustomerNode *node = table->buckets[which_bucket];
  bool found = false;
  CustomerNode *here = node;
  while(here){
    if(!strcmp(here->email, email)){
      found = true;
      break;
    }
    here = here->next;
  }
  if(!found){
    return false;
  }
  table->buckets[which_bucket] = delete_customer_from_list(email, node);
  return true;
}
void lookup(CustomerTable* table, char* email){
  unsigned long hashvalue = hash(email);
  size_t which_bucket = hashvalue % table->num_buckets;
  CustomerNode *node = table->buckets[which_bucket];
  while(node){
    if(strcmp(node->email, email) == 0){

      printf("Email Address: %s \n", node->email);
      printf("Name: %s \n", node->name);
      int shoe = node->shoe_size;
      printf("Shoe size: %d \n", shoe);
      printf("Favorite Food: %s \n", node->fav_food);
    }
    node = node->next;

  }
}
void list(CustomerTable* table){
  for(size_t i = 0; i < table->num_buckets; i++){
    if(table->buckets[i]){
      printf("Email Address: %s \n", table->buckets[i]->email);
      printf("Name: %s \n", table->buckets[i]->name);
      int shoe = table->buckets[i]->shoe_size;
      printf("Shoe size: %d \n", shoe);
      printf("Favorite Food: %s \n\n\n", table->buckets[i]->fav_food);
    }
  }

}
void save(char *filename, CustomerTable *table){
  FILE *infile = fopen(filename, "w");
  for(size_t i = 0; i < table->num_buckets; i++){
    if(table->buckets[i]){
      fprintf(infile, "%s\t", table->buckets[i]->email);
      fprintf(infile, "%s\t", table->buckets[i]->name);
      int shoe = table->buckets[i]->shoe_size;
      fprintf(infile, "%d\t", shoe);
      fprintf(infile, "%s\t\n", table->buckets[i]->fav_food);

    }

  }
  fclose(infile);

}
void load_customers(char *filename, CustomerTable *table){
  FILE *infile = fopen(filename, "r");
  char buf[BUF_SIZE];
  char *email[BUF_SIZE];
  char *name[BUF_SIZE];
  char *food[BUF_SIZE];
  char *shoe[BUF_SIZE];
  while(fgets(buf,BUF_SIZE,infile) != NULL){
    *email = strtok(buf,"\t\n");
    *name = strtok(NULL, "\t\n");
    *shoe = strtok(NULL, "\t\n");
    int shoe_size = atoi(*shoe);
    *food = strtok(NULL, "\t\n");
    add_customer_to_table(*email, *name, *food, shoe_size, table);
  }
  fclose(infile);
}
void delete_customer_table(CustomerTable *table) {
  // for each bucket, delete everything in that bucket
  for (size_t i=0; i < table->num_buckets; i++) {
    CustomerNode *here = table->buckets[i];
    while(here) {
      free(here->name);
      free(here->fav_food);
      free(here->email);
      CustomerNode *freethis = here;
      here = here->next;
      free(freethis);
    }
  }
  free(table->buckets);
  free(table);
}
void remove_newline(char *s){
  if(!s){
    return;
  }
  char *newline;
  newline = strchr(s, '\n');
  if(newline){
    *newline = '\0';
  }
}

int main(void){
  CustomerTable* table = build_customer_table(1000);
  load_customers("customers.tsv", table);
  while(true){
    char input[200];
    printf("\n Please enter a command: ");
    fgets(input,200,stdin);
    //scanf("%s\n", input);
    input[strlen(input)-1]=0;

    if (strcmp(input, "add") == 0){
      char email[200];
      char name[200];
      char shoe_size[200];
      char fav_food[200];
      printf("Please enter your email: ");
      fgets(email, 200, stdin);
      email[strlen(email)-1]=0;
      //scanf("%[^\t\n]", email);
      printf("Please enter your name: ");
      fgets(name, 200, stdin);
      name[strlen(name)-1]=0;
      //scanf("%[^\t\n]", name);
      printf("Please enter your shoe size: ");
      fgets(shoe_size, 200, stdin);
      shoe_size[strlen(shoe_size)-1]=0;
      //scanf("%[^\t\n]", shoe_size);
      int shoe = atoi(shoe_size);
      printf("Please enter your favorite food: ");
      fgets(fav_food, 200, stdin);
      fav_food[strlen(fav_food)-1]=0;
      //scanf("%[^\t\n]", fav_food);
      bool check = search(table, email);
      if (check == true){
        delete(table, email);
        add_customer_to_table(email, name, fav_food, shoe , table);
      }
      add_customer_to_table(email, name, fav_food, shoe , table);

    }
    if (strcmp(input, "lookup") == 0){
      char email[200];
      printf("Please enter your email: ");
      fgets(email, 200, stdin);
      email[strlen(email)-1]=0;
      bool check = search(table, email);
      if (check == false){
        printf("Customer not found.");
      }
      else{
        lookup(table, email);

    }
  }
    if (strcmp(input, "delete") == 0){
      char email[200];
      printf("Please enter your email: ");
      fgets(email, 200, stdin);
      email[strlen(email)-1]=0;
      bool check = search(table, email);
      if (check == false){
        printf("Customer not found.");
      }
      else{
        delete(table,email);
        printf("Customer deleted");

      }
    }
    if (strcmp(input, "list") == 0){
      list(table);
    }
    if (strcmp(input, "save") == 0){
      save("customers.tsv", table);
      printf("Save Sucessful");


    }
    if (strcmp(input, "quit") == 0){
      delete_customer_table(table);

      break;


    }



  }
}
