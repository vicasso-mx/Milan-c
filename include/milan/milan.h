#ifndef MILAN_MILAN_H
#define MILAN_MILAN_H

#include <stddef.h>
#include <stdint.h>

#define MILAN_PRODUCT_CAPACITY 12
#define MILAN_CART_MAX_QUANTITY 99
#define MILAN_TEXT_CAPACITY 512
#define MILAN_PATH_CAPACITY 256

typedef enum
{
    MILAN_OK = 0,
    MILAN_PRODUCT_NOT_FOUND,
    MILAN_ITEM_NOT_FOUND,
    MILAN_INVALID_QUANTITY,
    MILAN_QUANTITY_LIMIT,
    MILAN_DATA_ERROR,
    MILAN_IO_ERROR
} MilanResult;

typedef struct
{
    int id;
    char name[MILAN_TEXT_CAPACITY];
    char description[MILAN_TEXT_CAPACITY];
    char brand[MILAN_TEXT_CAPACITY];
    char category[MILAN_TEXT_CAPACITY];
    int64_t price_cents;
    char image[MILAN_PATH_CAPACITY];
} MilanProduct;

typedef struct
{
    MilanProduct products[MILAN_PRODUCT_CAPACITY];
    size_t count;
} MilanCatalog;

typedef struct
{
    int product_id;
    unsigned quantity;
} MilanCartLine;

typedef struct
{
    MilanCartLine lines[MILAN_PRODUCT_CAPACITY];
    size_t count;
} MilanCart;

typedef struct
{
    MilanCartLine line;
    const MilanProduct *product;
    int64_t subtotal_cents;
} MilanCartItem;

typedef struct
{
    MilanCartItem items[MILAN_PRODUCT_CAPACITY];
    size_t line_count;
    unsigned total_units;
    int64_t total_cents;
} MilanCartSummary;

MilanResult milan_catalog_load(const char *path, MilanCatalog *catalog);
const MilanProduct *milan_catalog_find(const MilanCatalog *catalog, int id);
void milan_cart_init(MilanCart *cart);
MilanResult milan_cart_add(MilanCart *cart, const MilanCatalog *catalog, int id, int quantity);
MilanResult milan_cart_set_quantity(MilanCart *cart, int id, int quantity);
MilanResult milan_cart_remove(MilanCart *cart, int id);
MilanResult milan_cart_summary(const MilanCart *cart, const MilanCatalog *catalog,
                               MilanCartSummary *summary);
const char *milan_result_message(MilanResult result);
void milan_print_product(const MilanProduct *product, int detail);
void milan_print_cart(const MilanCartSummary *summary);
int milan_console_run(const MilanCatalog *catalog, MilanCart *cart);

#endif
