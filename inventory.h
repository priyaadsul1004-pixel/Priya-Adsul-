#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_NAME_LEN 40
#define DATA_FILE "inventory.dat"

/* Item structure stored in binary file */
typedef struct {
    int   id;
    char  name[MAX_NAME_LEN];
    int   quantity;
    float price;
    int   is_deleted; /* 0 = active, 1 = deleted */
} Item;

/* C backend API — returns 1 on success, 0 on failure */
int add_item(const Item *item);
int get_item(int id, Item *out);
int update_item(int id, const Item *updated);
int delete_item(int id);
int list_items(Item *buffer, int max_items);

#endif /* INVENTORY_H */
