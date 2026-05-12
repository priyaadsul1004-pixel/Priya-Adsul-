#ifndef INVENTORY_MANAGER_HPP
#define INVENTORY_MANAGER_HPP

#include <vector>
#include <string>
#include <algorithm>

extern "C" {
#include "inventory.h"
}

/* ---------------------------------------------------------------
 * InventoryManager — C++ layer that calls the C backend.
 * Uses std::vector for temporary listing and std::sort for sorting.
 * --------------------------------------------------------------- */
class InventoryManager {
public:
    /* Maximum items we ever load at once */
    static constexpr int MAX_ITEMS = 1024;

    /* ---- CRUD wrappers ---- */

    bool addItem(int id, const std::string &name, int quantity, float price)
    {
        if (id <= 0 || name.empty() || quantity < 0 || price < 0.0f)
            return false;

        Item item{};
        item.id = id;
        item.quantity = quantity;
        item.price = price;
        item.is_deleted = 0;
        /* Safe copy — always NUL-terminated */
        name.copy(item.name, MAX_NAME_LEN - 1);
        item.name[MAX_NAME_LEN - 1] = '\0';

        return add_item(&item) == 1;
    }

    bool viewItem(int id, Item &out) const
    {
        return get_item(id, &out) == 1;
    }

    bool updateItem(int id, const std::string &name, int quantity, float price)
    {
        if (id <= 0 || name.empty() || quantity < 0 || price < 0.0f)
            return false;

        Item item{};
        item.id = id;
        item.quantity = quantity;
        item.price = price;
        item.is_deleted = 0;
        name.copy(item.name, MAX_NAME_LEN - 1);
        item.name[MAX_NAME_LEN - 1] = '\0';

        return update_item(id, &item) == 1;
    }

    bool deleteItem(int id)
    {
        return delete_item(id) == 1;
    }

    /* ---- List + sort ---- */

    enum class SortBy { ID, NAME };

    /* Returns sorted vector of all active items */
    std::vector<Item> listAll(SortBy by = SortBy::ID) const
    {
        Item buffer[MAX_ITEMS];
        int count = list_items(buffer, MAX_ITEMS);

        std::vector<Item> items(buffer, buffer + count);

        if (by == SortBy::NAME) {
            std::sort(items.begin(), items.end(),
                      [](const Item &a, const Item &b) {
                          return std::string(a.name) < std::string(b.name);
                      });
        } else {
            std::sort(items.begin(), items.end(),
                      [](const Item &a, const Item &b) {
                          return a.id < b.id;
                      });
        }

        return items;
    }
};

#endif /* INVENTORY_MANAGER_HPP */
