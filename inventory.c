#include <stdio.h>
#include <string.h>
#include "inventory.h"

/* ---------------------------------------------------------------
 * Internal helper: open the data file, returning NULL on error.
 * --------------------------------------------------------------- */
static FILE *open_file(const char *mode)
{
    return fopen(DATA_FILE, mode);
}

/* ---------------------------------------------------------------
 * add_item — append a new record to the binary file.
 * Returns 1 on success, 0 on failure.
 * Rejects duplicate IDs.
 * --------------------------------------------------------------- */
int add_item(const Item *item)
{
    if (!item || item->id <= 0) return 0;

    /* Check for duplicate ID */
    Item existing;
    if (get_item(item->id, &existing)) return 0; /* duplicate */

    FILE *f = open_file("ab");
    if (!f) return 0;
    size_t written = fwrite(item, sizeof(Item), 1, f);
    fclose(f);
    return (written == 1) ? 1 : 0;
}

/* ---------------------------------------------------------------
 * get_item — find a non-deleted item by ID.
 * Returns 1 and fills *out on success, 0 if not found / deleted.
 * --------------------------------------------------------------- */
int get_item(int id, Item *out)
{
    if (!out || id <= 0) return 0;

    FILE *f = open_file("rb");
    if (!f) return 0;

    Item tmp;
    int found = 0;
    while (fread(&tmp, sizeof(Item), 1, f) == 1) {
        if (tmp.id == id && tmp.is_deleted == 0) {
            *out = tmp;
            found = 1;
            break;
        }
    }
    fclose(f);
    return found;
}

/* ---------------------------------------------------------------
 * update_item — overwrite the matching record in place.
 * Returns 1 on success, 0 if not found / deleted.
 * --------------------------------------------------------------- */
int update_item(int id, const Item *updated)
{
    if (!updated || id <= 0) return 0;

    FILE *f = open_file("r+b");
    if (!f) return 0;

    Item tmp;
    int found = 0;
    while (fread(&tmp, sizeof(Item), 1, f) == 1) {
        if (tmp.id == id && tmp.is_deleted == 0) {
            /* Step back one record and overwrite */
            fseek(f, -(long)sizeof(Item), SEEK_CUR);
            Item to_write = *updated;
            to_write.id = id;            /* preserve original ID */
            to_write.is_deleted = 0;
            fwrite(&to_write, sizeof(Item), 1, f);
            found = 1;
            break;
        }
    }
    fclose(f);
    return found;
}

/* ---------------------------------------------------------------
 * delete_item — soft delete: set is_deleted = 1 in place.
 * Returns 1 on success, 0 if not found / already deleted.
 * --------------------------------------------------------------- */
int delete_item(int id)
{
    if (id <= 0) return 0;

    FILE *f = open_file("r+b");
    if (!f) return 0;

    Item tmp;
    int found = 0;
    while (fread(&tmp, sizeof(Item), 1, f) == 1) {
        if (tmp.id == id && tmp.is_deleted == 0) {
            fseek(f, -(long)sizeof(Item), SEEK_CUR);
            tmp.is_deleted = 1;
            fwrite(&tmp, sizeof(Item), 1, f);
            found = 1;
            break;
        }
    }
    fclose(f);
    return found;
}

/* ---------------------------------------------------------------
 * list_items — copy all non-deleted items into buffer.
 * Returns the number of items copied (up to max_items).
 * --------------------------------------------------------------- */
int list_items(Item *buffer, int max_items)
{
    if (!buffer || max_items <= 0) return 0;

    FILE *f = open_file("rb");
    if (!f) return 0;

    Item tmp;
    int count = 0;
    while (count < max_items && fread(&tmp, sizeof(Item), 1, f) == 1) {
        if (tmp.is_deleted == 0) {
            buffer[count++] = tmp;
        }
    }
    fclose(f);
    return count;
}
