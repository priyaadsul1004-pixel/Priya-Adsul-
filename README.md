# Hybrid Inventory Manager

A capstone project demonstrating a **hybrid C/C++ architecture**:

| Layer | Language | Responsibilities |
|-------|----------|-----------------|
| Data  | C        | Binary file I/O (`fread/fwrite/fseek`), struct storage |
| UI    | C++      | Classes, STL (`std::vector`, `std::sort`), menu, input validation |

---

## File Structure

```
inventory_project/
├── include/
│   └── inventory.h          # C struct + function declarations
├── src/
│   ├── inventory.c          # C backend (file read/write/update/delete)
│   ├── InventoryManager.hpp # C++ class wrapping C functions
│   └── main.cpp             # C++ menu driver
├── Makefile
└── README.md
```

---

## Build & Run

### Prerequisites
- GCC / G++ with C11 and C++17 support (standard on Linux/macOS)
- `make`

### Build
```bash
make
```

### Run
```bash
./inventory
```

### Clean
```bash
make clean
```

---

## Menu Options

```
1. Add item      — store a new item with unique ID
2. View item     — find and display one item by ID
3. Update item   — modify name, quantity, or price
4. Delete item   — soft-delete (hidden from view/list)
5. List all      — show all active items, sorted by ID or Name
6. Exit
```

---

## Test Cases

1. **Persistence after restart**
   - Add 3 items → choose `6 Exit` → re-run `./inventory` → choose `5 List all`
   - ✅ All 3 items appear

2. **Duplicate ID rejected**
   - Add an item with ID `10` → add another with the same ID `10`
   - ✅ Second add prints a failure message; only one record in list

3. **Update persists**
   - Add item ID `1`, name "Apple", qty 5, price 1.00
   - Update ID `1` → name "Apple Premium", qty 10, price 2.50
   - Exit and re-run → View ID `1`
   - ✅ Updated values are shown

4. **Soft delete not visible**
   - Add items ID 1, 2, 3 → Delete ID 2
   - Choose `5 List all` → View ID `2`
   - ✅ Item 2 does not appear in list or view

5. **Input validation**
   - On "Add item", enter `-5` for ID → enter `0` for ID → enter `abc` for quantity → enter `-1` for price
   - ✅ Each invalid input shows an error and re-prompts; app never crashes

---

## Design Notes

- **Binary storage**: each `Item` struct is stored as a fixed-size binary record using `fwrite`. Updates and deletes use `fseek` to overwrite the exact record in place — O(1) writes.
- **Soft delete**: `is_deleted = 1` marks a record hidden; the slot is preserved so no compaction is needed.
- **STL usage**: `std::vector` holds the in-memory listing buffer; `std::sort` with a lambda sorts by ID or Name before printing.
- **No global state**: all file operations open/close the file handle per call, keeping the C backend stateless.
