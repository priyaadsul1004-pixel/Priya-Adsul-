#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include "InventoryManager.hpp"

/* ---------------------------------------------------------------
 * Input helpers — re-ask on invalid input (never crash).
 * --------------------------------------------------------------- */

static int readPositiveInt(const std::string &prompt)
{
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v > 0) return v;
        std::cout << "  [!] Must be a positive integer. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static int readNonNegInt(const std::string &prompt)
{
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= 0) return v;
        std::cout << "  [!] Must be 0 or more. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static float readNonNegFloat(const std::string &prompt)
{
    float v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v && v >= 0.0f) return v;
        std::cout << "  [!] Must be 0.00 or more. Try again.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static std::string readNonEmptyString(const std::string &prompt)
{
    std::string s;
    while (true) {
        std::cout << prompt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, s);
        if (!s.empty()) return s;
        std::cout << "  [!] Name must not be empty. Try again.\n";
    }
}

/* ---------------------------------------------------------------
 * Display helper
 * --------------------------------------------------------------- */
static void printHeader()
{
    std::cout << "\n"
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(22) << "Name"
              << std::setw(10) << "Quantity"
              << std::setw(10) << "Price"
              << "\n"
              << std::string(48, '-') << "\n";
}

static void printItem(const Item &it)
{
    std::cout << std::left
              << std::setw(6)  << it.id
              << std::setw(22) << it.name
              << std::setw(10) << it.quantity
              << "$" << std::fixed << std::setprecision(2) << it.price
              << "\n";
}

/* ---------------------------------------------------------------
 * Menu handlers
 * --------------------------------------------------------------- */

static void handleAdd(InventoryManager &mgr)
{
    std::cout << "\n--- Add Item ---\n";
    int id       = readPositiveInt("  Enter ID       : ");
    std::string name = readNonEmptyString("  Enter Name     : ");
    int qty      = readNonNegInt("  Enter Quantity : ");
    float price  = readNonNegFloat("  Enter Price    : ");

    if (mgr.addItem(id, name, qty, price))
        std::cout << "  [+] Item added successfully.\n";
    else
        std::cout << "  [!] Failed to add item (duplicate ID or invalid data).\n";
}

static void handleView(InventoryManager &mgr)
{
    std::cout << "\n--- View Item ---\n";
    int id = readPositiveInt("  Enter ID : ");
    Item it;
    if (mgr.viewItem(id, it)) {
        printHeader();
        printItem(it);
    } else {
        std::cout << "  [!] Item not found or deleted.\n";
    }
}

static void handleUpdate(InventoryManager &mgr)
{
    std::cout << "\n--- Update Item ---\n";
    int id = readPositiveInt("  Enter ID of item to update : ");

    Item existing;
    if (!mgr.viewItem(id, existing)) {
        std::cout << "  [!] Item not found or deleted.\n";
        return;
    }
    std::cout << "  Current: " << existing.name
              << "  Qty=" << existing.quantity
              << "  Price=$" << std::fixed << std::setprecision(2) << existing.price << "\n";

    std::string name = readNonEmptyString("  New Name     : ");
    int qty          = readNonNegInt("  New Quantity : ");
    float price      = readNonNegFloat("  New Price    : ");

    if (mgr.updateItem(id, name, qty, price))
        std::cout << "  [+] Item updated successfully.\n";
    else
        std::cout << "  [!] Update failed.\n";
}

static void handleDelete(InventoryManager &mgr)
{
    std::cout << "\n--- Delete Item ---\n";
    int id = readPositiveInt("  Enter ID of item to delete : ");
    if (mgr.deleteItem(id))
        std::cout << "  [+] Item soft-deleted (hidden from view).\n";
    else
        std::cout << "  [!] Item not found or already deleted.\n";
}

static void handleList(InventoryManager &mgr)
{
    std::cout << "\n--- List All Items ---\n";
    std::cout << "  Sort by: (1) ID  (2) Name  -> ";
    int choice;
    std::cin >> choice;

    auto by = (choice == 2)
              ? InventoryManager::SortBy::NAME
              : InventoryManager::SortBy::ID;

    auto items = mgr.listAll(by);
    if (items.empty()) {
        std::cout << "  (no active items)\n";
        return;
    }
    printHeader();
    for (const auto &it : items) printItem(it);
    std::cout << "  Total: " << items.size() << " item(s).\n";
}

/* ---------------------------------------------------------------
 * Main
 * --------------------------------------------------------------- */
int main()
{
    InventoryManager mgr;
    int choice = 0;

    std::cout << "=========================================\n";
    std::cout << "    Hybrid Inventory Manager  v1.0       \n";
    std::cout << "=========================================\n";

    while (true) {
        std::cout << "\n"
                  << "  1. Add item\n"
                  << "  2. View item\n"
                  << "  3. Update item\n"
                  << "  4. Delete item\n"
                  << "  5. List all\n"
                  << "  6. Exit\n"
                  << "  Choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "  [!] Invalid choice.\n";
            continue;
        }

        switch (choice) {
            case 1: handleAdd(mgr);    break;
            case 2: handleView(mgr);   break;
            case 3: handleUpdate(mgr); break;
            case 4: handleDelete(mgr); break;
            case 5: handleList(mgr);   break;
            case 6:
                std::cout << "\n  Goodbye!\n";
                return 0;
            default:
                std::cout << "  [!] Please enter 1–6.\n";
        }
    }
}
