the main interaction loop:

  The first thing I did when doing this assignment is making a .h file with my structs:

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

  Listed above are the structs I used when creating this database. The first step I took into making this database of customers was making the table using the build_customer_table() I implemented.

 build_customer_table:
  In the build customer table function I allocate a new customer table and set the num_buckets to whatever the parameter so in my case I passed in 1000 buckets. Then I just allocate space for the num_buckets pointer which are already null pointers. Then I return the customer table I created.

  The next step I did in my main was load the customer database from the tsv file into the table I created.

load_customers:
    To load the customers from the file I first open it using fopen(). I define BUF_SIZE of size 1024 and made a buf char and one for the key and values. Then I run a while loop using fgets() that reads from the file provided and puts it into the buffer I made. Then I set the key, email in our case, to strtok() which seperates the string into tokens based on the deliemeter we set so in our cause it was "\t\n". To convert the char shoe size to an int I used atoi() so I can properly pass it into the add_customer_to_table(). Then I close the file.


add_customer_to_table:
  To add the add_customer_to_table I get the hashvalue by doing the hash function on the key. Then I set the variable which_bucket to the hashvalue % total number of buckets in the table. Doing this will give me the specific bucket the key is in. Then I make a new CustomerNode and set it equal to the specific bucket in the table that we get based on the hash function. Then I set add it to the list using the add_customer_to_list function I create.

add_customer_to_list:
  To add the customer to the list I make a new CustomerNode node and allocate it using calloc. Then I copy over the parameter of function using strdup() to the node->key or node->value. Then I set the value of next to node being passed into the function.

After loading the customers from the file to the table I have a continous while that asks user for input until they say quit in which case it will break out of the loop and free the memory. 
